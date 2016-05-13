// ----------------------------------------------------------------------------
// Server.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Server.h"


namespace ModularDevice
{
Server::Server() :
  eeprom_initialized_sv_(constants::eeprom_initialized_default_value)
{
  setup();
}

Server::Server(Stream &stream)
{
  addServerStream(stream);
  setup();
}

void Server::setup()
{
  setName(constants::empty_constant_string);
  model_number_ = 0;
  firmware_major_ = 0;
  firmware_minor_ = 0;
  firmware_patch_ = 0;
  request_method_index_ = -1;
  parameter_count_ = 0;
  error_ = false;
  result_key_in_response_ = false;
  server_stream_index_ = 0;

  eeprom_initialized_ = false;

  // Fields
  Field& serial_number_field = createInternalField(constants::serial_number_field_name,constants::serial_number_default);
  serial_number_field.setRange(constants::serial_number_min,constants::serial_number_max);

  // Parameters
  Parameter& field_name_parameter = createInternalParameter(constants::field_name_parameter_name);
  field_name_parameter.setTypeString();

  Parameter& field_value_parameter = createInternalParameter(constants::field_value_parameter_name);
  field_value_parameter.setTypeValue();

  Parameter& field_element_index_parameter = createInternalParameter(constants::field_element_index_parameter_name);
  field_element_index_parameter.setTypeLong();

  // Methods
  InternalMethod& get_device_info_method = createInternalMethod(constants::get_device_info_method_name,true);
  get_device_info_method.attachCallback(&Server::getDeviceInfoCallback);

  InternalMethod& get_method_ids_method = createInternalMethod(constants::get_method_ids_method_name,true);
  get_method_ids_method.attachCallback(&Server::getMethodIdsCallback);

  InternalMethod& get_parameters_method = createInternalMethod(constants::get_parameters_method_name,true);
  get_parameters_method.attachCallback(&Server::getParametersCallback);

  InternalMethod& help_method = createInternalMethod(constants::help_method_name,true);
  help_method.attachCallback(&Server::helpCallback);

  InternalMethod& verbose_help_method = createInternalMethod(constants::verbose_help_method_name,true);
  verbose_help_method.attachCallback(&Server::verboseHelpCallback);

#ifdef __AVR__
  InternalMethod& get_memory_free_method = createInternalMethod(constants::get_memory_free_method_name);
  get_memory_free_method.attachCallback(&Server::getMemoryFreeCallback);
  get_memory_free_method.setReturnTypeLong();
#endif

  InternalMethod& get_field_default_values_method = createInternalMethod(constants::get_field_default_values_method_name);
  get_field_default_values_method.attachCallback(&Server::getFieldDefaultValuesCallback);

  InternalMethod& set_fields_to_defaults_method = createInternalMethod(constants::set_fields_to_defaults_method_name);
  set_fields_to_defaults_method.attachCallback(&Server::setFieldsToDefaultsCallback);

  InternalMethod& set_field_to_default_method = createInternalMethod(constants::set_field_to_default_method_name);
  set_field_to_default_method.attachCallback(&Server::setFieldToDefaultCallback);
  set_field_to_default_method.addParameter(field_name_parameter);

  InternalMethod& get_field_values_method = createInternalMethod(constants::get_field_values_method_name);
  get_field_values_method.attachCallback(&Server::getFieldValuesCallback);
  get_field_values_method.setReturnTypeObject();

  InternalMethod& get_field_value_method = createInternalMethod(constants::get_field_value_method_name);
  get_field_value_method.attachCallback(&Server::getFieldValueCallback);
  get_field_value_method.addParameter(field_name_parameter);
  get_field_value_method.setReturnTypeValue();

  InternalMethod& get_field_element_value_method = createInternalMethod(constants::get_field_element_value_method_name);
  get_field_element_value_method.attachCallback(&Server::getFieldElementValueCallback);
  get_field_element_value_method.addParameter(field_name_parameter);
  get_field_element_value_method.addParameter(field_element_index_parameter);
  get_field_element_value_method.setReturnTypeValue();

  InternalMethod& set_field_value_method = createInternalMethod(constants::set_field_value_method_name);
  set_field_value_method.attachCallback(&Server::setFieldValueCallback);
  set_field_value_method.addParameter(field_name_parameter);
  set_field_value_method.addParameter(field_value_parameter);

  InternalMethod& set_field_element_value_method = createInternalMethod(constants::set_field_element_value_method_name);
  set_field_element_value_method.attachCallback(&Server::setFieldElementValueCallback);
  set_field_element_value_method.addParameter(field_name_parameter);
  set_field_element_value_method.addParameter(field_element_index_parameter);
  set_field_element_value_method.addParameter(field_value_parameter);

  server_running_ = false;
}

void Server::addServerStream(Stream &stream)
{
  bool stream_found = false;
  for (unsigned int i=0;i<server_stream_ptrs_.size();++i)
  {
    if (server_stream_ptrs_[i] == &stream)
    {
      stream_found = true;
    }
  }
  if (!stream_found)
  {
    server_stream_ptrs_.push_back(&stream);
  }
  json_stream_.setStream(stream);
}

void Server::setName(const ConstantString &name)
{
  name_ptr_ = &name;
}

void Server::setModelNumber(const long model_number)
{
  model_number_ = model_number;
}

void Server::setFirmwareVersion(const long firmware_major,const long firmware_minor,const long firmware_patch)
{
  firmware_major_ = firmware_major;
  firmware_minor_ = firmware_minor;
  firmware_patch_ = firmware_patch;
}

InternalMethod& Server::createInternalMethod(const ConstantString &method_name, bool is_private)
{
  int method_index = findMethodIndex(method_name);
  if ((method_index < 0) || (method_index >= (int)internal_methods_.max_size()))
  {
    internal_methods_.push_back(InternalMethod(method_name));
    internal_methods_.back().setPrivacy(is_private);
    return internal_methods_.back();
  }
  else
  {
    internal_methods_[method_index] = InternalMethod(method_name);
    internal_methods_[method_index].setPrivacy(is_private);
    return internal_methods_[method_index];
  }
}

Method& Server::createMethod(const ConstantString &method_name)
{
  int method_index = findMethodIndex(method_name);
  if ((method_index < 0) || (method_index < (int)internal_methods_.max_size()))
  {
    external_methods_.push_back(Method(method_name));
    return external_methods_.back();
  }
  else
  {
    method_index -= internal_methods_.max_size();
    external_methods_[method_index] = Method(method_name);
    return external_methods_[method_index];
  }
}

Method& Server::copyMethod(Method method,const ConstantString &method_name)
{
  external_methods_.push_back(method);
  external_methods_.back().setName(method_name);
  return external_methods_.back();
}

Parameter& Server::createInternalParameter(const ConstantString &parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if ((parameter_index < 0) || (parameter_index >= (int)internal_parameters_.max_size()))
  {
    internal_parameters_.push_back(Parameter(parameter_name));
    return internal_parameters_.back();
  }
  else
  {
    internal_parameters_[parameter_index] = Parameter(parameter_name);
    return internal_parameters_[parameter_index];
  }
}

Parameter& Server::createParameter(const ConstantString &parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if ((parameter_index < 0) || (parameter_index < (int)internal_parameters_.max_size()))
  {
    external_parameters_.push_back(Parameter(parameter_name));
    return external_parameters_.back();
  }
  else
  {
    parameter_index -= internal_parameters_.max_size();
    external_parameters_[parameter_index] = Parameter(parameter_name);
    return external_parameters_[parameter_index];
  }
}

Parameter& Server::copyParameter(Parameter parameter,const ConstantString &parameter_name)
{
  external_parameters_.push_back(parameter);
  external_parameters_.back().setName(parameter_name);
  return external_parameters_.back();
}

ArduinoJson::JsonVariant Server::getParameterValue(const ConstantString &name)
{
  int parameter_index = findMethodParameterIndex(request_method_index_,name);
  // index 0 is the method, index 1 is the first parameter
  return (*request_json_array_ptr_)[parameter_index+1];
}

void Server::writeNullToResponse()
{
  json_stream_.writeNull();
}

void Server::writeResultKeyToResponse()
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.writeKey(constants::result_constant_string);
  }
}

