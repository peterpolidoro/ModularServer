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

void Server::setup()
{
  setDeviceName(constants::empty_constant_string);
  setFirmwareName(constants::empty_constant_string);
  firmware_major_ = 0;
  firmware_minor_ = 0;
  firmware_patch_ = 0;
  setHardwareName(constants::empty_constant_string);
  model_number_ = 0;
  request_method_index_ = -1;
  parameter_count_ = 0;
  error_ = false;
  result_key_in_response_ = false;
  server_stream_index_ = 0;

  eeprom_initialized_ = false;

  // Add Storage
  addFieldStorage(server_fields_);
  addParameterStorage(server_parameters_);
  addMethodStorage(server_methods_);

  // Fields
  Field & serial_number_field = createField(constants::serial_number_field_name,constants::serial_number_default);
  serial_number_field.setRange(constants::serial_number_min,constants::serial_number_max);

  // Parameters
  Parameter & field_name_parameter = createParameter(constants::field_name_parameter_name);
  field_name_parameter.setTypeString();

  Parameter & field_value_parameter = createParameter(constants::field_value_parameter_name);
  field_value_parameter.setTypeValue();

  Parameter & field_element_index_parameter = createParameter(constants::field_element_index_parameter_name);
  field_element_index_parameter.setTypeLong();

  // Methods
  Method & get_device_info_method = createMethod(constants::get_device_info_method_name);
  get_device_info_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getDeviceInfoCallback));
  private_method_index_ = 0;

  Method & get_method_ids_method = createMethod(constants::get_method_ids_method_name);
  get_method_ids_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getMethodIdsCallback));
  private_method_index_++;

  Method & get_parameters_method = createMethod(constants::get_parameters_method_name);
  get_parameters_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getParametersCallback));
  private_method_index_++;

  Method & help_method = createMethod(constants::help_method_name);
  help_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::helpCallback));
  private_method_index_++;

  Method & verbose_help_method = createMethod(constants::verbose_help_method_name);
  verbose_help_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::verboseHelpCallback));
  private_method_index_++;

#ifdef __AVR__
  Method & get_memory_free_method = createMethod(constants::get_memory_free_method_name);
  get_memory_free_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getMemoryFreeCallback));
  get_memory_free_method.setReturnTypeLong();
#endif

  Method & get_field_default_values_method = createMethod(constants::get_field_default_values_method_name);
  get_field_default_values_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getFieldDefaultValuesCallback));

  Method & set_fields_to_defaults_method = createMethod(constants::set_fields_to_defaults_method_name);
  set_fields_to_defaults_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::setFieldsToDefaultsCallback));

  Method & set_field_to_default_method = createMethod(constants::set_field_to_default_method_name);
  set_field_to_default_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::setFieldToDefaultCallback));
  set_field_to_default_method.addParameter(field_name_parameter);

  Method & get_field_values_method = createMethod(constants::get_field_values_method_name);
  get_field_values_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getFieldValuesCallback));
  get_field_values_method.setReturnTypeObject();

  Method & get_field_value_method = createMethod(constants::get_field_value_method_name);
  get_field_value_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getFieldValueCallback));
  get_field_value_method.addParameter(field_name_parameter);
  get_field_value_method.setReturnTypeValue();

  Method & get_field_element_value_method = createMethod(constants::get_field_element_value_method_name);
  get_field_element_value_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::getFieldElementValueCallback));
  get_field_element_value_method.addParameter(field_name_parameter);
  get_field_element_value_method.addParameter(field_element_index_parameter);
  get_field_element_value_method.setReturnTypeValue();

  Method & set_field_value_method = createMethod(constants::set_field_value_method_name);
  set_field_value_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::setFieldValueCallback));
  set_field_value_method.addParameter(field_name_parameter);
  set_field_value_method.addParameter(field_value_parameter);

  Method & set_field_element_value_method = createMethod(constants::set_field_element_value_method_name);
  set_field_element_value_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::setFieldElementValueCallback));
  set_field_element_value_method.addParameter(field_name_parameter);
  set_field_element_value_method.addParameter(field_element_index_parameter);
  set_field_element_value_method.addParameter(field_value_parameter);

  Method & set_all_field_element_values_method = createMethod(constants::set_all_field_element_values_method_name);
  set_all_field_element_values_method.attachCallback(makeFunctor((Functor0 *)0,*this,&Server::setAllFieldElementValuesCallback));
  set_all_field_element_values_method.addParameter(field_name_parameter);
  set_all_field_element_values_method.addParameter(field_value_parameter);

  server_running_ = false;
}

// Stream
void Server::addServerStream(Stream & stream)
{
  bool stream_found = false;
  for (size_t i=0;i<server_stream_ptrs_.size();++i)
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

// Device Info
void Server::setDeviceName(const ConstantString & device_name)
{
  device_name_ptr_ = &device_name;
}

void Server::setModelNumber(const long model_number)
{
  model_number_ = model_number;
}

void Server::setFirmwareName(const ConstantString & firmware_name)
{
  firmware_name_ptr_ = &firmware_name;
}

void Server::setFirmwareVersion(const long firmware_major, const long firmware_minor, const long firmware_patch)
{
  firmware_major_ = firmware_major;
  firmware_minor_ = firmware_minor;
  firmware_patch_ = firmware_patch;
}

void Server::setHardwareName(const ConstantString & hardware_name)
{
  hardware_name_ptr_ = &hardware_name;
}

void Server::setHardwareVersion(const long hardware_major, const long hardware_minor)
{
  hardware_major_ = hardware_major;
  hardware_minor_ = hardware_minor;
}

// Field
bool Server::setFieldValue(const ConstantString & field_name,
                           ArduinoJson::JsonArray & value)
{
  bool success = false;
  int field_index;
  Field & field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    size_t array_length = field.getArrayLength();
    JsonStream::JsonTypes field_type = field.getType();
    if (field_type == JsonStream::ARRAY_TYPE)
    {
      size_t N = value.size();
      size_t array_length_min = min(array_length,N);
      JsonStream::JsonTypes array_element_type = field.getArrayElementType();
      field.preSetValueCallback();
      switch (array_element_type)
      {
        case JsonStream::LONG_TYPE:
        {
          for (size_t i=0; i<array_length_min; ++i)
          {
            long v = value[i];
            success = setFieldElementValue(field_name,i,v);
            if (!success)
            {
              break;
            }
          }
          break;
        }
        case JsonStream::DOUBLE_TYPE:
        {
          for (size_t i=0; i<array_length_min; ++i)
          {
            double v = value[i];
            success = setFieldElementValue(field_name,i,v);
            if (!success)
            {
              break;
            }
          }
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          for (size_t i=0;i<array_length_min;++i)
          {
            bool v = value[i];
            success = setFieldElementValue(field_name,i,v);
            if (!success)
            {
              break;
            }
          }
          break;
        }
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          break;
        }
        case JsonStream::OBJECT_TYPE:
        {
          break;
        }
        case JsonStream::ARRAY_TYPE:
        {
          break;
        }
        case JsonStream::VALUE_TYPE:
        {
          break;
        }
      }
      field.postSetValueCallback();
    }
  }
  return success;
}

