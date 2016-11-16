// ----------------------------------------------------------------------------
// Server.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Server.h"


namespace modular_server
{
// public
Server::Server() :
  eeprom_initialized_sv_(constants::eeprom_initialized_default_value)
{
}

void Server::setup()
{
  request_procedure_index_ = -1;
  parameter_count_ = 0;
  server_stream_index_ = 0;

  eeprom_initialized_ = false;

  constants::SubsetMemberType firmware_name;
  firmware_name.cs_ptr = &constants::all_constant_string;
  firmware_name_array_.push_back(firmware_name);

  // Streams
  response_.setJsonStream(json_stream_);

  // Device ID
  setDeviceName(constants::empty_constant_string);
  setFormFactor(constants::empty_constant_string);

  // Hardware Info

  // Firmware
  addFirmware(constants::firmware_info,
              server_properties_,
              server_parameters_,
              server_methods_,
              server_callbacks_);

  // Properties
  Property & serial_number_property = createProperty(constants::serial_number_property_name,constants::serial_number_default);
  serial_number_property.setRange(constants::serial_number_min,constants::serial_number_max);

  // Parameters
  Parameter::get_value_functor_ = makeFunctor((Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> *)0,*this,&Server::getParameterValue);

  Parameter & firmware_parameter = createParameter(constants::firmware_constant_string);
  firmware_parameter.setTypeString();
  firmware_parameter.setArrayLengthRange(1,constants::FIRMWARE_COUNT_MAX);
  firmware_parameter.setSubset(firmware_name_array_.data(),
                               firmware_name_array_.max_size(),
                               firmware_name_array_.size());

  // Methods
  Method & get_procedure_ids_method = createMethod(constants::get_procedure_ids_method_name);
  get_procedure_ids_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getProcedureIdsHandler));
  get_procedure_ids_method.setReturnTypeObject();
  private_method_index_ = 0;

  Method & help_method = createMethod(constants::help_method_name);
  help_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::helpHandler));
  help_method.setReturnTypeObject();
  private_method_index_++;

  Method & verbose_help_method = createMethod(constants::verbose_help_method_name);
  verbose_help_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::verboseHelpHandler));
  verbose_help_method.setReturnTypeObject();
  private_method_index_++;

  Method & get_device_id_method = createMethod(constants::get_device_id_method_name);
  get_device_id_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getDeviceIdHandler));
  get_device_id_method.setReturnTypeObject();

  Method & get_device_info_method = createMethod(constants::get_device_info_method_name);
  get_device_info_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getDeviceInfoHandler));
  get_device_info_method.setReturnTypeObject();

  Method & get_api_method = createMethod(constants::get_api_method_name);
  get_api_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getApiHandler));
  get_api_method.addParameter(firmware_parameter);
  get_api_method.setReturnTypeObject();

  Method & get_api_verbose_method = createMethod(constants::get_api_verbose_method_name);
  get_api_verbose_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getApiVerboseHandler));
  get_api_verbose_method.addParameter(firmware_parameter);
  get_api_verbose_method.setReturnTypeObject();

  Method & get_property_default_values_method = createMethod(constants::get_property_default_values_method_name);
  get_property_default_values_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getPropertyDefaultValuesHandler));
  get_property_default_values_method.setReturnTypeObject();

  Method & get_property_values_method = createMethod(constants::get_property_values_method_name);
  get_property_values_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getPropertyValuesHandler));
  get_property_values_method.setReturnTypeObject();

#ifdef __AVR__
  Method & get_memory_free_method = createMethod(constants::get_memory_free_method_name);
  get_memory_free_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getMemoryFreeHandler));
  get_memory_free_method.setReturnTypeLong();
#endif

  // Callbacks
  Callback & set_properties_to_defaults_callback = createCallback(constants::set_properties_to_defaults_callback_name);
  set_properties_to_defaults_callback.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::setPropertiesToDefaultsHandler));

  // Property Parameters
  Parameter & property_value_parameter = Property::createParameter(property::value_parameter_name);

  // Property Array Parameters
  Parameter & property_element_index_parameter = Property::createArrayParameter(property::element_index_parameter_name);

  // Property Methods
  Method & property_get_value_method = Property::createMethod(property::get_value_method_name);

  Method & property_set_value_method = Property::createMethod(property::set_value_method_name);
  property_set_value_method.addParameter(property_value_parameter);

  Method & property_get_default_value_method = Property::createMethod(property::get_default_value_method_name);

  Method & property_set_value_to_default_method = Property::createMethod(property::set_value_to_default_method_name);

  // Property Array Methods
  Method & property_get_element_value_method = Property::createArrayMethod(property::get_element_value_method_name);
  property_get_element_value_method.addParameter(property_element_index_parameter);

  Method & property_set_element_value_method = Property::createArrayMethod(property::set_element_value_method_name);
  property_set_element_value_method.addParameter(property_element_index_parameter);
  property_set_element_value_method.addParameter(property_value_parameter);

  Method & property_get_default_element_value_method = Property::createArrayMethod(property::get_default_element_value_method_name);
  property_get_default_element_value_method.addParameter(property_element_index_parameter);

  Method & property_set_element_value_to_default_method = Property::createArrayMethod(property::set_element_value_to_default_method_name);
  property_set_element_value_to_default_method.addParameter(property_element_index_parameter);

  Method & property_set_all_element_values_method = Property::createArrayMethod(property::set_all_element_values_method_name);
  property_set_all_element_values_method.addParameter(property_value_parameter);

  server_running_ = false;
}

// Streams
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
}

// Device ID
void Server::setDeviceName(const ConstantString & device_name)
{
  device_name_ptr_ = &device_name;
}

void Server::setFormFactor(const ConstantString & form_factor)
{
  form_factor_ptr_ = &form_factor;
}

// Hardware Info
void Server::addHardwareInfo(const constants::HardwareInfo & hardware_info)
{
  hardware_info_array_.push_back(&hardware_info);
}

// Firmware

// Properties
Property & Server::property(const ConstantString & property_name)
{
  int property_index = findPropertyIndex(property_name);
  if ((property_index >= 0) && (property_index < (int)properties_.size()))
  {
    return properties_[property_index];
  }
  return dummy_property_;
}

void Server::setPropertiesToDefaults()
{
  for (size_t i=0; i<properties_.size(); ++i)
  {
    properties_[i].setValueToDefault();
  }
}