void Server::beginResponseObject()
{
  json_stream_.beginObject();
}

void Server::endResponseObject()
{
  json_stream_.endObject();
}

void Server::beginResponseArray()
{
  json_stream_.beginArray();
}

void Server::endResponseArray()
{
  json_stream_.endArray();
}

void Server::setFieldsToDefaults()
{
  for (unsigned int i=0; i<internal_fields_.size(); ++i)
  {
    internal_fields_[i].setDefaultValue();
  }
  for (unsigned int i=0; i<external_fields_.size(); ++i)
  {
    external_fields_[i].setDefaultValue();
  }
}

void Server::startServer()
{
  if (!eeprom_initialized_)
  {
    initializeEeprom();
  }
  server_running_ = true;
}

void Server::stopServer()
{
  server_running_ = false;
}

void Server::handleRequest()
{
  if (server_running_ && (server_stream_ptrs_.size() > 0) && (json_stream_.available() > 0))
  {
    int bytes_read = json_stream_.readJsonIntoBuffer(request_,constants::STRING_LENGTH_REQUEST);
    if (bytes_read > 0)
    {
      JsonSanitizer sanitizer(constants::JSON_TOKEN_MAX);
      if (sanitizer.firstCharIsValidJson(request_))
      {
        json_stream_.setCompactPrint();
      }
      else
      {
        json_stream_.setPrettyPrint();
      }
      json_stream_.beginObject();
      error_ = false;
      result_key_in_response_ = false;
      sanitizer.sanitize(request_,constants::STRING_LENGTH_REQUEST);
      StaticJsonBuffer<constants::STRING_LENGTH_REQUEST> json_buffer;
      if (sanitizer.firstCharIsValidJsonObject(request_))
      {
        error_ = true;
        writeKeyToResponse(constants::error_constant_string);
        beginResponseObject();
        writeToResponse(constants::message_constant_string,constants::server_error_error_message);
        writeToResponse(constants::data_constant_string,constants::object_request_error_data);
        writeToResponse(constants::code_constant_string,constants::server_error_error_code);
        endResponseObject();
      }
      else
      {
        request_json_array_ptr_ = &(json_buffer.parseArray(request_));
        if (request_json_array_ptr_->success())
        {
          processRequestArray();
        }
        else
        {
          error_ = true;
          writeKeyToResponse(constants::error_constant_string);
          beginResponseObject();
          writeToResponse(constants::message_constant_string,constants::parse_error_message);
          writeToResponse(constants::data_constant_string,request_);
          writeToResponse(constants::code_constant_string,constants::parse_error_code);
          endResponseObject();
        }
        if (!error_ && !result_key_in_response_)
        {
          writeNullToResponse(constants::result_constant_string);
        }
      }
      endResponseObject();
      json_stream_.writeNewline();
    }
    else if (bytes_read < 0)
    {
      json_stream_.setCompactPrint();
      json_stream_.beginObject();
      error_ = true;
      writeKeyToResponse(constants::error_constant_string);
      beginResponseObject();
      writeToResponse(constants::message_constant_string,constants::server_error_error_message);
      writeToResponse(constants::data_constant_string,constants::request_length_error_data);
      writeToResponse(constants::code_constant_string,constants::server_error_error_code);
      endResponseObject();
      endResponseObject();
      json_stream_.writeNewline();
    }
  }
  incrementServerStream();
}