size_t Server::getFieldArrayLength(const ConstantString & field_name)
{
  int field_index;
  Field & field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    return field.getArrayLength();
  }
  else
  {
    return 0;
  }

}

size_t Server::getFieldStringLength(const ConstantString & field_name)
{
  int field_index;
  Field & field = findField(field_name,&field_index);
  if (field_index >= 0)
  {
    JsonStream::JsonTypes field_type = field.getType();
    if (field_type == JsonStream::STRING_TYPE)
    {
      size_t array_length_max = field.getArrayLength();
      size_t array_length = 1;
      char value;
      while (array_length < array_length_max)
      {
        field.getElementValue(value,array_length-1);
        if (value == 0)
        {
          return array_length - 1;
        }
        ++array_length;
      }
      return array_length_max;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }

}

void Server::setFieldsToDefaults()
{
  for (size_t i=0; i<internal_fields_.size(); ++i)
  {
    internal_fields_[i].setDefaultValue();
  }
  for (size_t i=0; i<external_fields_.size(); ++i)
  {
    external_fields_[i].setDefaultValue();
  }
}

// Parameter
Parameter & Server::createParameter(const ConstantString & parameter_name)
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

Parameter & Server::copyParameter(Parameter parameter,const ConstantString & parameter_name)
{
  external_parameters_.push_back(parameter);
  external_parameters_.back().setName(parameter_name);
  return external_parameters_.back();
}

ArduinoJson::JsonVariant Server::getParameterValue(const ConstantString & parameter_name)
{
  int parameter_index = findMethodParameterIndex(request_method_index_,parameter_name);
  // index 0 is the method, index 1 is the first parameter
  return (*request_json_array_ptr_)[parameter_index+1];
}

// Method
Method & Server::createMethod(const ConstantString & method_name)
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

Method & Server::copyMethod(Method method,const ConstantString & method_name)
{
  external_methods_.push_back(method);
  external_methods_.back().setName(method_name);
  return external_methods_.back();
}

// Response
void Server::writeNullToResponse()
{
  json_stream_.writeNull();
}

void Server::writeToResponse(Vector<const constants::SubsetMemberType> & value, JsonStream::JsonTypes type)
{
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      Array<long,constants::SUBSET_ELEMENT_COUNT_MAX> subset_elements_array;
      for (size_t i=0; i<value.size(); ++i)
      {
        subset_elements_array.push_back(value[i].l);
      }
      json_stream_.write(subset_elements_array);
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      Array<ConstantString *,constants::SUBSET_ELEMENT_COUNT_MAX> subset_elements_array;
      for (size_t i=0; i<value.size(); ++i)
      {
        subset_elements_array.push_back(value[i].cs_ptr);
      }
      json_stream_.write(subset_elements_array);
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      break;
    }
    case JsonStream::VALUE_TYPE:
    {
      break;
    }
  }
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