// Parameters
Parameter & Server::createParameter(const ConstantString & parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if (parameter_index < 0)
  {
    parameters_.push_back(Parameter(parameter_name));
    const ConstantString * firmware_name_ptr = firmware_info_array_.back()->name_ptr;
    parameters_.back().setFirmwareName(*firmware_name_ptr);
    return parameters_.back();
  }
}

Parameter & Server::parameter(const ConstantString & parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if ((parameter_index >= 0) && (parameter_index < (int)parameters_.size()))
  {
    return parameters_[parameter_index];
  }
  return dummy_parameter_;
}

Parameter & Server::copyParameter(Parameter parameter,const ConstantString & parameter_name)
{
  parameters_.push_back(parameter);
  parameters_.back().setName(parameter_name);
  return parameters_.back();
}

// Methods
Method & Server::createMethod(const ConstantString & method_name)
{
  int method_index = findMethodIndex(method_name);
  if (method_index < 0)
  {
    methods_.push_back(Method(method_name));
    const ConstantString * firmware_name_ptr = firmware_info_array_.back()->name_ptr;
    methods_.back().setFirmwareName(*firmware_name_ptr);
    return methods_.back();
  }
}

Method & Server::method(const ConstantString & method_name)
{
  int method_index = findMethodIndex(method_name);
  if ((method_index >= 0) && (method_index < (int)methods_.size()))
  {
    return methods_[method_index];
  }
  return dummy_method_;
}

Method & Server::copyMethod(Method method,const ConstantString & method_name)
{
  methods_.push_back(method);
  methods_.back().setName(method_name);
  return methods_.back();
}

// Callbacks
Callback & Server::createCallback(const ConstantString & callback_name)
{
  int callback_index = findCallbackIndex(callback_name);
  if (callback_index < 0)
  {
    callbacks_.push_back(Callback(callback_name));
    const ConstantString * firmware_name_ptr = firmware_info_array_.back()->name_ptr;
    callbacks_.back().setFirmwareName(*firmware_name_ptr);
    return callbacks_.back();
  }
}

Callback & Server::callback(const ConstantString & callback_name)
{
  int callback_index = findCallbackIndex(callback_name);
  if ((callback_index >= 0) && (callback_index < (int)callbacks_.size()))
  {
    return callbacks_[callback_index];
  }
  return dummy_callback_;
}

// Response
Response & Server::response()
{
  return response_;
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
      JsonSanitizer<constants::JSON_TOKEN_MAX> sanitizer;
      if (sanitizer.firstCharIsValidJson(request_))
      {
        response_.setCompactPrint();
      }
      else
      {
        response_.setPrettyPrint();
      }
      response_.begin();
      sanitizer.sanitizeBuffer(request_);
      StaticJsonBuffer<constants::STRING_LENGTH_REQUEST> json_buffer;
      if (sanitizer.firstCharIsValidJsonObject(request_))
      {
        response_.returnError(constants::object_request_error_data);
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
          response_.returnRequestParseError(request_);
        }
      }
      response_.end();
    }
    else if (bytes_read < 0)
    {
      response_.setCompactPrint();
      response_.begin();
      response_.returnError(constants::request_length_error_data);
      response_.end();
    }
  }
  incrementServerStream();
}

// private
ArduinoJson::JsonVariant Server::getParameterValue(const ConstantString & parameter_name)
{
  int parameter_index = findMethodParameterIndex(methods_[request_procedure_index_],parameter_name);
  // index 0 is the method, index 1 is the first parameter
  return (*request_json_array_ptr_)[parameter_index+1];
}

void Server::processRequestArray()
{
  const char * procedure_string = (*request_json_array_ptr_)[0];
  request_procedure_index_ = findRequestProcedureIndex(procedure_string);
  if (request_procedure_index_ >= 0)
  {
    int array_elements_count = countJsonArrayElements((*request_json_array_ptr_));
    int parameter_count = array_elements_count - 1;
    char question_str[constants::question_constant_string.length()+1];
    constants::question_constant_string.copy(question_str);
    char question_double_str[constants::question_double_constant_string.length()+1];
    constants::question_double_constant_string.copy(question_double_str);
    if (request_procedure_index_ < methods_.size())
    {
      // method ?
      if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_str) == 0))
      {
        response_.writeResultKey();
        methodHelp(methods_[request_procedure_index_],false);
      }
      // method ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        methodHelp(methods_[request_procedure_index_],true);
      }
      // method parameter ?
      // method parameter ??
      else if ((parameter_count == 2) &&
               ((strcmp((*request_json_array_ptr_)[2],question_str) == 0) ||
                (strcmp((*request_json_array_ptr_)[2],question_double_str) == 0)))
      {
        int parameter_index = processParameterString((*request_json_array_ptr_)[1]);
        Parameter * parameter_ptr;
        parameter_ptr = methods_[request_procedure_index_].parameter_ptrs_[parameter_index];
        response_.writeResultKey();
        parameterHelp(*parameter_ptr);
      }
      // execute private method without checking parameters
      else if (request_procedure_index_ <= private_method_index_)
      {
        methods_[request_procedure_index_].functor();
      }
      else if (parameter_count != methods_[request_procedure_index_].getParameterCount())
      {
        response_.returnParameterCountError(parameter_count,methods_[request_procedure_index_].getParameterCount());
      }
      else
      {
        bool parameters_ok = checkParameters();
        if (parameters_ok)
        {
          methods_[request_procedure_index_].functor();
        }
      }
    }
    else if (request_procedure_index_ < (methods_.size() + callbacks_.size()))
    {
      int callback_index = request_procedure_index_ - methods_.size();
      // callback ?
      if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_str) == 0))
      {
        response_.writeResultKey();
        callbackHelp(callbacks_[callback_index],false);
      }
      // callback ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        callbackHelp(callbacks_[callback_index],true);
      }
      else if (parameter_count != 0)
      {
        response_.returnParameterCountError(parameter_count,0);
      }
      else
      {
        callbacks_[callback_index].functor();
      }
    }
  }
  else
  {
    response_.returnProcedureNotFoundError();
  }
}