void Server::processRequestArray()
{
  const char* method_string = (*request_json_array_ptr_)[0];
  request_method_index_ = processMethodString(method_string);
  if (request_method_index_ >= 0)
  {
    int array_elements_count = countJsonArrayElements((*request_json_array_ptr_));
    int parameter_count = array_elements_count - 1;
    // method ?
    if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],"?") == 0))
    {
      writeResultKeyToResponse();
      methodHelp(false,request_method_index_);
    }
    // method ??
    else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],"??") == 0))
    {
      writeResultKeyToResponse();
      methodHelp(true,request_method_index_);
    }
    // method parameter ?
    // method parameter ??
    else if ((parameter_count == 2) &&
             ((strcmp((*request_json_array_ptr_)[2],"?") == 0) ||
              (strcmp((*request_json_array_ptr_)[2],"??") == 0)))
    {
      int parameter_index = processParameterString((*request_json_array_ptr_)[1]);
      Parameter* parameter_ptr;
      if (request_method_index_ < (int)internal_methods_.max_size())
      {
        parameter_ptr = internal_methods_[request_method_index_].parameter_ptrs_[parameter_index];
      }
      else
      {
        int index = request_method_index_ - internal_methods_.max_size();
        parameter_ptr = external_methods_[index].parameter_ptrs_[parameter_index];
      }
      writeResultKeyToResponse();
      parameterHelp(*parameter_ptr);
    }
    else if (request_method_index_ < (int)internal_methods_.max_size())
    {
      if (internal_methods_[request_method_index_].isPrivate())
      {
        executeMethod();
      }
      else if (parameter_count != internal_methods_[request_method_index_].parameter_count_)
      {
        error_ = true;
        writeKeyToResponse(constants::error_constant_string);
        beginResponseObject();
        writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
        char incorrect_parameter_number[constants::incorrect_parameter_number_error_data.length()+1];
        constants::incorrect_parameter_number_error_data.copy(incorrect_parameter_number);
        char error_str[constants::STRING_LENGTH_ERROR];
        error_str[0] = 0;
        strcat(error_str,incorrect_parameter_number);
        char parameter_count_str[constants::STRING_LENGTH_PARAMETER_COUNT];
        dtostrf(parameter_count,0,0,parameter_count_str);
        strcat(error_str,parameter_count_str);
        strcat(error_str," given. ");
        dtostrf(internal_methods_[request_method_index_].parameter_count_,0,0,parameter_count_str);
        strcat(error_str,parameter_count_str);
        strcat(error_str," needed.");
        writeToResponse(constants::data_constant_string,error_str);
        writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
        endResponseObject();
      }
      else
      {
        bool parameters_ok = checkParameters();
        if (parameters_ok)
        {
          executeMethod();
        }
      }
    }
    else if (parameter_count != external_methods_[request_method_index_-internal_methods_.max_size()].parameter_count_)
    {
      error_ = true;
      writeKeyToResponse(constants::error_constant_string);
      beginResponseObject();
      writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
      char incorrect_parameter_number[constants::incorrect_parameter_number_error_data.length()+1];
      constants::incorrect_parameter_number_error_data.copy(incorrect_parameter_number);
      char error_str[constants::STRING_LENGTH_ERROR];
      error_str[0] = 0;
      strcat(error_str,incorrect_parameter_number);
      char parameter_count_str[constants::STRING_LENGTH_PARAMETER_COUNT];
      dtostrf(parameter_count,0,0,parameter_count_str);
      strcat(error_str,parameter_count_str);
      strcat(error_str," given. ");
      dtostrf(external_methods_[request_method_index_-internal_methods_.max_size()].parameter_count_,0,0,parameter_count_str);
      strcat(error_str,parameter_count_str);
      strcat(error_str," needed.");
      writeToResponse(constants::data_constant_string,error_str);
      writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
      endResponseObject();
    }
    else
    {
      bool parameters_ok = checkParameters();
      if (parameters_ok)
      {
        executeMethod();
      }
    }
  }
}

int Server::processMethodString(const char *method_string)
{
  int method_index = -1;
  int method_id = atoi(method_string);
  if (strcmp(method_string,"0") == 0)
  {
    method_index = 0;
    writeToResponse(constants::id_constant_string,0);
  }
  else if (method_id > 0)
  {
    method_index = method_id;
    writeToResponse(constants::id_constant_string,method_id);
  }
  else
  {
    method_index = findMethodIndex(method_string);
    writeToResponse(constants::id_constant_string,method_string);
  }
  if ((method_index < 0) ||
      ((method_index >= (int)internal_methods_.size()) && (method_index < (int)internal_methods_.max_size())) ||
      (method_index >= ((int)internal_methods_.max_size() + (int)external_methods_.size())))
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::method_not_found_error_message);
    writeToResponse(constants::code_constant_string,constants::method_not_found_error_code);
    endResponseObject();
    method_index = -1;
  }
  return method_index;
}

int Server::countJsonArrayElements(ArduinoJson::JsonArray &json_array)
{
  int elements_count = 0;
  for (ArduinoJson::JsonArray::iterator it=json_array.begin();
       it!=json_array.end();
       ++it)
  {
    elements_count++;
  }
  return elements_count;
}

void Server::executeMethod()
{
  if (request_method_index_ >= 0)
  {
    if (request_method_index_ < (int)internal_methods_.size())
    {
      internal_methods_[request_method_index_].callback(this);
    }
    else if ((request_method_index_ >= (int)internal_methods_.max_size()) &&
             (request_method_index_ < (int)(internal_methods_.max_size() + external_methods_.size())))
    {
      int index = request_method_index_ - internal_methods_.max_size();
      external_methods_[index].callback();
    }
  }
}