// Server
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
  const char * method_string = (*request_json_array_ptr_)[0];
  request_method_index_ = processMethodString(method_string);
  if (request_method_index_ >= 0)
  {
    int array_elements_count = countJsonArrayElements((*request_json_array_ptr_));
    int parameter_count = array_elements_count - 1;
    char question_str[constants::question_constant_string.length()+1];
    constants::question_constant_string.copy(question_str);
    char question_double_str[constants::question_double_constant_string.length()+1];
    constants::question_double_constant_string.copy(question_double_str);
    // method ?
    if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_str) == 0))
    {
      writeResultKeyToResponse();
      methodHelp(false,request_method_index_);
    }
    // method ??
    else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
    {
      writeResultKeyToResponse();
      methodHelp(true,request_method_index_);
    }
    // method parameter ?
    // method parameter ??
    else if ((parameter_count == 2) &&
             ((strcmp((*request_json_array_ptr_)[2],question_str) == 0) ||
              (strcmp((*request_json_array_ptr_)[2],question_double_str) == 0)))
    {
      int parameter_index = processParameterString((*request_json_array_ptr_)[1]);
      Parameter * parameter_ptr;
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
      if (request_method_index_ <= private_method_index_)
      {
        executeMethod();
      }
      else if (parameter_count != internal_methods_[request_method_index_].getParameterCount())
      {
        error_ = true;
        writeKeyToResponse(constants::error_constant_string);
        beginResponseObject();
        writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
        char incorrect_parameter_number[constants::incorrect_parameter_number_error_data.length()+1];
        constants::incorrect_parameter_number_error_data.copy(incorrect_parameter_number);
        char error_str[constants::STRING_LENGTH_ERROR];
        error_str[0] = '\0';
        strcat(error_str,incorrect_parameter_number);
        char parameter_count_str[constants::STRING_LENGTH_PARAMETER_COUNT];
        dtostrf(parameter_count,0,0,parameter_count_str);
        strcat(error_str,parameter_count_str);
        char given_str[constants::given_constant_string.length()+1];
        constants::given_constant_string.copy(given_str);
        strcat(error_str,given_str);
        dtostrf(internal_methods_[request_method_index_].getParameterCount(),0,0,parameter_count_str);
        strcat(error_str,parameter_count_str);
        char needed_str[constants::needed_constant_string.length()+1];
        constants::needed_constant_string.copy(needed_str);
        strcat(error_str,needed_str);
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
    else if (parameter_count != external_methods_[request_method_index_-internal_methods_.max_size()].getParameterCount())
    {
      error_ = true;
      writeKeyToResponse(constants::error_constant_string);
      beginResponseObject();
      writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
      char incorrect_parameter_number[constants::incorrect_parameter_number_error_data.length()+1];
      constants::incorrect_parameter_number_error_data.copy(incorrect_parameter_number);
      char error_str[constants::STRING_LENGTH_ERROR];
      error_str[0] = '\0';
      strcat(error_str,incorrect_parameter_number);
      char parameter_count_str[constants::STRING_LENGTH_PARAMETER_COUNT];
      dtostrf(parameter_count,0,0,parameter_count_str);
      strcat(error_str,parameter_count_str);
      char given_str[constants::given_constant_string.length()+1];
      constants::given_constant_string.copy(given_str);
      strcat(error_str,given_str);
      dtostrf(external_methods_[request_method_index_-internal_methods_.max_size()].getParameterCount(),0,0,parameter_count_str);
      strcat(error_str,parameter_count_str);
      char needed_str[constants::needed_constant_string.length()+1];
      constants::needed_constant_string.copy(needed_str);
      strcat(error_str,needed_str);
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

int Server::processMethodString(const char * method_string)
{
  int method_index = -1;
  int method_id = atoi(method_string);
  char zero_str[constants::zero_constant_string.length()+1];
  constants::zero_constant_string.copy(zero_str);
  if (strcmp(method_string,zero_str) == 0)
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

int Server::countJsonArrayElements(ArduinoJson::JsonArray & json_array)
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
      internal_methods_[request_method_index_].callback();
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
    const ConstantString & method_name = internal_methods_[method_index].getName();
    writeToResponse(constants::name_constant_string,method_name);
  }
  else
  {
    int index = method_index - internal_methods_.max_size();
    const ConstantString & method_name = external_methods_[index].getName();
    writeToResponse(constants::name_constant_string,method_name);
  }

  writeKeyToResponse(constants::parameters_constant_string);
  json_stream_.beginArray();
  Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
  if (method_index < (int)internal_methods_.max_size())
  {
    parameter_ptrs_ptr = &internal_methods_[method_index].parameter_ptrs_;
  }
  else
  {
    int index = method_index - internal_methods_.max_size();
    parameter_ptrs_ptr = &external_methods_[index].parameter_ptrs_;
  }
  for (size_t i=0; i<parameter_ptrs_ptr->size(); ++i)
  {
    if (verbose)
    {
      parameterHelp(*((*parameter_ptrs_ptr)[i]));
    }
    else
    {
      const ConstantString & parameter_name = (*parameter_ptrs_ptr)[i]->getName();
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

int Server::processParameterString(const char * parameter_string)
{
  int parameter_index = -1;
  int parameter_id = atoi(parameter_string);
  char zero_str[constants::zero_constant_string.length()+1];
  constants::zero_constant_string.copy(zero_str);
  if (strcmp(parameter_string,zero_str) == 0)
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
  Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
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

void Server::parameterHelp(Parameter & parameter, bool end_object)
{
  beginResponseObject();
  const ConstantString & parameter_name = parameter.getName();
  writeToResponse(constants::name_constant_string,parameter_name);

  const ConstantString & units = parameter.getUnits();
  char parameter_units[units.length()+1];
  parameter_units[0] = '\0';
  units.copy(parameter_units);
  char empty_str[] = {0};
  if (strcmp(parameter_units,empty_str) != 0)
  {
    writeToResponse(constants::units_constant_string,parameter_units);
  }
  JsonStream::JsonTypes type = parameter.getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      writeToResponse(constants::type_constant_string,JsonStream::LONG_TYPE);
      if (parameter.subsetIsSet())
      {
        writeKeyToResponse(constants::subset_constant_string);
        writeToResponse(parameter.getSubset(),JsonStream::LONG_TYPE);
      }
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
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      writeToResponse(constants::type_constant_string,JsonStream::STRING_TYPE);
      if (parameter.subsetIsSet())
      {
        writeKeyToResponse(constants::subset_constant_string);
        writeToResponse(parameter.getSubset(),JsonStream::STRING_TYPE);
      }
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
          if (parameter.subsetIsSet())
          {
            writeKeyToResponse(constants::array_element_subset_constant_string);
            writeToResponse(parameter.getSubset(),JsonStream::LONG_TYPE);
          }
          if (parameter.rangeIsSet())
          {
            long min = parameter.getMin().l;
            long max = parameter.getMax().l;
            writeToResponse(constants::array_element_min_constant_string,min);
            writeToResponse(constants::array_element_max_constant_string,max);
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
            writeToResponse(constants::array_element_min_constant_string,min);
            writeToResponse(constants::array_element_max_constant_string,max);
          }
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          writeToResponse(constants::array_element_type_constant_string,JsonStream::BOOL_TYPE);
          break;
        }
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          writeToResponse(constants::array_element_type_constant_string,JsonStream::STRING_TYPE);
          if (parameter.subsetIsSet())
          {
            writeKeyToResponse(constants::array_element_subset_constant_string);
            writeToResponse(parameter.getSubset(),JsonStream::STRING_TYPE);
          }
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
        case JsonStream::VALUE_TYPE:
        {
          break;
        }
      }
      if (parameter.arrayLengthRangeIsSet())
      {
        size_t array_length_min = parameter.getArrayLengthMin();
        size_t array_length_max = parameter.getArrayLengthMax();
        writeToResponse(constants::array_length_min_constant_string,array_length_min);
        writeToResponse(constants::array_length_max_constant_string,array_length_max);
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

void Server::fieldHelp(Field & field)
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
    // do not check method
    if (it!=request_json_array_ptr_->begin())
    {
      Parameter * parameter_ptr = NULL;
      if  (request_method_index_ < (int)internal_methods_.max_size())
      {
        parameter_ptr = internal_methods_[request_method_index_].parameter_ptrs_[parameter_index];
      }
      else
      {
        int index = request_method_index_ - internal_methods_.max_size();
        parameter_ptr = external_methods_[index].parameter_ptrs_[parameter_index];
      }
      if (checkParameter(*parameter_ptr,*it))
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

bool Server::checkParameter(Parameter & parameter, ArduinoJson::JsonVariant & json_value)
{
  bool in_subset = true;
  bool in_range = true;
  bool array_length_in_range = true;
  bool object_parse_unsuccessful = false;
  bool array_parse_unsuccessful = false;
  char min_str[JsonStream::STRING_LENGTH_DOUBLE];
  min_str[0] = '\0';
  char max_str[JsonStream::STRING_LENGTH_DOUBLE];
  max_str[0] = '\0';
  JsonStream::JsonTypes type = parameter.getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      long value = (long)json_value;
      if (!parameter.valueInSubset(value))
      {
        in_subset = false;
        break;
      }
      if (!parameter.valueInRange(value))
      {
        in_range = false;
        long min = parameter.getMin().l;
        long max = parameter.getMax().l;
        dtostrf(min,0,0,min_str);
        dtostrf(max,0,0,max_str);
      }
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      double value = (double)json_value;
      if (!parameter.valueInRange(value))
      {
        in_range = false;
        double min = parameter.getMin().d;
        double max = parameter.getMax().d;
        dtostrf(min,0,JsonStream::DOUBLE_DIGITS_DEFAULT,min_str);
        dtostrf(max,0,JsonStream::DOUBLE_DIGITS_DEFAULT,max_str);
      }
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      const char * value = (const char *)json_value;
      if (!parameter.valueInSubset(value))
      {
        in_subset = false;
      }
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      ArduinoJson::JsonObject & json_object = json_value;
      if (!json_object.success())
      {
        object_parse_unsuccessful = true;
      }
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      ArduinoJson::JsonArray & json_array = json_value;
      if (!json_array.success())
      {
        array_parse_unsuccessful = true;
      }
      else
      {
        size_t array_length = json_array.size();
        if (!parameter.arrayLengthInRange(array_length))
        {
          array_length_in_range = false;
          size_t array_length_min = parameter.getArrayLengthMin();
          size_t array_length_max = parameter.getArrayLengthMax();
          dtostrf(array_length_min,0,0,min_str);
          dtostrf(array_length_max,0,0,max_str);
          break;
        }
        for (ArduinoJson::JsonArray::iterator it=json_array.begin();
             it!=json_array.end();
             ++it)
        {
          bool parameter_ok = checkArrayParameterElement(parameter,*it);
          if (!parameter_ok)
          {
            break;
          }
        }
      }
      break;
    }
    case JsonStream::VALUE_TYPE:
    {
      break;
    }
  }
  if (!in_subset)
  {
    Vector<const constants::SubsetMemberType> & subset = parameter.getSubset();
    writeParameterNotInSubsetErrorToResponse(parameter,subset);
  }
  else if (!in_range)
  {
    writeParameterNotInRangeErrorToResponse(parameter,min_str,max_str);
  }
  else if (!array_length_in_range)
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    constants::array_parameter_length_error_error_data.copy(error_str);
    char value_not_in_range_str[constants::value_not_in_range_error_data.length() + 1];
    constants::value_not_in_range_error_data.copy(value_not_in_range_str);
    strcat(error_str,value_not_in_range_str);
    strcat(error_str,min_str);
    char less_than_equal_str[constants::less_than_equal_constant_string.length()+1];
    constants::less_than_equal_constant_string.copy(less_than_equal_str);
    strcat(error_str,less_than_equal_str);
    const ConstantString & name = parameter.getName();
    char parameter_name[name.length()+1];
    parameter_name[0] = '\0';
    name.copy(parameter_name);
    strcat(error_str,parameter_name);
    char array_length_str[constants::array_length_constant_string.length()+1];
    constants::array_length_constant_string.copy(array_length_str);
    strcat(error_str,array_length_str);
    strcat(error_str,less_than_equal_str);
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
    const ConstantString & name = parameter.getName();
    char parameter_name[name.length()+1];
    parameter_name[0] = '\0';
    name.copy(parameter_name);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
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
    const ConstantString & name = parameter.getName();
    char parameter_name[name.length()+1];
    parameter_name[0] = '\0';
    name.copy(parameter_name);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    strcat(error_str,parameter_name);
    char invalid_json_array[constants::invalid_json_array_error_data.length()+1];
    constants::invalid_json_array_error_data.copy(invalid_json_array);
    strcat(error_str,invalid_json_array);
    writeToResponse(constants::data_constant_string,error_str);
    writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
    endResponseObject();
  }
  bool parameter_ok = in_subset && in_range && array_length_in_range && (!object_parse_unsuccessful) && (!array_parse_unsuccessful);
  return parameter_ok;
}

bool Server::checkArrayParameterElement(Parameter & parameter, ArduinoJson::JsonVariant & json_value)
{
  bool in_subset = true;
  bool in_range = true;
  char min_str[JsonStream::STRING_LENGTH_DOUBLE];
  min_str[0] = '\0';
  char max_str[JsonStream::STRING_LENGTH_DOUBLE];
  max_str[0] = '\0';
  JsonStream::JsonTypes type = parameter.getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      JsonStream::JsonTypes array_element_type = parameter.getArrayElementType();
      switch (array_element_type)
      {
        case JsonStream::LONG_TYPE:
        {
          long value = (long)json_value;
          if (!parameter.valueInSubset(value))
          {
            in_subset = false;
            break;
          }
          if (!parameter.valueInRange(value))
          {
            in_range = false;
            long min = parameter.getMin().l;
            long max = parameter.getMax().l;
            dtostrf(min,0,0,min_str);
            dtostrf(max,0,0,max_str);
          }
          break;
        }
        case JsonStream::DOUBLE_TYPE:
        {
          double value = (double)json_value;
          if (!parameter.valueInRange(value))
          {
            in_range = false;
            double min = parameter.getMin().d;
            double max = parameter.getMax().d;
            dtostrf(min,0,JsonStream::DOUBLE_DIGITS_DEFAULT,min_str);
            dtostrf(max,0,JsonStream::DOUBLE_DIGITS_DEFAULT,max_str);
          }
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          break;
        }
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          const char * value = (const char *)json_value;
          if (!parameter.valueInSubset(value))
          {
            in_subset = false;
          }
          break;
        }
        case JsonStream::OBJECT_TYPE:
        {
          break;
        }
        case JsonStream::ARRAY_TYPE:
        {
          break;
        }
        case JsonStream::VALUE_TYPE:
        {
          break;
        }
      }
      break;
    }
    case JsonStream::VALUE_TYPE:
    {
      break;
    }
  }
  if (!in_subset)
  {
    Vector<const constants::SubsetMemberType> & subset = parameter.getSubset();
    writeParameterNotInSubsetErrorToResponse(parameter,subset);
  }
  else if (!in_range)
  {
    writeParameterNotInRangeErrorToResponse(parameter,min_str,max_str);
  }
  bool parameter_ok = in_subset && in_range;
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
  if (!eeprom_initialized_sv_.valueIsDefault())
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
      for (size_t method_index=0; method_index<internal_methods_.size(); ++method_index)
      {
        if (method_index > private_method_index_)
        {
          const ConstantString & method_name = internal_methods_[method_index].getName();
          writeToResponse(method_name);
        }
      }
      for (size_t method_index=0; method_index<external_methods_.size(); ++method_index)
      {
        const ConstantString & method_name = external_methods_[method_index].getName();
        writeToResponse(method_name);
      }
      endResponseArray();

      writeKeyToResponse(constants::parameters_constant_string);
      beginResponseArray();
      for (size_t parameter_index=0; parameter_index<internal_parameters_.size(); ++parameter_index)
      {
        const ConstantString & parameter_name = internal_parameters_[parameter_index].getName();
        writeToResponse(parameter_name);
      }
      for (size_t parameter_index=0; parameter_index<external_parameters_.size(); ++parameter_index)
      {
        const ConstantString & parameter_name = external_parameters_[parameter_index].getName();
        writeToResponse(parameter_name);
      }
      endResponseArray();

      writeKeyToResponse(constants::fields_constant_string);
      beginResponseArray();
      for (size_t field_index=0; field_index<internal_fields_.size(); ++field_index)
      {
        const ConstantString & field_name = internal_fields_[field_index].getName();
        writeToResponse(field_name);
      }
      for (size_t field_index=0; field_index<external_fields_.size(); ++field_index)
      {
        const ConstantString & field_name = external_fields_[field_index].getName();
        writeToResponse(field_name);
      }
      endResponseArray();
    }
    // ??
    else
    {
      writeKeyToResponse(constants::methods_constant_string);
      beginResponseArray();
      for (size_t method_index=0; method_index<internal_methods_.size(); ++method_index)
      {
        if (method_index > private_method_index_)
        {
          methodHelp(false,method_index);
        }
      }
      for (size_t method_index=0; method_index<external_methods_.size(); ++method_index)
      {
        int index = method_index + internal_methods_.max_size();
        methodHelp(false,index);
      }
      endResponseArray();

      writeKeyToResponse(constants::parameters_constant_string);
      beginResponseArray();
      for (size_t parameter_index=0; parameter_index<internal_parameters_.size(); ++parameter_index)
      {
        parameterHelp(internal_parameters_[parameter_index]);
      }
      for (size_t parameter_index=0; parameter_index<external_parameters_.size(); ++parameter_index)
      {
        parameterHelp(external_parameters_[parameter_index]);
      }
      endResponseArray();

      writeKeyToResponse(constants::fields_constant_string);
      beginResponseArray();
      for (size_t field_index=0; field_index<internal_fields_.size(); ++field_index)
      {
        fieldHelp(internal_fields_[field_index]);
      }
      for (size_t field_index=0; field_index<external_fields_.size(); ++field_index)
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
    const char * param_string = (*request_json_array_ptr_)[1];
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
        Parameter * parameter_ptr = NULL;
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
          Field * field_ptr = NULL;
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
    const char * method_string = (*request_json_array_ptr_)[1];
    int method_index = findMethodIndex(method_string);
    if (method_index >= 0)
    {
      int parameter_index = findMethodParameterIndex(method_index,(const char *)(*request_json_array_ptr_)[2]);
      if (parameter_index >= 0)
      {
        param_error = false;
        Parameter * parameter_ptr = NULL;
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

void Server::writeDeviceInfoToResponse()
{
  beginResponseObject();
  writeToResponse(constants::device_name_constant_string,device_name_ptr_);
  writeToResponse(constants::model_number_constant_string,model_number_);
  writeToResponse(constants::serial_number_field_name,getSerialNumber());
  writeToResponse(constants::firmware_name_constant_string,firmware_name_ptr_);
  writeKeyToResponse(constants::firmware_version_constant_string);
  beginResponseObject();
  writeToResponse(constants::major_constant_string,firmware_major_);
  writeToResponse(constants::minor_constant_string,firmware_minor_);
  writeToResponse(constants::patch_constant_string,firmware_patch_);
  endResponseObject();
  writeToResponse(constants::hardware_name_constant_string,hardware_name_ptr_);
  writeKeyToResponse(constants::hardware_version_constant_string);
  beginResponseObject();
  writeToResponse(constants::major_constant_string,hardware_major_);
  writeToResponse(constants::minor_constant_string,hardware_minor_);
  endResponseObject();
  writeToResponse(constants::processor_constant_string,constants::processor_name_constant_string);
  endResponseObject();
}

void Server::writeFieldToResponse(Field & field, bool write_key, bool write_default, int element_index)
{
  const ConstantString & field_name = field.getName();
  if (write_key)
  {
    writeKeyToResponse(field_name);
  }
  JsonStream::JsonTypes field_type = field.getType();
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
    case JsonStream::DOUBLE_TYPE:
    {
      if (element_index >= 0)
      {
        writeFieldErrorToResponse(constants::field_not_array_type_error_data);
        return;
      }
      double field_value;
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
    case JsonStream::STRING_TYPE:
    {
      size_t array_length = getFieldArrayLength(field_name);
      if (element_index >= 0)
      {
        if (element_index >= ((int)array_length-1))
        {
          writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
          return;
        }
        size_t array_length = 2;
        char char_array[array_length];
        char field_element_value;
        bool success = getFieldElementValue(field_name,element_index,field_element_value);
        if (success)
        {
          char_array[0] = field_element_value;
          char_array[1] = '\0';
        }
        else
        {
          char_array[0] = '\0';
        }
        writeToResponse(char_array);
        return;
      }
      char char_array[array_length];
      if (write_default)
      {
        getFieldDefaultValue(field_name,char_array,array_length);
      }
      else
      {
        getFieldValue(field_name,char_array,array_length);
      }
      writeToResponse(char_array);
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      const JsonStream::JsonTypes array_element_type = field.getArrayElementType();
      size_t array_length = field.getArrayLength();
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
              getFieldDefaultValue(field_name,field_value,array_length);
            }
            else
            {
              getFieldValue(field_name,field_value,array_length);
            }
            writeArrayToResponse(field_value,array_length);
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
        case JsonStream::DOUBLE_TYPE:
        {
          if (element_index < 0)
          {
            double field_value[array_length];
            if (write_default)
            {
              getFieldDefaultValue(field_name,field_value,array_length);
            }
            else
            {
              getFieldValue(field_name,field_value,array_length);
            }
            writeArrayToResponse(field_value,array_length);
          }
          else
          {
            double field_value;
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
              getFieldDefaultValue(field_name,field_value,array_length);
            }
            else
            {
              getFieldValue(field_name,field_value,array_length);
            }
            writeArrayToResponse(field_value,array_length);
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
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          break;
        }
        case JsonStream::OBJECT_TYPE:
        {
          break;
        }
        case JsonStream::ARRAY_TYPE:
        {
          break;
        }
        case JsonStream::VALUE_TYPE:
        {
          break;
        }
      }
      break;
    }
    case JsonStream::VALUE_TYPE:
    {
      break;
    }
  }
}

void Server::writeParameterNotInSubsetErrorToResponse(Parameter & parameter, Vector<const constants::SubsetMemberType> & subset)
{
  error_ = true;
  writeKeyToResponse(constants::error_constant_string);
  beginResponseObject();
  writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
  char error_str[constants::STRING_LENGTH_ERROR];
  error_str[0] = '\0';
  JsonStream::JsonTypes type = parameter.getType();
  if (type != JsonStream::ARRAY_TYPE)
  {
    constants::parameter_error_error_data.copy(error_str);
  }
  else
  {
    constants::array_parameter_error_error_data.copy(error_str);
    type = parameter.getArrayElementType();
  }
  char value_not_in_subset_str[constants::value_not_in_subset_error_data.length() + 1];
  constants::value_not_in_subset_error_data.copy(value_not_in_subset_str);
  strcat(error_str,value_not_in_subset_str);
  size_t length_left = constants::STRING_LENGTH_ERROR - strlen(error_str) - 1;
  subsetToString(error_str,subset,type,length_left);
  writeToResponse(constants::data_constant_string,error_str);
  writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
  endResponseObject();
}

void Server::writeParameterNotInRangeErrorToResponse(Parameter & parameter, char min_str[], char max_str[])
{
  error_ = true;
  writeKeyToResponse(constants::error_constant_string);
  beginResponseObject();
  writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
  char error_str[constants::STRING_LENGTH_ERROR];
  error_str[0] = '\0';
  JsonStream::JsonTypes type = parameter.getType();
  if (type != JsonStream::ARRAY_TYPE)
  {
    constants::parameter_error_error_data.copy(error_str);
  }
  else
  {
    constants::array_parameter_error_error_data.copy(error_str);
  }
  char value_not_in_range_str[constants::value_not_in_range_error_data.length() + 1];
  constants::value_not_in_range_error_data.copy(value_not_in_range_str);
  strcat(error_str,value_not_in_range_str);
  strcat(error_str,min_str);
  char less_than_equal_str[constants::less_than_equal_constant_string.length()+1];
  constants::less_than_equal_constant_string.copy(less_than_equal_str);
  strcat(error_str,less_than_equal_str);
  const ConstantString & name = parameter.getName();
  char parameter_name[name.length()+1];
  parameter_name[0] = '\0';
  name.copy(parameter_name);
  strcat(error_str,parameter_name);
  if (type == JsonStream::ARRAY_TYPE)
  {
    char element_str[constants::element_constant_string.length()+1];
    constants::element_constant_string.copy(element_str);
    strcat(error_str,element_str);
  }
  strcat(error_str,less_than_equal_str);
  strcat(error_str,max_str);
  writeToResponse(constants::data_constant_string,error_str);
  writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
  endResponseObject();
}

void Server::writeFieldErrorToResponse(const ConstantString & error)
{
  error_ = true;
  writeKeyToResponse(constants::error_constant_string);
  beginResponseObject();
  writeToResponse(constants::message_constant_string,constants::invalid_params_error_message);
  writeToResponse(constants::data_constant_string,error);
  writeToResponse(constants::code_constant_string,constants::invalid_params_error_code);
  endResponseObject();
}

void Server::subsetToString(char * destination, Vector<const constants::SubsetMemberType> & subset, const JsonStream::JsonTypes type, const size_t num)
{
  size_t length_left = num;
  size_t length = constants::array_open_constant_string.length();
  if (length_left < length)
  {
    return;
  }
  char array_open_str[length + 1];
  constants::array_open_constant_string.copy(array_open_str);
  strcat(destination,array_open_str);
  length_left -= length;

  char value_str[constants::STRING_LENGTH_SUBSET_ELEMENT];
  for (size_t i=0; i<subset.size(); ++i)
  {
    if (i != 0)
    {
      length = constants::array_separator_constant_string.length();
      if (length_left < length)
      {
        return;
      }
      char array_separator_str[length + 1];
      constants::array_separator_constant_string.copy(array_separator_str);
      strcat(destination,array_separator_str);
      length_left -= length;
    }
    value_str[0] = '\0';
    switch (type)
    {
      case JsonStream::LONG_TYPE:
      {
        ltoa(subset[i].l,value_str,10);
        break;
      }
      case JsonStream::DOUBLE_TYPE:
      {
        break;
      }
      case JsonStream::BOOL_TYPE:
      {
        break;
      }
      case JsonStream::NULL_TYPE:
      {
        break;
      }
      case JsonStream::STRING_TYPE:
      {
        char cs_str[subset[i].cs_ptr->length() + 1];
        subset[i].cs_ptr->copy(cs_str);
        strncat(value_str,cs_str,constants::STRING_LENGTH_SUBSET_ELEMENT - 2);
        break;
      }
      case JsonStream::OBJECT_TYPE:
      {
        break;
      }
      case JsonStream::ARRAY_TYPE:
      {
        break;
      }
      case JsonStream::VALUE_TYPE:
      {
        break;
      }
    }
    length = strlen(value_str);
    if (length_left < length)
    {
      return;
    }
    strcat(destination,value_str);
    length_left -= length;
  }

  length = constants::array_close_constant_string.length();
  if (length_left < length)
  {
    return;
  }
  char array_close_str[length + 1];
  constants::array_close_constant_string.copy(array_close_str);
  strcat(destination,array_close_str);
}

// internal methods
void Server::getDeviceInfoCallback()
{
  writeResultKeyToResponse();
  writeDeviceInfoToResponse();
}

void Server::getMethodIdsCallback()
{
  writeResultKeyToResponse();
  beginResponseObject();
  for (size_t method_index=0; method_index<internal_methods_.size(); ++method_index)
  {
    if (method_index > private_method_index_)
    {
      const ConstantString & method_name = internal_methods_[method_index].getName();
      writeToResponse(method_name,method_index);
    }
  }
  for (size_t method_index=0; method_index<external_methods_.size(); ++method_index)
  {
    const ConstantString & method_name = external_methods_[method_index].getName();
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
  for (size_t parameter_index=0; parameter_index<internal_parameters_.size(); ++parameter_index)
  {
    parameterHelp(internal_parameters_[parameter_index]);
  }
  for (size_t parameter_index=0; parameter_index<external_parameters_.size(); ++parameter_index)
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
  for (size_t i=0; i<internal_fields_.size(); ++i)
  {
    Field & field = internal_fields_[i];
    writeFieldToResponse(field,true,true);
  }
  for (size_t i=0; i<external_fields_.size(); ++i)
  {
    Field & field = external_fields_[i];
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
  const char * field_name = getParameterValue(constants::field_name_parameter_name);
  int field_index;
  Field & field = findField(field_name,&field_index);
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
  for (size_t i=0; i<internal_fields_.size(); ++i)
  {
    Field & field = internal_fields_[i];
    writeFieldToResponse(field,true,false);
  }
  for (size_t i=0; i<external_fields_.size(); ++i)
  {
    Field & field = external_fields_[i];
    writeFieldToResponse(field,true,false);
  }
  endResponseObject();
}

void Server::getFieldValueCallback()
{
  writeResultKeyToResponse();
  const char * field_name = getParameterValue(constants::field_name_parameter_name);
  int field_index;
  Field & field = findField(field_name,&field_index);
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
  const char * field_name = getParameterValue(constants::field_name_parameter_name);
  long field_element_index = getParameterValue(constants::field_element_index_parameter_name);
  if (field_element_index < 0)
  {
    writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
    return;
  }
  int field_index;
  Field & field = findField(field_name,&field_index);
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
  const char * field_name = getParameterValue(constants::field_name_parameter_name);
  int field_index;
  Field & field = findField(field_name,&field_index);
  const ConstantString & field_name_cs = field.getName();
  if (field_index >= 0)
  {
    ArduinoJson::JsonVariant json_value = getParameterValue(constants::field_value_parameter_name);
    bool parameter_ok = checkParameter(field.getParameter(),json_value);
    if (!parameter_ok)
    {
      return;
    }
    JsonStream::JsonTypes field_type = field.getType();
    switch (field_type)
    {
      case JsonStream::LONG_TYPE:
      {
        long field_value = getParameterValue(constants::field_value_parameter_name);
        setFieldValue(field_name_cs,field_value);
        break;
      }
      case JsonStream::DOUBLE_TYPE:
      {
        double field_value = getParameterValue(constants::field_value_parameter_name);
        setFieldValue(field_name_cs,field_value);
        break;
      }
      case JsonStream::BOOL_TYPE:
      {
        bool field_value = getParameterValue(constants::field_value_parameter_name);
        setFieldValue(field_name_cs,field_value);
        break;
      }
      case JsonStream::NULL_TYPE:
      {
        break;
      }
      case JsonStream::STRING_TYPE:
      {
        const char * field_value = getParameterValue(constants::field_value_parameter_name);
        size_t array_length = strlen(field_value) + 1;
        setFieldValue(field_name_cs,field_value,array_length);
        break;
      }
      case JsonStream::OBJECT_TYPE:
      {
        break;
      }
      case JsonStream::ARRAY_TYPE:
      {
        ArduinoJson::JsonArray & field_value = getParameterValue(constants::field_value_parameter_name);
        setFieldValue(field_name_cs,field_value);
        break;
      }
      case JsonStream::VALUE_TYPE:
      {
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
  const char * field_name = getParameterValue(constants::field_name_parameter_name);
  long field_element_index = getParameterValue(constants::field_element_index_parameter_name);
  if (field_element_index < 0)
  {
    writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
    return;
  }
  int field_index;
  Field & field = findField(field_name,&field_index);
  const ConstantString & field_name_cs = field.getName();
  if (field_index >= 0)
  {
    ArduinoJson::JsonVariant json_value = getParameterValue(constants::field_value_parameter_name);
    bool parameter_ok = checkArrayParameterElement(field.getParameter(),json_value);
    if (!parameter_ok)
    {
      return;
    }
    JsonStream::JsonTypes field_type = field.getType();
    switch (field_type)
    {
      case JsonStream::LONG_TYPE:
      {
        writeFieldErrorToResponse(constants::field_not_array_type_error_data);
        break;
      }
      case JsonStream::DOUBLE_TYPE:
      {
        writeFieldErrorToResponse(constants::field_not_array_type_error_data);
        break;
      }
      case JsonStream::BOOL_TYPE:
      {
        writeFieldErrorToResponse(constants::field_not_array_type_error_data);
        break;
      }
      case JsonStream::NULL_TYPE:
      {
        break;
      }
      case JsonStream::STRING_TYPE:
      {
        if (!field.stringIsSavedAsCharArray())
        {
          writeFieldErrorToResponse(constants::cannot_set_element_in_string_field_with_subset_error_data);
          break;
        }
        size_t array_length = field.getArrayLength();
        if ((size_t)field_element_index >= (array_length - 1))
        {
          writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
          return;
        }
        const char * field_value = getParameterValue(constants::field_value_parameter_name);
        size_t string_length = strlen(field_value);
        if (string_length >= 1)
        {
          char v = field_value[0];
          setFieldElementValue(field_name_cs,field_element_index,v);
        }
        break;
      }
      case JsonStream::OBJECT_TYPE:
      {
        break;
      }
      case JsonStream::ARRAY_TYPE:
      {
        size_t array_length = field.getArrayLength();
        if ((size_t)field_element_index >= array_length)
        {
          writeFieldErrorToResponse(constants::field_element_index_out_of_bounds_error_data);
          return;
        }
        JsonStream::JsonTypes array_element_type = field.getArrayElementType();
        switch (array_element_type)
        {
          case JsonStream::LONG_TYPE:
          {
            long field_value = getParameterValue(constants::field_value_parameter_name);
            setFieldElementValue(field_name_cs,field_element_index,field_value);
            break;
          }
          case JsonStream::DOUBLE_TYPE:
          {
            double field_value = getParameterValue(constants::field_value_parameter_name);
            setFieldElementValue(field_name_cs,field_element_index,field_value);
            break;
          }
          case JsonStream::BOOL_TYPE:
          {
            bool field_value = getParameterValue(constants::field_value_parameter_name);
            setFieldElementValue(field_name_cs,field_element_index,field_value);
            break;
          }
          case JsonStream::NULL_TYPE:
          {
            break;
          }
          case JsonStream::STRING_TYPE:
          {
            break;
          }
          case JsonStream::OBJECT_TYPE:
          {
            break;
          }
          case JsonStream::ARRAY_TYPE:
          {
            break;
          }
          case JsonStream::VALUE_TYPE:
          {
            break;
          }
        }
        break;
      }
      case JsonStream::VALUE_TYPE:
      {
        break;
      }
    }
  }
  else
  {
    writeFieldErrorToResponse(constants::field_not_found_error_data);
  }
}

void Server::setAllFieldElementValuesCallback()
{
  const char * field_name = getParameterValue(constants::field_name_parameter_name);
  int field_index;
  Field & field = findField(field_name,&field_index);
  ArduinoJson::JsonVariant json_value = getParameterValue(constants::field_value_parameter_name);
  const ConstantString & field_name_cs = field.getName();
  if (field_index >= 0)
  {
    bool parameter_ok = checkArrayParameterElement(field.getParameter(),json_value);
    if (!parameter_ok)
    {
      return;
    }
    JsonStream::JsonTypes field_type = field.getType();
    switch (field_type)
    {
      case JsonStream::LONG_TYPE:
      {
        break;
      }
      case JsonStream::DOUBLE_TYPE:
      {
        break;
      }
      case JsonStream::BOOL_TYPE:
      {
        break;
      }
      case JsonStream::NULL_TYPE:
      {
        break;
      }
      case JsonStream::STRING_TYPE:
      {
        if (!field.stringIsSavedAsCharArray())
        {
          writeFieldErrorToResponse(constants::cannot_set_element_in_string_field_with_subset_error_data);
          break;
        }
        const char * field_value = getParameterValue(constants::field_value_parameter_name);
        size_t string_length = strlen(field_value);
        if (string_length >= 1)
        {
          char v = field_value[0];
          setAllFieldElementValues(field_name_cs,v);
        }
        break;
      }
      case JsonStream::OBJECT_TYPE:
      {
        break;
      }
      case JsonStream::ARRAY_TYPE:
      {
        JsonStream::JsonTypes array_element_type = field.getArrayElementType();
        switch (array_element_type)
        {
          case JsonStream::LONG_TYPE:
          {
            long field_value = getParameterValue(constants::field_value_parameter_name);
            setAllFieldElementValues(field_name_cs,field_value);
            break;
          }
          case JsonStream::DOUBLE_TYPE:
          {
            double field_value = getParameterValue(constants::field_value_parameter_name);
            setAllFieldElementValues(field_name_cs,field_value);
            break;
          }
          case JsonStream::BOOL_TYPE:
          {
            bool field_value = getParameterValue(constants::field_value_parameter_name);
            setAllFieldElementValues(field_name_cs,field_value);
            break;
          }
          case JsonStream::NULL_TYPE:
          {
            break;
          }
          case JsonStream::STRING_TYPE:
          {
            break;
          }
          case JsonStream::OBJECT_TYPE:
          {
            break;
          }
          case JsonStream::ARRAY_TYPE:
          {
            break;
          }
          case JsonStream::VALUE_TYPE:
          {
            break;
          }
        }
        break;
      }
      case JsonStream::VALUE_TYPE:
      {
        break;
      }
    }
  }
  else
  {
    writeFieldErrorToResponse(constants::field_not_found_error_data);
  }
}

}