int Server::findRequestProcedureIndex(const char * procedure_string)
{
  int procedure_index = -1;
  int procedure_id = atoi(procedure_string);
  char zero_str[constants::zero_constant_string.length()+1];
  constants::zero_constant_string.copy(zero_str);
  if (strcmp(procedure_string,zero_str) == 0)
  {
    procedure_index = 0;
    response_.write(constants::id_constant_string,0);
  }
  else if (procedure_id > 0)
  {
    procedure_index = procedure_id;
    response_.write(constants::id_constant_string,procedure_id);
  }
  else
  {
    procedure_index = findMethodIndex(procedure_string);
    if (procedure_index >= 0)
    {
      response_.write(constants::id_constant_string,procedure_string);
      return procedure_index;
    }
    procedure_index = findCallbackIndex(procedure_string);
    if (procedure_index >= 0)
    {
      response_.write(constants::id_constant_string,procedure_string);
      procedure_index += methods_.size();
      return procedure_index;
    }
    procedure_index = findCallbackIndex(procedure_string);
    if (procedure_index >= 0)
    {
      response_.write(constants::id_constant_string,procedure_string);
      procedure_index += methods_.size() + callbacks_.size();
      return procedure_index;
    }
  }
  return procedure_index;
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
    parameter_index = findMethodParameterIndex(methods_[request_procedure_index_],parameter_string);
  }
  Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
  parameter_ptrs_ptr = &methods_[request_procedure_index_].parameter_ptrs_;
  if ((parameter_index < 0) || (parameter_index >= (int)parameter_ptrs_ptr->size()))
  {
    response_.returnParameterNotFoundError();
    parameter_index = -1;
  }
  return parameter_index;
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
      parameter_ptr = methods_[request_procedure_index_].parameter_ptrs_[parameter_index];
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
    case JsonStream::ANY_TYPE:
    {
      break;
    }
  }
  if (!in_subset)
  {
    Vector<constants::SubsetMemberType> & subset = parameter.getSubset();
    char subset_str[constants::STRING_LENGTH_ERROR];
    subset_str[0] = '\0';
    subsetToString(subset_str,
                   subset,
                   parameter.getType(),
                   parameter.getArrayElementType(),
                   constants::STRING_LENGTH_ERROR-1);
    response_.returnParameterNotInSubsetError(subset_str,
                                              parameter.getType());
  }
  else if (!in_range)
  {
    response_.returnParameterNotInRangeError(parameter.getName(),
                                             parameter.getType(),
                                             min_str,
                                             max_str);
  }
  else if (!array_length_in_range)
  {
    response_.returnParameterArrayLengthError(parameter.getName(),min_str,max_str);
  }
  else if (object_parse_unsuccessful)
  {
    response_.returnParameterObjectParseError(parameter.getName());
  }
  else if (array_parse_unsuccessful)
  {
    response_.returnParameterArrayParseError(parameter.getName());
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
        case JsonStream::ANY_TYPE:
        {
          break;
        }
      }
      break;
    }
    case JsonStream::ANY_TYPE:
    {
      break;
    }
  }
  if (!in_subset)
  {
    Vector<constants::SubsetMemberType> & subset = parameter.getSubset();
    char subset_str[constants::STRING_LENGTH_ERROR];
    subset_str[0] = '\0';
    subsetToString(subset_str,
                   subset,
                   parameter.getType(),
                   parameter.getArrayElementType(),
                   constants::STRING_LENGTH_ERROR-1);
    response_.returnParameterNotInSubsetError(subset_str,
                                              parameter.getType());
  }
  else if (!in_range)
  {
    response_.returnParameterNotInRangeError(parameter.getName(),
                                             parameter.getType(),
                                             min_str,
                                             max_str);
  }
  bool parameter_ok = in_subset && in_range;
  return parameter_ok;
}

long Server::getSerialNumber()
{
  long serial_number;
  property(constants::serial_number_property_name).getValue(serial_number);
  return serial_number;
}