void Server::methodHelp(bool verbose, int method_index)
{
  beginResponseObject();
  if (method_index < (int)internal_methods_.max_size())
  {
    const ConstantString& method_name = internal_methods_[method_index].getName();
    writeToResponse(constants::name_constant_string,method_name);
  }
  else
  {
    int index = method_index - internal_methods_.max_size();
    const ConstantString& method_name = external_methods_[index].getName();
    writeToResponse(constants::name_constant_string,method_name);
  }

  writeKeyToResponse(constants::parameters_constant_string);
  json_stream_.beginArray();
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>* parameter_ptrs_ptr = NULL;
  if (method_index < (int)internal_methods_.max_size())
  {
    parameter_ptrs_ptr = &internal_methods_[method_index].parameter_ptrs_;
  }
  else
  {
    int index = method_index - internal_methods_.max_size();
    parameter_ptrs_ptr = &external_methods_[index].parameter_ptrs_;
  }
  for (unsigned int i=0; i<parameter_ptrs_ptr->size(); ++i)
  {
    if (verbose)
    {
      parameterHelp(*((*parameter_ptrs_ptr)[i]));
    }
    else
    {
      const ConstantString& parameter_name = (*parameter_ptrs_ptr)[i]->getName();
      writeToResponse(parameter_name);
    }
  }
  json_stream_.endArray();
  if (method_index < (int)internal_methods_.max_size())
  {
    writeToResponse(constants::result_type_constant_string,internal_methods_[method_index].getReturnType());
  }
  else
  {
    int index = method_index - internal_methods_.max_size();
    writeToResponse(constants::result_type_constant_string,external_methods_[index].getReturnType());
  }
  endResponseObject();
}

int Server::processParameterString(const char *parameter_string)
{
  int parameter_index = -1;
  int parameter_id = atoi(parameter_string);
  if (strcmp(parameter_string,"0") == 0)
  {
    parameter_index = 0;
  }
  else if (parameter_id > 0)
  {
    parameter_index = parameter_id;
  }
  else
  {
    parameter_index = findMethodParameterIndex(request_method_index_,parameter_string);
  }
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>* parameter_ptrs_ptr = NULL;
  if  (request_method_index_ < (int)internal_methods_.max_size())
  {
    parameter_ptrs_ptr = &internal_methods_[request_method_index_].parameter_ptrs_;
  }
  else
  {
    int index = request_method_index_ - internal_methods_.max_size();
    parameter_ptrs_ptr = &external_methods_[index].parameter_ptrs_;
  }
  if ((parameter_index < 0) || (parameter_index >= (int)parameter_ptrs_ptr->size()))
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
    writeToResponse(constants::data_constant_string,constants::parameter_not_found_error_data);
    writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
    endResponseObject();
    parameter_index = -1;
  }
  return parameter_index;
}

void Server::parameterHelp(Parameter &parameter, bool end_object)
{
  beginResponseObject();
  const ConstantString& parameter_name = parameter.getName();
  writeToResponse(constants::name_constant_string,parameter_name);

  char parameter_units[constants::STRING_LENGTH_PARAMETER_UNITS];
  parameter_units[0] = 0;
  const ConstantString& units = parameter.getUnits();
  units.copy(parameter_units);
  if (strcmp(parameter_units,"") != 0)
  {
    writeToResponse(constants::units_constant_string,parameter_units);
  }
  JsonStream::JsonTypes type = parameter.getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
      {
        writeToResponse(constants::type_constant_string,JsonStream::LONG_TYPE);
        if (parameter.rangeIsSet())
        {
          long min = parameter.getMin().l;
          long max = parameter.getMax().l;
          writeToResponse(constants::min_constant_string,min);
          writeToResponse(constants::max_constant_string,max);
        }
        break;
      }
    case JsonStream::DOUBLE_TYPE:
      {
        writeToResponse(constants::type_constant_string,JsonStream::DOUBLE_TYPE);
        if (parameter.rangeIsSet())
        {
          double min = parameter.getMin().d;
          double max = parameter.getMax().d;
          writeToResponse(constants::min_constant_string,min);
          writeToResponse(constants::max_constant_string,max);
        }
        break;
      }
    case JsonStream::BOOL_TYPE:
      {
        writeToResponse(constants::type_constant_string,JsonStream::BOOL_TYPE);
        break;
      }
    case JsonStream::NULL_TYPE:
      break;
    case JsonStream::STRING_TYPE:
      {
        writeToResponse(constants::type_constant_string,JsonStream::STRING_TYPE);
        break;
      }
    case JsonStream::OBJECT_TYPE:
      {
        writeToResponse(constants::type_constant_string,JsonStream::OBJECT_TYPE);
        break;
      }
    case JsonStream::ARRAY_TYPE:
      {
        writeToResponse(constants::type_constant_string,JsonStream::ARRAY_TYPE);
        JsonStream::JsonTypes array_element_type = parameter.getArrayElementType();
        switch (array_element_type)
        {
          case JsonStream::LONG_TYPE:
            {
              writeToResponse(constants::array_element_type_constant_string,JsonStream::LONG_TYPE);
              if (parameter.rangeIsSet())
              {
                long min = parameter.getMin().l;
                long max = parameter.getMax().l;
                writeToResponse(constants::min_constant_string,min);
                writeToResponse(constants::max_constant_string,max);
              }
              break;
            }
          case JsonStream::DOUBLE_TYPE:
            {
              writeToResponse(constants::array_element_type_constant_string,JsonStream::DOUBLE_TYPE);
              if (parameter.rangeIsSet())
              {
                double min = parameter.getMin().d;
                double max = parameter.getMax().d;
                writeToResponse(constants::min_constant_string,min);
                writeToResponse(constants::max_constant_string,max);
              }
              break;
            }
          case JsonStream::BOOL_TYPE:
            {
              writeToResponse(constants::array_element_type_constant_string,JsonStream::BOOL_TYPE);
              break;
            }
          case JsonStream::NULL_TYPE:
            break;
          case JsonStream::STRING_TYPE:
            {
              writeToResponse(constants::array_element_type_constant_string,JsonStream::STRING_TYPE);
              break;
            }
          case JsonStream::OBJECT_TYPE:
            {
              writeToResponse(constants::array_element_type_constant_string,JsonStream::OBJECT_TYPE);
              break;
            }
          case JsonStream::ARRAY_TYPE:
            {
              writeToResponse(constants::array_element_type_constant_string,JsonStream::ARRAY_TYPE);
              break;
            }
        }
        break;
      }
    case JsonStream::VALUE_TYPE:
      {
        writeToResponse(constants::type_constant_string,JsonStream::VALUE_TYPE);
        break;
      }
  }
  if (end_object)
  {
    endResponseObject();
  }
}

void Server::fieldHelp(Field &field)
{
  parameterHelp(field.getParameter(),false);
  writeKeyToResponse(constants::value_constant_string);
  writeFieldToResponse(field,false,false);
  writeKeyToResponse(constants::default_value_constant_string);
  writeFieldToResponse(field,false,true);
  endResponseObject();
}

bool Server::checkParameters()
{
  int parameter_index = 0;
  for (ArduinoJson::JsonArray::iterator it=request_json_array_ptr_->begin();
       it!=request_json_array_ptr_->end();
       ++it)
  {
    if (it!=request_json_array_ptr_->begin())
    {
      if (checkParameter(parameter_index,*it))
      {
        parameter_index++;
      }
      else
      {
        return false;
      }
    }
  }
  parameter_count_ = parameter_index;
  return true;
}

bool Server::checkParameter(int parameter_index, ArduinoJson::JsonVariant &json_value)
{
  bool out_of_range = false;
  bool object_parse_unsuccessful = false;
  bool array_parse_unsuccessful = false;
  Parameter* parameter_ptr = NULL;
  if  (request_method_index_ < (int)internal_methods_.max_size())
  {
    parameter_ptr = internal_methods_[request_method_index_].parameter_ptrs_[parameter_index];
  }
  else
  {
    int index = request_method_index_ - internal_methods_.max_size();
    parameter_ptr = external_methods_[index].parameter_ptrs_[parameter_index];
  }
  JsonStream::JsonTypes type = parameter_ptr->getType();
  char min_str[JsonStream::STRING_LENGTH_DOUBLE];
  min_str[0] = 0;
  char max_str[JsonStream::STRING_LENGTH_DOUBLE];
  max_str[0] = 0;
  switch (type)
  {
    case JsonStream::LONG_TYPE:
      {
        if (parameter_ptr->rangeIsSet())
        {
          long value = (long)json_value;
          long min = parameter_ptr->getMin().l;
          long max = parameter_ptr->getMax().l;
          if ((value < min) || (value > max))
          {
            out_of_range = true;
            dtostrf(min,0,0,min_str);
            dtostrf(max,0,0,max_str);
          }
        }
        break;
      }
    case JsonStream::DOUBLE_TYPE:
      {
        if (parameter_ptr->rangeIsSet())
        {
          double value = (double)json_value;
          double min = parameter_ptr->getMin().d;
          double max = parameter_ptr->getMax().d;
          if ((value < min) || (value > max))
          {
            out_of_range = true;
            dtostrf(min,0,JsonStream::DOUBLE_DIGITS_DEFAULT,min_str);
            dtostrf(max,0,JsonStream::DOUBLE_DIGITS_DEFAULT,max_str);
          }
        }
        break;
      }
    case JsonStream::BOOL_TYPE:
      break;
    case JsonStream::NULL_TYPE:
      break;
    case JsonStream::STRING_TYPE:
      break;
    case JsonStream::OBJECT_TYPE:
      {
        ArduinoJson::JsonObject& json_object = json_value;
        if (!json_object.success())
        {
          object_parse_unsuccessful = true;
        }
        break;
      }
    case JsonStream::ARRAY_TYPE:
      {
        ArduinoJson::JsonArray& json_array = json_value;
        if (!json_array.success())
        {
          array_parse_unsuccessful = true;
        }
        else
        {
          JsonStream::JsonTypes array_element_type = parameter_ptr->getArrayElementType();
          switch (array_element_type)
          {
            case JsonStream::LONG_TYPE:
              {
                if (parameter_ptr->rangeIsSet())
                {
                  long value;
                  long min = parameter_ptr->getMin().l;
                  long max = parameter_ptr->getMax().l;
                  for (ArduinoJson::JsonArray::iterator it=json_array.begin();
                       it!=json_array.end();
                       ++it)
                  {
                    value = (long)*it;
                    if ((value < min) || (value > max))
                    {
                      out_of_range = true;
                      dtostrf(min,0,0,min_str);
                      dtostrf(max,0,0,max_str);
                      break;
                    }
                  }
                }
                break;
              }
            case JsonStream::DOUBLE_TYPE:
              {
                if (parameter_ptr->rangeIsSet())
                {
                  double value;
                  double min = parameter_ptr->getMin().d;
                  double max = parameter_ptr->getMax().d;
                  for (ArduinoJson::JsonArray::iterator it=json_array.begin();
                       it!=json_array.end();
                       ++it)
                  {
                    value = (double)*it;
                    if ((value < min) || (value > max))
                    {
                      out_of_range = true;
                      dtostrf(min,0,JsonStream::DOUBLE_DIGITS_DEFAULT,min_str);
                      dtostrf(max,0,JsonStream::DOUBLE_DIGITS_DEFAULT,max_str);
                      break;
                    }
                  }
                }
                break;
              }
            case JsonStream::BOOL_TYPE:
              break;
            case JsonStream::NULL_TYPE:
              break;
            case JsonStream::STRING_TYPE:
              break;
            case JsonStream::OBJECT_TYPE:
              break;
            case JsonStream::ARRAY_TYPE:
              break;
          }
        }
        break;
      }
  }
  if (out_of_range)
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = 0;
    if (type != JsonStream::ARRAY_TYPE)
    {
      constants::parameter_error_error_data.copy(error_str);
    }
    else
    {
      constants::array_parameter_error_error_data.copy(error_str);
    }
    strcat(error_str,min_str);
    strcat(error_str," <= ");
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME];
    parameter_name[0] = 0;
    const ConstantString& name = parameter_ptr->getName();
    name.copy(parameter_name);
    strcat(error_str,parameter_name);
    if (type == JsonStream::ARRAY_TYPE)
    {
      strcat(error_str," element");
    }
    strcat(error_str," <= ");
    strcat(error_str,max_str);
    writeToResponse(constants::data_constant_string,error_str);
    writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
    endResponseObject();
  }
  else if (object_parse_unsuccessful)
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME];
    parameter_name[0] = 0;
    const ConstantString& name = parameter_ptr->getName();
    name.copy(parameter_name);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = 0;
    strcat(error_str,parameter_name);
    char invalid_json_object[constants::invalid_json_object_error_data.length()+1];
    constants::invalid_json_object_error_data.copy(invalid_json_object);
    strcat(error_str,invalid_json_object);
    writeToResponse(constants::data_constant_string,error_str);
    writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
    endResponseObject();
  }
  else if (array_parse_unsuccessful)
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME];
    parameter_name[0] = 0;
    const ConstantString& name = parameter_ptr->getName();
    name.copy(parameter_name);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = 0;
    strcat(error_str,parameter_name);
    char invalid_json_array[constants::invalid_json_array_error_data.length()+1];
    constants::invalid_json_array_error_data.copy(invalid_json_array);
    strcat(error_str,invalid_json_array);
    writeToResponse(constants::data_constant_string,error_str);
    writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
    endResponseObject();
  }
  bool parameter_ok = (!out_of_range) && (!object_parse_unsuccessful) && (!array_parse_unsuccessful);
  return parameter_ok;
}