void Server::initializeEeprom()
{
  if (!eeprom_initialized_sv_.valueIsDefault())
  {
    eeprom_initialized_sv_.setValueToDefault();
    setPropertiesToDefaults();
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

void Server::propertyHelp(Property & property, bool verbose)
{
  parameterHelp(property.parameter(),false);

  response_.writeKey(constants::value_constant_string);
  writePropertyToResponse(property,false,false);

  response_.writeKey(constants::default_value_constant_string);
  writePropertyToResponse(property,false,true);

  response_.writeKey(constants::parameters_constant_string);
  response_.beginArray();

  for (size_t i=0; i<Property::parameters_.size(); ++i)
  {
    if (verbose)
    {
      parameterHelp(Property::parameters_[i]);
    }
    else
    {
      response_.write(Property::parameters_[i].getName());
    }
  }

  JsonStream::JsonTypes type = property.parameter().getType();
  if (type == JsonStream::ARRAY_TYPE)
  {
    for (size_t i=0; i<Property::array_parameters_.size(); ++i)
    {
      if (verbose)
      {
        parameterHelp(Property::array_parameters_[i]);
      }
      else
      {
        response_.write(Property::array_parameters_[i].getName());
      }
    }
  }

  response_.endArray();

  response_.writeKey(constants::methods_constant_string);
  response_.beginArray();

  for (size_t i=0; i<Property::methods_.size(); ++i)
  {
    if (verbose)
    {
      methodHelp(Property::methods_[i],false);
    }
    else
    {
      response_.write(Property::methods_[i].getName());
    }
  }

  if (type == JsonStream::ARRAY_TYPE)
  {
    for (size_t i=0; i<Property::array_methods_.size(); ++i)
    {
      if (verbose)
      {
        methodHelp(Property::array_methods_[i],false);
      }
      else
      {
        response_.write(Property::array_methods_[i].getName());
      }
    }
  }

  response_.endArray();

  response_.write(constants::result_type_constant_string,type);

  response_.endObject();
}

void Server::parameterHelp(Parameter & parameter, bool end_object)
{
  response_.beginObject();
  const ConstantString & parameter_name = parameter.getName();
  response_.write(constants::name_constant_string,parameter_name);
  const ConstantString & firmware_name = parameter.getFirmwareName();
  response_.write(constants::firmware_constant_string,firmware_name);

  const ConstantString & units = parameter.getUnits();
  if (units.length() != 0)
  {
    response_.write(constants::units_constant_string,units);
  }
  JsonStream::JsonTypes type = parameter.getType();
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      response_.write(constants::type_constant_string,JsonStream::LONG_TYPE);
      if (parameter.subsetIsSet())
      {
        response_.writeKey(constants::subset_constant_string);
        response_.write(parameter.getSubset(),JsonStream::LONG_TYPE);
      }
      if (parameter.rangeIsSet())
      {
        long min = parameter.getMin().l;
        long max = parameter.getMax().l;
        response_.write(constants::min_constant_string,min);
        response_.write(constants::max_constant_string,max);
      }
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      response_.write(constants::type_constant_string,JsonStream::DOUBLE_TYPE);
      if (parameter.rangeIsSet())
      {
        double min = parameter.getMin().d;
        double max = parameter.getMax().d;
        response_.write(constants::min_constant_string,min);
        response_.write(constants::max_constant_string,max);
      }
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      response_.write(constants::type_constant_string,JsonStream::BOOL_TYPE);
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      response_.write(constants::type_constant_string,JsonStream::STRING_TYPE);
      if (parameter.subsetIsSet())
      {
        response_.writeKey(constants::subset_constant_string);
        response_.write(parameter.getSubset(),JsonStream::STRING_TYPE);
      }
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      response_.write(constants::type_constant_string,JsonStream::OBJECT_TYPE);
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      response_.write(constants::type_constant_string,JsonStream::ARRAY_TYPE);
      JsonStream::JsonTypes array_element_type = parameter.getArrayElementType();
      switch (array_element_type)
      {
        case JsonStream::LONG_TYPE:
        {
          response_.write(constants::array_element_type_constant_string,JsonStream::LONG_TYPE);
          if (parameter.subsetIsSet())
          {
            response_.writeKey(constants::array_element_subset_constant_string);
            response_.write(parameter.getSubset(),JsonStream::LONG_TYPE);
          }
          if (parameter.rangeIsSet())
          {
            long min = parameter.getMin().l;
            long max = parameter.getMax().l;
            response_.write(constants::array_element_min_constant_string,min);
            response_.write(constants::array_element_max_constant_string,max);
          }
          break;
        }
        case JsonStream::DOUBLE_TYPE:
        {
          response_.write(constants::array_element_type_constant_string,JsonStream::DOUBLE_TYPE);
          if (parameter.rangeIsSet())
          {
            double min = parameter.getMin().d;
            double max = parameter.getMax().d;
            response_.write(constants::array_element_min_constant_string,min);
            response_.write(constants::array_element_max_constant_string,max);
          }
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          response_.write(constants::array_element_type_constant_string,JsonStream::BOOL_TYPE);
          break;
        }
        case JsonStream::NULL_TYPE:
        {
          break;
        }
        case JsonStream::STRING_TYPE:
        {
          response_.write(constants::array_element_type_constant_string,JsonStream::STRING_TYPE);
          if (parameter.subsetIsSet())
          {
            response_.writeKey(constants::array_element_subset_constant_string);
            response_.write(parameter.getSubset(),JsonStream::STRING_TYPE);
          }
          break;
        }
        case JsonStream::OBJECT_TYPE:
        {
          response_.write(constants::array_element_type_constant_string,JsonStream::OBJECT_TYPE);
          break;
        }
        case JsonStream::ARRAY_TYPE:
        {
          response_.write(constants::array_element_type_constant_string,JsonStream::ARRAY_TYPE);
          break;
        }
        case JsonStream::ANY_TYPE:
        {
          break;
        }
      }
      if (parameter.arrayLengthRangeIsSet())
      {
        size_t array_length_min = parameter.getArrayLengthMin();
        size_t array_length_max = parameter.getArrayLengthMax();
        response_.write(constants::array_length_min_constant_string,array_length_min);
        response_.write(constants::array_length_max_constant_string,array_length_max);
      }
      break;
    }
    case JsonStream::ANY_TYPE:
    {
      response_.write(constants::type_constant_string,JsonStream::ANY_TYPE);
      break;
    }
  }
  if (end_object)
  {
    response_.endObject();
  }
}

void Server::methodHelp(Method & method, bool verbose)
{
  response_.beginObject();

  const ConstantString & method_name = method.getName();
  response_.write(constants::name_constant_string,method_name);
  const ConstantString & firmware_name = method.getFirmwareName();
  response_.write(constants::firmware_constant_string,firmware_name);

  response_.writeKey(constants::parameters_constant_string);
  response_.beginArray();
  Array<Parameter *,constants::METHOD_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
  parameter_ptrs_ptr = &method.parameter_ptrs_;
  for (size_t i=0; i<parameter_ptrs_ptr->size(); ++i)
  {
    if (verbose)
    {
      parameterHelp(*((*parameter_ptrs_ptr)[i]));
    }
    else
    {
      const ConstantString & parameter_name = (*parameter_ptrs_ptr)[i]->getName();
      response_.write(parameter_name);
    }
  }
  response_.endArray();

  response_.write(constants::result_type_constant_string,method.getReturnType());

  response_.endObject();
}