long Server::getSerialNumber()
{
  long serial_number;
  getFieldValue(constants::serial_number_field_name,serial_number);
  return serial_number;
}

void Server::initializeEeprom()
{
  if (!eeprom_initialized_sv_.isDefaultValue())
  {
    eeprom_initialized_sv_.setDefaultValue();
    setFieldsToDefaults();
  }
  eeprom_initialized_ = true;
}

void Server::incrementServerStream()
{
  if (server_stream_ptrs_.size() > 0)
  {
    server_stream_index_ = (server_stream_index_ + 1) % server_stream_ptrs_.size();
    json_stream_.setStream(*server_stream_ptrs_[server_stream_index_]);
  }
}

void Server::writeDeviceInfoToResponse()
{
  beginResponseObject();
  writeToResponse(constants::name_constant_string,name_ptr_);
  writeToResponse(constants::model_number_constant_string,model_number_);
  writeToResponse(constants::board_constant_string,constants::board_type_constant_string);
  writeToResponse(constants::serial_number_field_name,getSerialNumber());
  writeKeyToResponse(constants::firmware_version_constant_string);
  beginResponseObject();
  writeToResponse(constants::major_constant_string,firmware_major_);
  writeToResponse(constants::minor_constant_string,firmware_minor_);
  writeToResponse(constants::patch_constant_string,firmware_patch_);
  endResponseObject();
  endResponseObject();
}

void Server::writeFieldToResponse(Field &field, bool write_key, bool write_default, int element_index)
{
  const ConstantString& field_name = field.getParameter().getName();
  if (write_key)
  {
    writeKeyToResponse(field_name);
  }
  JsonStream::JsonTypes field_type = field.getParameter().getType();
  switch (field_type)
  {
    case JsonStream::LONG_TYPE:
      {
        if (element_index >= 0)
        {
          writeFieldErrorToResponse(constants::field_not_array_type_error_data);
          return;
        }
        long field_value;
        if (write_default)
        {
          getFieldDefaultValue(field_name,field_value);
        }
        else
        {
          getFieldValue(field_name,field_value);
        }
        writeToResponse(field_value);
        break;
      }
    case JsonStream::BOOL_TYPE:
      {
        if (element_index >= 0)
        {
          writeFieldErrorToResponse(constants::field_not_array_type_error_data);
          return;
        }
        bool field_value;
        if (write_default)
        {
          getFieldDefaultValue(field_name,field_value);
        }
        else
        {
          getFieldValue(field_name,field_value);
        }
        writeToResponse(field_value);
        break;
      }
    case JsonStream::ARRAY_TYPE:
      {
        const JsonStream::JsonTypes array_element_type = field.getParameter().getArrayElementType();
        unsigned int array_length = field.getArrayLength();
        if (element_index >= (int)array_length)
        {
          writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
          return;
        }
        switch (array_element_type)
        {
          case JsonStream::LONG_TYPE:
            {
              if (element_index < 0)
              {
                long field_value[array_length];
                if (write_default)
                {
                  for (int i;i<array_length;++i)
                  {
                    long v;
                    getFieldDefaultElementValue(field_name,i,v);
                    field_value[i] = v;
                  }
                }
                else
                {
                  for (int i;i<array_length;++i)
                  {
                    long v;
                    getFieldElementValue(field_name,i,v);
                    field_value[i] = v;
                  }
                }
                writeToResponse(field_value,array_length);
              }
              else
              {
                long field_value;
                if (write_default)
                {
                  getFieldDefaultElementValue(field_name,element_index,field_value);
                }
                else
                {
                  getFieldElementValue(field_name,element_index,field_value);
                }
                writeToResponse(field_value);
              }
              break;
            }
          case JsonStream::BOOL_TYPE:
            {
              if (element_index < 0)
              {
                bool field_value[array_length];
                if (write_default)
                {
                  for (int i;i<array_length;++i)
                  {
                    bool v;
                    getFieldDefaultElementValue(field_name,i,v);
                    field_value[i] = v;
                  }
                }
                else
                {
                  for (int i;i<array_length;++i)
                  {
                    bool v;
                    getFieldElementValue(field_name,i,v);
                    field_value[i] = v;
                  }
                }
                writeToResponse(field_value,array_length);
              }
              else
              {
                bool field_value;
                if (write_default)
                {
                  getFieldDefaultElementValue(field_name,element_index,field_value);
                }
                else
                {
                  getFieldElementValue(field_name,element_index,field_value);
                }
                writeToResponse(field_value);
              }
              break;
            }
        }
      }
  }
}

void Server::writeFieldErrorToResponse(const ConstantString &error)
{
  error_ = true;
  writeKeyToResponse(constants::error_constant_string);
  beginResponseObject();
  writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
  writeToResponse(constants::data_constant_string,error);
  writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
  endResponseObject();
}