void Server::callbackHelp(Callback & callback, bool verbose)
{
  response_.beginObject();

  const ConstantString & callback_name = callback.getName();
  response_.write(constants::name_constant_string,callback_name);
  const ConstantString & firmware_name = callback.getFirmwareName();
  response_.write(constants::firmware_constant_string,firmware_name);

  response_.writeKey(constants::properties_constant_string);
  response_.beginArray();
  Array<Property *,constants::CALLBACK_PROPERTY_COUNT_MAX> * property_ptrs_ptr = NULL;
  property_ptrs_ptr = &callback.property_ptrs_;
  for (size_t i=0; i<property_ptrs_ptr->size(); ++i)
  {
    if (verbose)
    {
      propertyHelp(*((*property_ptrs_ptr)[i]),false);
    }
    else
    {
      const ConstantString & property_name = (*property_ptrs_ptr)[i]->getName();
      response_.write(property_name);
    }
  }
  response_.endArray();

  response_.writeKey(constants::parameters_constant_string);
  response_.beginArray();
  response_.endArray();

  response_.write(constants::result_type_constant_string,JsonStream::NULL_TYPE);

  response_.endObject();
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
    response_.writeResultKey();
    response_.beginObject();

    response_.writeKey(constants::device_id_constant_string);
    writeDeviceIdToResponse();

    if (verbose)
    {
      response_.writeKey(constants::device_info_constant_string);
      writeDeviceInfoToResponse();
    }

    response_.writeKey(constants::api_constant_string);
    char all_str[constants::all_constant_string.length() + 1];
    all_str[0] = '\0';
    constants::all_constant_string.copy(all_str);
    ArduinoJson::StaticJsonBuffer<constants::JSON_BUFFER_SIZE> json_buffer;
    ArduinoJson::JsonArray& firmware_name_array = json_buffer.createArray();
    firmware_name_array.add<char *>(all_str);
    writeApiToResponse(verbose,firmware_name_array);

    response_.endObject();
  }
  // ? method
  // ? parameter
  // ? property
  // ? callback
  // ?? method
  // ?? parameter
  // ?? property
  // ?? callback
  else if (parameter_count == 1)
  {
    const char * param_string = (*request_json_array_ptr_)[1];
    int method_index = findMethodIndex(param_string);
    if ((method_index >= 0) && (method_index < (int)methods_.size()))
    {
      // ? method
      param_error = false;
      response_.writeResultKey();
      methodHelp(methods_[method_index],verbose);
    }
    else
    {
      int parameter_index = findParameterIndex(param_string);
      if ((parameter_index >= 0) && (parameter_index < (int)parameters_.size()))
      {
        // ? parameter
        // ?? parameter
        param_error = false;
        response_.writeResultKey();
        parameterHelp(parameters_[parameter_index]);
      }
      else
      {
        int property_index = findPropertyIndex(param_string);
        if ((property_index >= 0) && (property_index < (int)properties_.max_size()))
        {
          // ? property
          // ?? property
          param_error = false;
          response_.writeResultKey();
          propertyHelp(properties_[property_index],verbose);
        }
        else
        {
          int callback_index = findCallbackIndex(param_string);
          if ((callback_index >= 0) && (callback_index < (int)callbacks_.max_size()))
          {
            // ? callback
            // ?? callback
            param_error = false;
            response_.writeResultKey();
            callbackHelp(callbacks_[callback_index],verbose);
          }
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
    if ((method_index >= 0) && (method_index < (int)methods_.size()))
    {
      int parameter_index = findMethodParameterIndex(methods_[method_index],(const char *)(*request_json_array_ptr_)[2]);
      if ((parameter_index >= 0) && (parameter_index < (int)parameters_.size()))
      {
        param_error = false;
        response_.writeResultKey();
        parameterHelp(*methods_[method_index].parameter_ptrs_[parameter_index]);
      }
    }
  }
  // ? unknown
  // ?? unknown
  // ? method unknown
  // ?? method unknown
  if (param_error)
  {
    response_.returnParameterInvalidError(constants::empty_constant_string);
  }
}

void Server::writeDeviceIdToResponse()
{
  if (response_.error())
  {
    return;
  }
  response_.beginObject();

  response_.write(constants::name_constant_string,device_name_ptr_);
  response_.write(constants::form_factor_constant_string,form_factor_ptr_);
  response_.write(constants::serial_number_property_name,getSerialNumber());

  response_.endObject();
}

void Server::writeFirmwareInfoToResponse()
{
  if (response_.error())
  {
    return;
  }
  char version_str[constants::STRING_LENGTH_VERSION];

  response_.beginArray();
  for (size_t i=0; i<firmware_info_array_.size(); ++i)
  {
    const constants::FirmwareInfo * firmware_info_ptr = firmware_info_array_[i];
    response_.beginObject();
    response_.write(constants::name_constant_string,firmware_info_ptr->name_ptr);
    versionToString(version_str,
                    firmware_info_ptr->version_major,
                    firmware_info_ptr->version_minor,
                    firmware_info_ptr->version_patch,
                    constants::STRING_LENGTH_VERSION-1);
    if (strlen(version_str) > 0)
    {
      response_.write(constants::version_constant_string,version_str);
    }
    response_.endObject();
  }
  response_.endArray();
}

void Server::writeHardwareInfoToResponse()
{
  if (response_.error())
  {
    return;
  }
  char version_str[constants::STRING_LENGTH_VERSION];

  response_.beginArray();
  for (size_t i=0; i<hardware_info_array_.size(); ++i)
  {
    const constants::HardwareInfo * hardware_info_ptr = hardware_info_array_[i];
    response_.beginObject();
    response_.write(constants::name_constant_string,hardware_info_ptr->name_ptr);
    if (hardware_info_ptr->part_number > 0)
    {
      response_.write(constants::part_number_constant_string,hardware_info_ptr->part_number);
    }
    versionToString(version_str,
                    hardware_info_ptr->version_major,
                    hardware_info_ptr->version_minor,
                    -1,
                    constants::STRING_LENGTH_VERSION-1);
    if (strlen(version_str) > 0)
    {
      response_.write(constants::version_constant_string,version_str);
    }
    response_.endObject();
  }
  response_.endArray();
}

void Server::writeDeviceInfoToResponse()
{
  if (response_.error())
  {
    return;
  }
  response_.beginObject();

  response_.write(constants::processor_constant_string,constants::processor_name_constant_string);

  response_.writeKey(constants::hardware_constant_string);
  writeHardwareInfoToResponse();

  response_.writeKey(constants::firmware_constant_string);
  writeFirmwareInfoToResponse();

  response_.endObject();
}

void Server::writeApiToResponse(bool verbose, ArduinoJson::JsonArray & firmware_name_array)
{
  if (response_.error())
  {
    return;
  }
  response_.beginObject();

  response_.write(constants::firmware_constant_string,&firmware_name_array);

  if (!verbose)
  {
    response_.writeKey(constants::methods_constant_string);
    response_.beginArray();
    for (size_t method_index=0; method_index<methods_.size(); ++method_index)
    {
      if (method_index > private_method_index_)
      {
        Method & method = methods_[method_index];
        if (method.firmwareNameInArray(firmware_name_array))
        {
          const ConstantString & method_name = method.getName();
          response_.write(method_name);
        }
      }
    }
    response_.endArray();

    response_.writeKey(constants::parameters_constant_string);
    response_.beginArray();
    for (size_t parameter_index=0; parameter_index<parameters_.size(); ++parameter_index)
    {
      Parameter & parameter = parameters_[parameter_index];
      if (parameter.firmwareNameInArray(firmware_name_array))
      {
        const ConstantString & parameter_name = parameter.getName();
        response_.write(parameter_name);
      }
    }
    response_.endArray();

    response_.writeKey(constants::properties_constant_string);
    response_.beginArray();
    for (size_t property_index=0; property_index<properties_.size(); ++property_index)
    {
      Property & property = properties_[property_index];
      if (property.firmwareNameInArray(firmware_name_array))
      {
        const ConstantString & property_name = properties_[property_index].getName();
        response_.write(property_name);
      }
    }
    response_.endArray();

    response_.writeKey(constants::callbacks_constant_string);
    response_.beginArray();
    for (size_t callback_index=0; callback_index<callbacks_.size(); ++callback_index)
    {
      Callback & callback = callbacks_[callback_index];
      if (callback.firmwareNameInArray(firmware_name_array))
      {
        const ConstantString & callback_name = callback.getName();
        response_.write(callback_name);
      }
    }
    response_.endArray();
  }
  else
  {
    response_.writeKey(constants::methods_constant_string);
    response_.beginArray();
    for (size_t method_index=0; method_index<methods_.size(); ++method_index)
    {
      if (method_index > private_method_index_)
      {
        methodHelp(methods_[method_index],false);
      }
    }
    response_.endArray();

    response_.writeKey(constants::parameters_constant_string);
    response_.beginArray();
    for (size_t parameter_index=0; parameter_index<parameters_.size(); ++parameter_index)
    {
      parameterHelp(parameters_[parameter_index]);
    }
    response_.endArray();

    response_.writeKey(constants::properties_constant_string);
    response_.beginArray();
    for (size_t property_index=0; property_index<properties_.size(); ++property_index)
    {
      propertyHelp(properties_[property_index],false);
    }
    response_.endArray();

    response_.writeKey(constants::callbacks_constant_string);
    response_.beginArray();
    for (size_t callback_index=0; callback_index<callbacks_.size(); ++callback_index)
    {
      callbackHelp(callbacks_[callback_index],false);
    }
    response_.endArray();
  }
  response_.endObject();
}

void Server::writePropertyToResponse(Property & property,
                                     bool write_key,
                                     bool write_default,
                                     int element_index)
{
  if (response_.error())
  {
    return;
  }
  const ConstantString & property_name = property.getName();
  if (write_key)
  {
    response_.writeKey(property_name);
  }
  JsonStream::JsonTypes property_type = property.getType();
  switch (property_type)
  {
    case JsonStream::LONG_TYPE:
    {
      if (element_index >= 0)
      {
        response_.returnParameterInvalidError(constants::property_not_array_type_error_data);
        return;
      }
      long property_value;
      if (write_default)
      {
        property.getDefaultValue(property_value);
      }
      else
      {
        property.getValue(property_value);
      }
      response_.write(property_value);
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      if (element_index >= 0)
      {
        response_.returnParameterInvalidError(constants::property_not_array_type_error_data);
        return;
      }
      double property_value;
      if (write_default)
      {
        property.getDefaultValue(property_value);
      }
      else
      {
        property.getValue(property_value);
      }
      response_.write(property_value);
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      if (element_index >= 0)
      {
        response_.returnParameterInvalidError(constants::property_not_array_type_error_data);
        return;
      }
      bool property_value;
      if (write_default)
      {
        property.getDefaultValue(property_value);
      }
      else
      {
        property.getValue(property_value);
      }
      response_.write(property_value);
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      size_t array_length = property.getArrayLength();
      if (element_index >= 0)
      {
        if (element_index >= ((int)array_length-1))
        {
          response_.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
          return;
        }
        size_t array_length = 2;
        char char_array[array_length];
        char property_element_value;
        bool success = property.getElementValue(element_index,property_element_value);
        if (success)
        {
          char_array[0] = property_element_value;
          char_array[1] = '\0';
        }
        else
        {
          char_array[0] = '\0';
        }
        response_.write(char_array);
        return;
      }
      char char_array[array_length];
      if (write_default)
      {
        property.getDefaultValue(char_array,array_length);
      }
      else
      {
        property.getValue(char_array,array_length);
      }
      response_.write(char_array);
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      const JsonStream::JsonTypes array_element_type = property.getArrayElementType();
      size_t array_length = property.getArrayLength();
      if (element_index >= (int)array_length)
      {
        response_.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
        return;
      }
      switch (array_element_type)
      {
        case JsonStream::LONG_TYPE:
        {
          if (element_index < 0)
          {
            long property_value[array_length];
            if (write_default)
            {
              property.getDefaultValue(property_value,array_length);
            }
            else
            {
              property.getValue(property_value,array_length);
            }
            response_.writeArray(property_value,array_length);
          }
          else
          {
            long property_value;
            if (write_default)
            {
              property.getDefaultElementValue(element_index,property_value);
            }
            else
            {
              property.getElementValue(element_index,property_value);
            }
            response_.write(property_value);
          }
          break;
        }
        case JsonStream::DOUBLE_TYPE:
        {
          if (element_index < 0)
          {
            double property_value[array_length];
            if (write_default)
            {
              property.getDefaultValue(property_value,array_length);
            }
            else
            {
              property.getValue(property_value,array_length);
            }
            response_.writeArray(property_value,array_length);
          }
          else
          {
            double property_value;
            if (write_default)
            {
              property.getDefaultElementValue(element_index,property_value);
            }
            else
            {
              property.getElementValue(element_index,property_value);
            }
            response_.write(property_value);
          }
          break;
        }
        case JsonStream::BOOL_TYPE:
        {
          if (element_index < 0)
          {
            bool property_value[array_length];
            if (write_default)
            {
              property.getDefaultValue(property_value,array_length);
            }
            else
            {
              property.getValue(property_value,array_length);
            }
            response_.writeArray(property_value,array_length);
          }
          else
          {
            bool property_value;
            if (write_default)
            {
              property.getDefaultElementValue(element_index,property_value);
            }
            else
            {
              property.getElementValue(element_index,property_value);
            }
            response_.write(property_value);
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
        case JsonStream::ANY_TYPE:
        {
          break;
        }
      }
      break;
    }
    case JsonStream::ANY_TYPE:
    {
      break;
    }
  }
}

void Server::versionToString(char* destination,
                             const long major,
                             const long minor,
                             const long patch,
                             const size_t num)
{
  size_t length_left = num;
  if (length_left == 0)
  {
    return;
  }
  destination[0] = '\0';
  if ((major == 0) && (minor == 0) && (patch <= 0))
  {
    return;
  }
  char version_property_str[constants::STRING_LENGTH_VERSION_PROPERTY];
  char version_property_sep_str[constants::version_property_separator_constant_string.length()+1];
  constants::version_property_separator_constant_string.copy(version_property_sep_str);

  // major
  version_property_str[0] = '\0';
  ltoa(major,version_property_str,10);
  strncat(destination,version_property_str,length_left);
  length_left = num - strlen(destination);
  strncat(destination,version_property_sep_str,length_left);
  length_left = num - strlen(destination);

  // minor
  version_property_str[0] = '\0';
  ltoa(minor,version_property_str,10);
  strncat(destination,version_property_str,length_left);
  length_left = num - strlen(destination);

  // patch
  if (patch >= 0)
  {
    strncat(destination,version_property_sep_str,length_left);
    length_left = num - strlen(destination);
    version_property_str[0] = '\0';
    ltoa(patch,version_property_str,10);
    strncat(destination,version_property_str,length_left);
  }
}

void Server::subsetToString(char * destination,
                            Vector<constants::SubsetMemberType> & subset,
                            const JsonStream::JsonTypes & parameter_type,
                            const JsonStream::JsonTypes & parameter_array_element_type,
                            const size_t num)
{
  JsonStream::JsonTypes type = parameter_type;
  if (type == JsonStream::ARRAY_TYPE)
  {
    type = parameter_array_element_type;
  }
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
      case JsonStream::ANY_TYPE:
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

// Handlers
void Server::getProcedureIdsHandler()
{
  response_.writeResultKey();
  response_.beginObject();
  size_t procedure_index;
  for (size_t method_index=0; method_index<methods_.size(); ++method_index)
  {
    if (method_index > private_method_index_)
    {
      const ConstantString & method_name = methods_[method_index].getName();
      procedure_index = method_index;
      response_.write(method_name,procedure_index);
    }
  }
  for (size_t callback_index=0; callback_index<callbacks_.size(); ++callback_index)
  {
    const ConstantString & callback_name = callbacks_[callback_index].getName();
    procedure_index = callback_index + methods_.size();
    response_.write(callback_name,procedure_index);
  }
  for (size_t property_index=0; property_index<properties_.size(); ++property_index)
  {
    const ConstantString & property_name = properties_[property_index].getName();
    procedure_index = property_index + methods_.size() + callbacks_.size();
    response_.write(property_name,procedure_index);
  }
  response_.endObject();
}

void Server::helpHandler()
{
  help(false);
}

void Server::verboseHelpHandler()
{
  help(true);
}

void Server::getDeviceIdHandler()
{
  response_.writeResultKey();
  writeDeviceIdToResponse();
}

void Server::getDeviceInfoHandler()
{
  response_.writeResultKey();
  writeDeviceInfoToResponse();
}

void Server::getApiHandler()
{
  ArduinoJson::JsonArray * firmware_name_array_ptr;
  parameter(constants::firmware_constant_string).getValue(firmware_name_array_ptr);
  response_.writeResultKey();
  writeApiToResponse(false,*firmware_name_array_ptr);
}

void Server::getApiVerboseHandler()
{
  ArduinoJson::JsonArray * firmware_name_array_ptr;
  parameter(constants::firmware_constant_string).getValue(firmware_name_array_ptr);
  response_.writeResultKey();
  writeApiToResponse(true,*firmware_name_array_ptr);
}

#ifdef __AVR__
void Server::getMemoryFreeHandler()
{
  response_.returnResult(freeMemory());
}
#endif

void Server::getPropertyDefaultValuesHandler()
{
  response_.writeResultKey();
  response_.beginObject();
  for (size_t i=0; i<properties_.size(); ++i)
  {
    Property & property = properties_[i];
    writePropertyToResponse(property,true,true);
  }
  response_.endObject();
}

void Server::setPropertiesToDefaultsHandler()
{
  setPropertiesToDefaults();
}

void Server::setPropertyToDefaultHandler()
{
  // const char * property_name = getParameterValue(constants::property_name_parameter_name);
  // int property_index = findPropertyIndex(property_name);
  // if ((property_index >= 0) && (property_index < (int)properties_.size()))
  // {
  //   Property & property = properties_[property_index];
  //   property.setValueToDefault();
  // }
  // else
  // {
  //   response_.returnParameterInvalidError(constants::property_not_found_error_data);
  // }
}

void Server::getPropertyValuesHandler()
{
  // response_.writeResultKey();
  // response_.beginObject();
  // for (size_t i=0; i<properties_.size(); ++i)
  // {
  //   Property & property = properties_[i];
  //   writePropertyToResponse(property,true,false);
  // }
  // response_.endObject();
}

void Server::getPropertyValueHandler()
{
  // response_.writeResultKey();
  // const char * property_name = getParameterValue(constants::property_name_parameter_name);
  // int property_index = findPropertyIndex(property_name);
  // if ((property_index >= 0) && (property_index < (int)properties_.size()))
  // {
  //   Property & property = properties_[property_index];
  //   writePropertyToResponse(property,false,false);
  // }
  // else
  // {
  //   response_.returnParameterInvalidError(constants::property_not_found_error_data);
  // }
}

void Server::getPropertyElementValueHandler()
{
  // response_.writeResultKey();
  // const char * property_name = getParameterValue(constants::property_name_parameter_name);
  // long property_element_index = getParameterValue(constants::property_element_index_parameter_name);
  // if (property_element_index < 0)
  // {
  //   response_.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
  //   return;
  // }
  // int property_index = findPropertyIndex(property_name);
  // if ((property_index >= 0) && (property_index < (int)properties_.size()))
  // {
  //   Property & property = properties_[property_index];
  //   writePropertyToResponse(property,false,false,property_element_index);
  // }
  // else
  // {
  //   response_.returnParameterInvalidError(constants::property_not_found_error_data);
  // }
}

void Server::setPropertyValueHandler()
{
  // const char * property_name = getParameterValue(constants::property_name_parameter_name);
  // int property_index = findPropertyIndex(property_name);
  // if ((property_index >= 0) && (property_index < (int)properties_.size()))
  // {
  //   Property & property = properties_[property_index];
  //   ArduinoJson::JsonVariant json_value = getParameterValue(constants::property_value_parameter_name);
  //   bool parameter_ok = checkParameter(property.parameter(),json_value);
  //   if (!parameter_ok)
  //   {
  //     return;
  //   }
  //   JsonStream::JsonTypes property_type = property.getType();
  //   switch (property_type)
  //   {
  //     case JsonStream::LONG_TYPE:
  //     {
  //       long property_value = getParameterValue(constants::property_value_parameter_name);
  //       property.setValue(property_value);
  //       break;
  //     }
  //     case JsonStream::DOUBLE_TYPE:
  //     {
  //       double property_value = getParameterValue(constants::property_value_parameter_name);
  //       property.setValue(property_value);
  //       break;
  //     }
  //     case JsonStream::BOOL_TYPE:
  //     {
  //       bool property_value = getParameterValue(constants::property_value_parameter_name);
  //       property.setValue(property_value);
  //       break;
  //     }
  //     case JsonStream::NULL_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::STRING_TYPE:
  //     {
  //       const char * property_value = getParameterValue(constants::property_value_parameter_name);
  //       size_t array_length = strlen(property_value) + 1;
  //       property.setValue(property_value,array_length);
  //       break;
  //     }
  //     case JsonStream::OBJECT_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::ARRAY_TYPE:
  //     {
  //       ArduinoJson::JsonArray & property_value = getParameterValue(constants::property_value_parameter_name);
  //       property.setValue(property_value);
  //       break;
  //     }
  //     case JsonStream::ANY_TYPE:
  //     {
  //       break;
  //     }
  //   }
  // }
  // else
  // {
  //   response_.returnParameterInvalidError(constants::property_not_found_error_data);
  // }
}

void Server::setPropertyElementValueHandler()
{
  // const char * property_name = getParameterValue(constants::property_name_parameter_name);
  // long property_element_index = getParameterValue(constants::property_element_index_parameter_name);
  // if (property_element_index < 0)
  // {
  //   response_.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
  //   return;
  // }
  // int property_index = findPropertyIndex(property_name);
  // if ((property_index >= 0) && (property_index < (int)properties_.size()))
  // {
  //   Property & property = properties_[property_index];
  //   ArduinoJson::JsonVariant json_value = getParameterValue(constants::property_value_parameter_name);
  //   bool parameter_ok = checkArrayParameterElement(property.parameter(),json_value);
  //   if (!parameter_ok)
  //   {
  //     return;
  //   }
  //   JsonStream::JsonTypes property_type = property.getType();
  //   switch (property_type)
  //   {
  //     case JsonStream::LONG_TYPE:
  //     {
  //       response_.returnParameterInvalidError(constants::property_not_array_type_error_data);
  //       break;
  //     }
  //     case JsonStream::DOUBLE_TYPE:
  //     {
  //       response_.returnParameterInvalidError(constants::property_not_array_type_error_data);
  //       break;
  //     }
  //     case JsonStream::BOOL_TYPE:
  //     {
  //       response_.returnParameterInvalidError(constants::property_not_array_type_error_data);
  //       break;
  //     }
  //     case JsonStream::NULL_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::STRING_TYPE:
  //     {
  //       if (!property.stringIsSavedAsCharArray())
  //       {
  //         response_.returnParameterInvalidError(constants::cannot_set_element_in_string_property_with_subset_error_data);
  //         break;
  //       }
  //       size_t array_length = property.getArrayLength();
  //       if ((size_t)property_element_index >= (array_length - 1))
  //       {
  //         response_.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
  //         return;
  //       }
  //       const char * property_value = getParameterValue(constants::property_value_parameter_name);
  //       size_t string_length = strlen(property_value);
  //       if (string_length >= 1)
  //       {
  //         char v = property_value[0];
  //         property.setElementValue(property_element_index,v);
  //       }
  //       break;
  //     }
  //     case JsonStream::OBJECT_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::ARRAY_TYPE:
  //     {
  //       size_t array_length = property.getArrayLength();
  //       if ((size_t)property_element_index >= array_length)
  //       {
  //         response_.returnParameterInvalidError(constants::property_element_index_out_of_bounds_error_data);
  //         return;
  //       }
  //       JsonStream::JsonTypes array_element_type = property.getArrayElementType();
  //       switch (array_element_type)
  //       {
  //         case JsonStream::LONG_TYPE:
  //         {
  //           long property_value = getParameterValue(constants::property_value_parameter_name);
  //           property.setElementValue(property_element_index,property_value);
  //           break;
  //         }
  //         case JsonStream::DOUBLE_TYPE:
  //         {
  //           double property_value = getParameterValue(constants::property_value_parameter_name);
  //           property.setElementValue(property_element_index,property_value);
  //           break;
  //         }
  //         case JsonStream::BOOL_TYPE:
  //         {
  //           bool property_value = getParameterValue(constants::property_value_parameter_name);
  //           property.setElementValue(property_element_index,property_value);
  //           break;
  //         }
  //         case JsonStream::NULL_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::STRING_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::OBJECT_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::ARRAY_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::ANY_TYPE:
  //         {
  //           break;
  //         }
  //       }
  //       break;
  //     }
  //     case JsonStream::ANY_TYPE:
  //     {
  //       break;
  //     }
  //   }
  // }
  // else
  // {
  //   response_.returnParameterInvalidError(constants::property_not_found_error_data);
  // }
}

void Server::setAllPropertyElementValuesHandler()
{
  // const char * property_name = getParameterValue(constants::property_name_parameter_name);
  // int property_index = findPropertyIndex(property_name);
  // if ((property_index >= 0) && (property_index < (int)properties_.size()))
  // {
  //   Property & property = properties_[property_index];
  //   ArduinoJson::JsonVariant json_value = getParameterValue(constants::property_value_parameter_name);
  //   bool parameter_ok = checkArrayParameterElement(property.parameter(),json_value);
  //   if (!parameter_ok)
  //   {
  //     return;
  //   }
  //   JsonStream::JsonTypes property_type = property.getType();
  //   switch (property_type)
  //   {
  //     case JsonStream::LONG_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::DOUBLE_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::BOOL_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::NULL_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::STRING_TYPE:
  //     {
  //       if (!property.stringIsSavedAsCharArray())
  //       {
  //         response_.returnParameterInvalidError(constants::cannot_set_element_in_string_property_with_subset_error_data);
  //         break;
  //       }
  //       const char * property_value = getParameterValue(constants::property_value_parameter_name);
  //       size_t string_length = strlen(property_value);
  //       if (string_length >= 1)
  //       {
  //         char v = property_value[0];
  //         property.setAllElementValues(v);
  //       }
  //       break;
  //     }
  //     case JsonStream::OBJECT_TYPE:
  //     {
  //       break;
  //     }
  //     case JsonStream::ARRAY_TYPE:
  //     {
  //       JsonStream::JsonTypes array_element_type = property.getArrayElementType();
  //       switch (array_element_type)
  //       {
  //         case JsonStream::LONG_TYPE:
  //         {
  //           long property_value = getParameterValue(constants::property_value_parameter_name);
  //           property.setAllElementValues(property_value);
  //           break;
  //         }
  //         case JsonStream::DOUBLE_TYPE:
  //         {
  //           double property_value = getParameterValue(constants::property_value_parameter_name);
  //           property.setAllElementValues(property_value);
  //           break;
  //         }
  //         case JsonStream::BOOL_TYPE:
  //         {
  //           bool property_value = getParameterValue(constants::property_value_parameter_name);
  //           property.setAllElementValues(property_value);
  //           break;
  //         }
  //         case JsonStream::NULL_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::STRING_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::OBJECT_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::ARRAY_TYPE:
  //         {
  //           break;
  //         }
  //         case JsonStream::ANY_TYPE:
  //         {
  //           break;
  //         }
  //       }
  //       break;
  //     }
  //     case JsonStream::ANY_TYPE:
  //     {
  //       break;
  //     }
  //   }
  // }
  // else
  // {
  //   response_.returnParameterInvalidError(constants::property_not_found_error_data);
  // }
}

}