void Server::help(bool verbose)
{
  int array_elements_count = countJsonArrayElements((*request_json_array_ptr_));
  int parameter_count = array_elements_count - 1;
  bool param_error = true;
  // ?
  // ??
  if (parameter_count == 0)
  {
    param_error = false;
    writeResultKeyToResponse();
    beginResponseObject();
    writeKeyToResponse(constants::device_info_constant_string);
    writeDeviceInfoToResponse();

    // ?
    if (!verbose)
    {
      writeKeyToResponse(constants::methods_constant_string);
      beginResponseArray();
      for (unsigned int method_index=0; method_index<internal_methods_.size(); ++method_index)
      {
        if (!internal_methods_[method_index].isPrivate())
        {
          const ConstantString& method_name = internal_methods_[method_index].getName();
          writeToResponse(method_name);
        }
      }
      for (unsigned int method_index=0; method_index<external_methods_.size(); ++method_index)
      {
        const ConstantString& method_name = external_methods_[method_index].getName();
        writeToResponse(method_name);
      }
      endResponseArray();

      writeKeyToResponse(constants::parameters_constant_string);
      beginResponseArray();
      for (unsigned int parameter_index=0; parameter_index<internal_parameters_.size(); ++parameter_index)
      {
        const ConstantString& parameter_name = internal_parameters_[parameter_index].getName();
        writeToResponse(parameter_name);
      }
      for (unsigned int parameter_index=0; parameter_index<external_parameters_.size(); ++parameter_index)
      {
        const ConstantString& parameter_name = external_parameters_[parameter_index].getName();
        writeToResponse(parameter_name);
      }
      endResponseArray();

      writeKeyToResponse(constants::fields_constant_string);
      beginResponseArray();
      for (unsigned int field_index=0; field_index<internal_fields_.size(); ++field_index)
      {
        const ConstantString& field_name = internal_fields_[field_index].getParameter().getName();
        writeToResponse(field_name);
      }
      for (unsigned int field_index=0; field_index<external_fields_.size(); ++field_index)
      {
        const ConstantString& field_name = external_fields_[field_index].getParameter().getName();
        writeToResponse(field_name);
      }
      endResponseArray();
    }
    // ??
    else
    {
      writeKeyToResponse(constants::methods_constant_string);
      beginResponseArray();
      for (unsigned int method_index=0; method_index<internal_methods_.size(); ++method_index)
      {
        if (!internal_methods_[method_index].isPrivate())
        {
          methodHelp(false,method_index);
        }
      }
      for (unsigned int method_index=0; method_index<external_methods_.size(); ++method_index)
      {
        int index = method_index + internal_methods_.max_size();
        methodHelp(false,index);
      }
      endResponseArray();

      writeKeyToResponse(constants::parameters_constant_string);
      beginResponseArray();
      for (unsigned int parameter_index=0; parameter_index<internal_parameters_.size(); ++parameter_index)
      {
        parameterHelp(internal_parameters_[parameter_index]);
      }
      for (unsigned int parameter_index=0; parameter_index<external_parameters_.size(); ++parameter_index)
      {
        parameterHelp(external_parameters_[parameter_index]);
      }
      endResponseArray();

      writeKeyToResponse(constants::fields_constant_string);
      beginResponseArray();
      for (unsigned int field_index=0; field_index<internal_fields_.size(); ++field_index)
      {
        fieldHelp(internal_fields_[field_index]);
      }
      for (unsigned int field_index=0; field_index<external_fields_.size(); ++field_index)
      {
        fieldHelp(external_fields_[field_index]);
      }
      endResponseArray();
    }
    endResponseObject();
  }
  // ? method
  // ? parameter
  // ? field
  // ?? method
  // ?? parameter
  // ?? field
  else if (parameter_count == 1)
  {
    const char* param_string = (*request_json_array_ptr_)[1];
    int method_index = findMethodIndex(param_string);
    if (method_index >= 0)
    {
      // ? method
      param_error = false;
      writeResultKeyToResponse();
      methodHelp(verbose,method_index);
    }
    else
    {
      int parameter_index = findParameterIndex(param_string);
      if (parameter_index >= 0)
      {
        // ? parameter
        // ?? parameter
        param_error = false;
        writeResultKeyToResponse();
        Parameter* parameter_ptr = NULL;
        if (parameter_index < (int)internal_parameters_.max_size())
        {
          parameter_ptr = &internal_parameters_[parameter_index];
        }
        else
        {
          int index = parameter_index - internal_parameters_.max_size();
          parameter_ptr = &external_parameters_[index];
        }
        parameterHelp(*parameter_ptr);
      }
      else
      {
        int field_index = findFieldIndex(param_string);
        if (field_index >= 0)
        {
          // ? field
          // ?? field
          param_error = false;
          writeResultKeyToResponse();
          Field* field_ptr = NULL;
          if (field_index < (int)internal_fields_.max_size())
          {
            field_ptr = &internal_fields_[field_index];
          }
          else
          {
            int index = field_index - internal_fields_.max_size();
            field_ptr = &external_fields_[index];
          }
          fieldHelp(*field_ptr);
        }
      }
    }
  }
  // ? method parameter
  // ?? method parameter
  else if (parameter_count == 2)
  {
    const char* method_string = (*request_json_array_ptr_)[1];
    int method_index = findMethodIndex(method_string);
    if (method_index >= 0)
    {
      int parameter_index = findMethodParameterIndex(method_index,(const char *)(*request_json_array_ptr_)[2]);
      if (parameter_index >= 0)
      {
        param_error = false;
        Parameter* parameter_ptr = NULL;
        if (method_index < (int)internal_methods_.max_size())
        {
          parameter_ptr = internal_methods_[method_index].parameter_ptrs_[parameter_index];
        }
        else
        {
          int index = method_index - internal_methods_.max_size();
          parameter_ptr = external_methods_[index].parameter_ptrs_[parameter_index];
        }
        writeResultKeyToResponse();
        parameterHelp(*parameter_ptr);
      }
    }
  }
  // ? unknown
  // ?? unknown
  // ? method unknown
  // ?? method unknown
  if (param_error)
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
    writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
    endResponseObject();
  }
}

void Server::getDeviceInfoCallback()
{
  writeResultKeyToResponse();
  writeDeviceInfoToResponse();
}

void Server::getMethodIdsCallback()
{
  writeResultKeyToResponse();
  beginResponseObject();
  for (unsigned int method_index=0; method_index<internal_methods_.size(); ++method_index)
  {
    if (!internal_methods_[method_index].isPrivate())
    {
      const ConstantString& method_name = internal_methods_[method_index].getName();
      writeToResponse(method_name,method_index);
    }
  }
  for (unsigned int method_index=0; method_index<external_methods_.size(); ++method_index)
  {
    const ConstantString& method_name = external_methods_[method_index].getName();
    int index = method_index + internal_methods_.max_size();
    writeToResponse(method_name,index);
  }
  endResponseObject();
}

void Server::getParametersCallback()
{
  writeResultKeyToResponse();
  beginResponseObject();
  writeKeyToResponse(constants::parameters_constant_string);
  json_stream_.beginArray();
  for (unsigned int parameter_index=0; parameter_index<internal_parameters_.size(); ++parameter_index)
  {
    parameterHelp(internal_parameters_[parameter_index]);
  }
  for (unsigned int parameter_index=0; parameter_index<external_parameters_.size(); ++parameter_index)
  {
    parameterHelp(external_parameters_[parameter_index]);
  }
  json_stream_.endArray();
  endResponseObject();
}

void Server::helpCallback()
{
  help(false);
}

void Server::verboseHelpCallback()
{
  help(true);
}

#ifdef __AVR__
void Server::getMemoryFreeCallback()
{
  writeResultToResponse(freeMemory());
}
#endif

void Server::getFieldDefaultValuesCallback()
{
  writeResultKeyToResponse();
  beginResponseObject();
  for (unsigned int i=0; i<internal_fields_.size(); ++i)
  {
    Field& field = internal_fields_[i];
    writeFieldToResponse(field,true,true);
  }
  for (unsigned int i=0; i<external_fields_.size(); ++i)
  {
    Field& field = external_fields_[i];
    writeFieldToResponse(field,true,true);
  }
  endResponseObject();
}

void Server::setFieldsToDefaultsCallback()
{
  setFieldsToDefaults();
}

void Server::setFieldToDefaultCallback()
{
  const char* field_name = getParameterValue(constants::field_name_parameter_name);
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    field.setDefaultValue();
  }
  else
  {
    writeFieldErrorToResponse(constants::field_not_found_error_data);
  }
}

void Server::getFieldValuesCallback()
{
  writeResultKeyToResponse();
  beginResponseObject();
  for (unsigned int i=0; i<internal_fields_.size(); ++i)
  {
    Field& field = internal_fields_[i];
    writeFieldToResponse(field,true,false);
  }
  for (unsigned int i=0; i<external_fields_.size(); ++i)
  {
    Field& field = external_fields_[i];
    writeFieldToResponse(field,true,false);
  }
  endResponseObject();
}

void Server::getFieldValueCallback()
{
  writeResultKeyToResponse();
  const char* field_name = getParameterValue(constants::field_name_parameter_name);
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    writeFieldToResponse(field,false,false);
  }
  else
  {
    writeFieldErrorToResponse(constants::field_not_found_error_data);
  }
}

void Server::getFieldElementValueCallback()
{
  writeResultKeyToResponse();
  const char* field_name = getParameterValue(constants::field_name_parameter_name);
  long field_element_index = getParameterValue(constants::field_element_index_parameter_name);
  if (field_element_index < 0)
  {
    writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
    return;
  }
  int field_index;
  Field& field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    writeFieldToResponse(field,false,false,field_element_index);
  }
  else
  {
    writeFieldErrorToResponse(constants::field_not_found_error_data);
  }
}

void Server::setFieldValueCallback()
{
  const char* field_name = getParameterValue(constants::field_name_parameter_name);
  int field_index;
  Field& field = findField(field_name,&field_index);
  const ConstantString& field_name_cs = field.getParameter().getName();
  if (field_index >= 0)
  {
    JsonStream::JsonTypes field_type = field.getParameter().getType();
    switch (field_type)
    {
      case JsonStream::LONG_TYPE:
        {
          long field_value = getParameterValue(constants::field_value_parameter_name);
          setFieldValue(field_name_cs,field_value);
          break;
        }
      case JsonStream::BOOL_TYPE:
        {
          bool field_value = getParameterValue(constants::field_value_parameter_name);
          setFieldValue(field_name_cs,field_value);;
          break;
        }
    }
  }
  else
  {
    writeFieldErrorToResponse(constants::field_not_found_error_data);
  }
}

void Server::setFieldElementValueCallback()
{
  const char* field_name = getParameterValue(constants::field_name_parameter_name);
  long field_element_index = getParameterValue(constants::field_element_index_parameter_name);
  if (field_element_index < 0)
  {
    writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
    return;
  }
  int field_index;
  Field& field = findField(field_name,&field_index);
  const ConstantString& field_name_cs = field.getParameter().getName();
  if (field_index >= 0)
  {
    JsonStream::JsonTypes field_type = field.getParameter().getType();
    switch (field_type)
    {
      case JsonStream::LONG_TYPE:
        {
          writeFieldErrorToResponse(constants::field_not_array_type_error_data);
          break;
        }
      case JsonStream::BOOL_TYPE:
        {
          writeFieldErrorToResponse(constants::field_not_array_type_error_data);
          break;
        }
      case JsonStream::ARRAY_TYPE:
        {
          JsonStream::JsonTypes array_element_type = field.getParameter().getArrayElementType();
          switch (array_element_type)
          {
            case JsonStream::LONG_TYPE:
              {
                long field_value = getParameterValue(constants::field_value_parameter_name);
                setFieldElementValue(field_name_cs,field_element_index,field_value);
                break;
              }
            case JsonStream::BOOL_TYPE:
              {
                bool field_value = getParameterValue(constants::field_value_parameter_name);
                setFieldElementValue(field_name_cs,field_element_index,field_value);
                break;
              }
          }
        }
    }
  }
  else
  {
    writeFieldErrorToResponse(constants::field_not_found_error_data);
  }
}

}
