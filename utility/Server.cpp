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
  request_method_index_ = -1;
  property_function_index_ = -1;
  callback_function_index_ = -1;
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

  // Hardware

  // Interrupts

  // Firmware
  addFirmware(constants::firmware_info,
              server_properties_,
              server_parameters_,
              server_functions_,
              server_callbacks_);

  // Properties
  Property::response_ptr_ = &response_;
  Property::write_property_to_response_functor_ = makeFunctor((Functor4<Property &, bool, bool, int> *)0,*this,&Server::writePropertyToResponse);
  Property::get_parameter_value_functor_ = makeFunctor((Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> *)0,*this,&Server::getParameterValue);

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

  // Functions
  Function & get_method_ids_function = createFunction(constants::get_method_ids_function_name);
  get_method_ids_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getMethodIdsHandler));
  get_method_ids_function.setReturnTypeObject();
  private_function_index_ = 0;

  Function & help_function = createFunction(constants::help_function_name);
  help_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::helpHandler));
  help_function.setReturnTypeObject();
  private_function_index_++;

  Function & verbose_help_function = createFunction(constants::verbose_help_function_name);
  verbose_help_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::verboseHelpHandler));
  verbose_help_function.setReturnTypeObject();
  private_function_index_++;

  Function & get_device_id_function = createFunction(constants::get_device_id_function_name);
  get_device_id_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getDeviceIdHandler));
  get_device_id_function.setReturnTypeObject();

  Function & get_device_info_function = createFunction(constants::get_device_info_function_name);
  get_device_info_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getDeviceInfoHandler));
  get_device_info_function.setReturnTypeObject();

  Function & get_interrupt_info_function = createFunction(constants::get_interrupt_info_function_name);
  get_interrupt_info_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getInterruptInfoHandler));
  get_interrupt_info_function.setReturnTypeObject();

  Function & detach_all_interrupts_function = createFunction(constants::detach_all_interrupts_function_name);
  detach_all_interrupts_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::detachAllInterruptsHandler));

  Function & get_api_function = createFunction(constants::get_api_function_name);
  get_api_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getApiHandler));
  get_api_function.addParameter(firmware_parameter);
  get_api_function.setReturnTypeObject();

  Function & get_api_verbose_function = createFunction(constants::get_api_verbose_function_name);
  get_api_verbose_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getApiVerboseHandler));
  get_api_verbose_function.addParameter(firmware_parameter);
  get_api_verbose_function.setReturnTypeObject();

  Function & get_property_default_values_function = createFunction(constants::get_property_default_values_function_name);
  get_property_default_values_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getPropertyDefaultValuesHandler));
  get_property_default_values_function.setReturnTypeObject();

  Function & set_properties_to_defaults_function = createFunction(constants::set_properties_to_defaults_function_name);
  set_properties_to_defaults_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::setPropertiesToDefaultsHandler));

  Function & get_property_values_function = createFunction(constants::get_property_values_function_name);
  get_property_values_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getPropertyValuesHandler));
  get_property_values_function.setReturnTypeObject();

#ifdef __AVR__
  Function & get_memory_free_function = createFunction(constants::get_memory_free_function_name);
  get_memory_free_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getMemoryFreeHandler));
  get_memory_free_function.setReturnTypeLong();
#endif

  // Callbacks
  Callback::interrupt_name_array_ptr_ = &interrupt_name_array_;
  Callback::find_interrupt_ptr_functor_ = makeFunctor((Functor1wRet<const char *, Interrupt *> *)0,*this,&Server::findInterruptPtr);
  Callback::get_parameter_value_functor_ = makeFunctor((Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> *)0,*this,&Server::getParameterValue);

  // Server
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

// Hardware

// Interrupts
Interrupt & Server::createInterrupt(const ConstantString & interrupt_name,
                                    const size_t pin)
{
  int interrupt_index = findInterruptIndex(interrupt_name);
  if (interrupt_index < 0)
  {
    constants::SubsetMemberType int_name;
    int_name.cs_ptr = &interrupt_name;
    interrupt_name_array_.push_back(int_name);
    interrupts_.push_back(Interrupt(interrupt_name,pin));
    return interrupts_.back();
  }
  return dummy_interrupt_;
}

Interrupt & Server::interrupt(const ConstantString & interrupt_name)
{
  int interrupt_index = findInterruptIndex(interrupt_name);
  if ((interrupt_index >= 0) && (interrupt_index < (int)interrupts_.size()))
  {
    return interrupts_[interrupt_index];
  }
  return dummy_interrupt_;
}

Interrupt * Server::findInterruptPtr(const char * interrupt_name)
{
  int interrupt_index = findInterruptIndex(interrupt_name);
  if ((interrupt_index >= 0) && (interrupt_index < (int)interrupts_.size()))
  {
    return &interrupts_[interrupt_index];
  }
  return NULL;
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
  return dummy_parameter_;
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

// Functions
Function & Server::createFunction(const ConstantString & function_name)
{
  int function_index = findFunctionIndex(function_name);
  if (function_index < 0)
  {
    functions_.push_back(Function(function_name));
    const ConstantString * firmware_name_ptr = firmware_info_array_.back()->name_ptr;
    functions_.back().setFirmwareName(*firmware_name_ptr);
    return functions_.back();
  }
  return dummy_function_;
}

Function & Server::function(const ConstantString & function_name)
{
  int function_index = findFunctionIndex(function_name);
  if ((function_index >= 0) && (function_index < (int)functions_.size()))
  {
    return functions_[function_index];
  }
  return dummy_function_;
}

Function & Server::copyFunction(Function function,const ConstantString & function_name)
{
  functions_.push_back(function);
  functions_.back().setName(function_name);
  return functions_.back();
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
  return dummy_callback_;
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
  // index 0 is the request method, index 1 is the first parameter
  int parameter_index = 1;
  if (request_method_index_ < (int)functions_.size())
  {
    int function_index = request_method_index_;
    Function & function = functions_[function_index];
    parameter_index += findFunctionParameterIndex(function,parameter_name);
  }
  else if (request_method_index_ < (int)(functions_.size() + callbacks_.size()))
  {
    if (callback_function_index_ >= 0)
    {
      int callback_index = request_method_index_ - functions_.size();
      Callback & callback = callbacks_[callback_index];
      Function & function = callback.functions_[callback_function_index_];

      // index 0 is the request method, index 1 is the callback function
      parameter_index += findFunctionParameterIndex(function,parameter_name) + 1;
    }
  }
  else if (request_method_index_ < (int)(functions_.size() + callbacks_.size() + properties_.size()))
  {
    if (property_function_index_ >= 0)
    {
      int property_index = request_method_index_ - functions_.size() - callbacks_.size();
      Property & property = properties_[property_index];
      Function & function = property.functions_[property_function_index_];

      // index 0 is the request method, index 1 is the property function
      parameter_index += findFunctionParameterIndex(function,parameter_name) + 1;
    }
  }
  return (*request_json_array_ptr_)[parameter_index];
}

void Server::processRequestArray()
{
  const char * method_string = (*request_json_array_ptr_)[0];
  request_method_index_ = findMethodIndex(method_string);
  if (request_method_index_ >= 0)
  {
    int array_elements_count = countJsonArrayElements((*request_json_array_ptr_));
    int parameter_count = array_elements_count - 1;
    char question_str[constants::question_constant_string.length()+1];
    constants::question_constant_string.copy(question_str);
    char question_double_str[constants::question_double_constant_string.length()+1];
    constants::question_double_constant_string.copy(question_double_str);
    if (request_method_index_ < (int)functions_.size())
    {
      int function_index = request_method_index_;
      Function & function = functions_[function_index];
      // function ?
      if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_str) == 0))
      {
        response_.writeResultKey();
        functionHelp(function,false);
      }
      // function ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        functionHelp(function,true);
      }
      // function parameter ?
      // function parameter ??
      else if ((parameter_count == 2) &&
               ((strcmp((*request_json_array_ptr_)[2],question_str) == 0) ||
                (strcmp((*request_json_array_ptr_)[2],question_double_str) == 0)))
      {
        int parameter_index = processParameterString(function,(*request_json_array_ptr_)[1]);
        if (parameter_index >= 0)
        {
          Parameter * parameter_ptr;
          parameter_ptr = function.parameter_ptrs_[parameter_index];
          response_.writeResultKey();
          parameterHelp(*parameter_ptr);
        }
      }
      // execute private function without checking parameters
      else if (request_method_index_ <= private_function_index_)
      {
        function.functor();
      }
      // check parameter count
      else if (parameter_count != function.getParameterCount())
      {
        response_.returnParameterCountError(parameter_count,function.getParameterCount());
        return;
      }
      // check parameters and call function functor
      else
      {
        ArduinoJson::JsonArray::iterator iterator = request_json_array_ptr_->begin();
        // do not check function at begin
        iterator++;
        bool parameters_ok = checkParameters(function,iterator);
        if (parameters_ok)
        {
          function.functor();
        }
      }
    }
    else if (request_method_index_ < (int)(functions_.size() + callbacks_.size()))
    {
      int callback_index = request_method_index_ - functions_.size();
      Callback & callback = callbacks_[callback_index];
      // callback ?
      if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_str) == 0))
      {
        response_.writeResultKey();
        callbackHelp(callback,false);
      }
      // callback ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        callbackHelp(callback,true);
      }
      // check parameter count
      else if (parameter_count == 0)
      {
        response_.returnParameterCountError(parameter_count,1);
        return;
      }
      // callback function
      else
      {
        callback.updateFunctionsAndParameters();

        // index 0 is the request method, index 1 is the callback function
        const char * callback_function_name = (*request_json_array_ptr_)[1];
        callback_function_index_ = callback.findFunctionIndex(callback_function_name);
        if (callback_function_index_ < 0)
        {
          response_.returnCallbackFunctionNotFoundError();
          return;
        }

        Function & function = callback.functions_[callback_function_index_];

        int callback_parameter_count = parameter_count - 1;

        // callback function ?
        if ((callback_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_str) == 0))
        {
          response_.writeResultKey();
          functionHelp(function,false);
        }
        // callback function ??
        else if ((callback_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_double_str) == 0))
        {
          response_.writeResultKey();
          functionHelp(function,true);
        }
        // callback function parameter ?
        // callback function parameter ??
        else if ((callback_parameter_count == 2) &&
                 ((strcmp((*request_json_array_ptr_)[3],question_str) == 0) ||
                  (strcmp((*request_json_array_ptr_)[3],question_double_str) == 0)))
        {
          int parameter_index = processParameterString(function,(*request_json_array_ptr_)[2]);
          if (parameter_index >= 0)
          {
            Parameter * parameter_ptr;
            parameter_ptr = function.parameter_ptrs_[parameter_index];
            response_.writeResultKey();
            parameterHelp(*parameter_ptr);
          }
        }
        // check callback parameter count
        else if (callback_parameter_count != function.getParameterCount())
        {
          response_.returnCallbackParameterCountError(callback_parameter_count,function.getParameterCount());
          return;
        }
        // check callback parameters and call callback function functor
        else
        {
          ArduinoJson::JsonArray::iterator iterator = request_json_array_ptr_->begin();
          // do not check callback at begin
          iterator++;
          // do not check callback method at begin + 1
          iterator++;
          long test = *iterator;
          bool parameters_ok = checkParameters(function,iterator);
          if (parameters_ok)
          {
            function.functor();
          }
        }
      }
    }
    else if (request_method_index_ < (int)(functions_.size() + callbacks_.size() + properties_.size()))
    {
      int property_index = request_method_index_ - functions_.size() - callbacks_.size();
      Property & property = properties_[property_index];
      // property ?
      if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_str) == 0))
      {
        response_.writeResultKey();
        propertyHelp(property,false);
      }
      // property ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        propertyHelp(property,true);
      }
      // check parameter count
      else if (parameter_count == 0)
      {
        response_.returnParameterCountError(parameter_count,1);
        return;
      }
      // property function
      else
      {
        property.updateFunctionsAndParameters();

        // index 0 is the request method, index 1 is the property function
        const char * property_function_name = (*request_json_array_ptr_)[1];
        property_function_index_ = property.findFunctionIndex(property_function_name);
        if (property_function_index_ < 0)
        {
          response_.returnPropertyFunctionNotFoundError();
          return;
        }

        Function & function = property.functions_[property_function_index_];

        int property_parameter_count = parameter_count - 1;

        // property function ?
        if ((property_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_str) == 0))
        {
          response_.writeResultKey();
          functionHelp(function,false);
        }
        // property function ??
        else if ((property_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_double_str) == 0))
        {
          response_.writeResultKey();
          functionHelp(function,true);
        }
        // property function parameter ?
        // property function parameter ??
        else if ((property_parameter_count == 2) &&
                 ((strcmp((*request_json_array_ptr_)[3],question_str) == 0) ||
                  (strcmp((*request_json_array_ptr_)[3],question_double_str) == 0)))
        {
          int parameter_index = processParameterString(function,(*request_json_array_ptr_)[2]);
          if (parameter_index >= 0)
          {
            Parameter * parameter_ptr;
            parameter_ptr = function.parameter_ptrs_[parameter_index];
            response_.writeResultKey();
            parameterHelp(*parameter_ptr);
          }
        }
        // check property parameter count
        else if (property_parameter_count != function.getParameterCount())
        {
          response_.returnPropertyParameterCountError(property_parameter_count,function.getParameterCount());
          return;
        }
        // check property parameters and call property function functor
        else
        {
          ArduinoJson::JsonArray::iterator iterator = request_json_array_ptr_->begin();
          // do not check property at begin
          iterator++;
          // do not check property method at begin + 1
          iterator++;
          long test = *iterator;
          bool parameters_ok = checkParameters(function,iterator);
          if (parameters_ok)
          {
            function.functor();
          }
        }
      }
    }
  }
  else
  {
    response_.returnMethodNotFoundError();
    return;
  }
}

int Server::findMethodIndex(const char * method_string)
{
  int method_index = -1;
  int method_id = atoi(method_string);
  char zero_str[constants::zero_constant_string.length()+1];
  constants::zero_constant_string.copy(zero_str);
  if (strcmp(method_string,zero_str) == 0)
  {
    method_index = 0;
    response_.write(constants::id_constant_string,0);
  }
  else if (method_id > 0)
  {
    method_index = method_id;
    response_.write(constants::id_constant_string,method_id);
  }
  else
  {
    method_index = findFunctionIndex(method_string);
    if (method_index >= 0)
    {
      response_.write(constants::id_constant_string,method_string);
      return method_index;
    }
    method_index = findCallbackIndex(method_string);
    if (method_index >= 0)
    {
      response_.write(constants::id_constant_string,method_string);
      method_index += functions_.size();
      return method_index;
    }
    method_index = findPropertyIndex(method_string);
    if (method_index >= 0)
    {
      response_.write(constants::id_constant_string,method_string);
      method_index += functions_.size() + callbacks_.size();
      return method_index;
    }
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

int Server::processParameterString(Function & function, const char * parameter_string)
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
    parameter_index = findFunctionParameterIndex(function,parameter_string);
  }
  Array<Parameter *,constants::FUNCTION_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
  parameter_ptrs_ptr = &function.parameter_ptrs_;
  if ((parameter_index < 0) || (parameter_index >= (int)parameter_ptrs_ptr->size()))
  {
    response_.returnParameterNotFoundError();
    parameter_index = -1;
  }
  return parameter_index;
}

bool Server::checkParameters(Function & function, ArduinoJson::JsonArray::iterator iterator)
{
  int parameter_index = 0;
  for (ArduinoJson::JsonArray::iterator it=iterator;
       it!=request_json_array_ptr_->end();
       ++it)
  {
    Parameter * parameter_ptr = NULL;
    parameter_ptr = function.parameter_ptrs_[parameter_index];
    if (checkParameter(*parameter_ptr,*it))
    {
      parameter_index++;
    }
    else
    {
      return false;
    }
  }
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
  property.updateFunctionsAndParameters();

  parameterHelp(property.parameter(),false);

  response_.writeKey(constants::value_constant_string);
  writePropertyToResponse(property,false,false);

  response_.writeKey(constants::default_value_constant_string);
  writePropertyToResponse(property,false,true);

  response_.writeKey(constants::functions_constant_string);
  response_.beginArray();
  for (size_t i=0; i<Property::functions_.size(); ++i)
  {
    if (verbose)
    {
      functionHelp(Property::functions_[i],false);
    }
    else
    {
      response_.write(Property::functions_[i].getName());
    }
  }
  response_.endArray();

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
  response_.endArray();

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

void Server::functionHelp(Function & function, bool verbose)
{
  response_.beginObject();

  const ConstantString & function_name = function.getName();
  response_.write(constants::name_constant_string,function_name);
  const ConstantString & firmware_name = function.getFirmwareName();
  response_.write(constants::firmware_constant_string,firmware_name);

  response_.writeKey(constants::parameters_constant_string);
  response_.beginArray();
  Array<Parameter *,constants::FUNCTION_PARAMETER_COUNT_MAX> * parameter_ptrs_ptr = NULL;
  parameter_ptrs_ptr = &function.parameter_ptrs_;
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

  response_.write(constants::result_type_constant_string,function.getReturnType());

  response_.endObject();
}

void Server::callbackHelp(Callback & callback, bool verbose)
{
  callback.updateFunctionsAndParameters();

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

  response_.writeKey(constants::interrupts_constant_string);
  response_.beginArray();
  IndexedContainer<Interrupt *,constants::CALLBACK_INTERRUPT_COUNT_MAX> * interrupt_ptrs_ptr = NULL;
  interrupt_ptrs_ptr = &callback.interrupt_ptrs_;
  for (size_t i=0; i<interrupt_ptrs_ptr->max_size(); ++i)
  {
    if (interrupt_ptrs_ptr->indexHasValue(i))
    {
      if (verbose)
      {
        interruptHelp(*((*interrupt_ptrs_ptr)[i]),true);
      }
      else
      {
        const ConstantString & interrupt_name = (*interrupt_ptrs_ptr)[i]->getName();
        response_.write(interrupt_name);
      }
    }
  }
  response_.endArray();

  response_.writeKey(constants::functions_constant_string);
  response_.beginArray();
  for (size_t i=0; i<Callback::functions_.size(); ++i)
  {
    if (verbose)
    {
      functionHelp(Callback::functions_[i],false);
    }
    else
    {
      response_.write(Callback::functions_[i].getName());
    }
  }
  response_.endArray();

  response_.writeKey(constants::parameters_constant_string);
  response_.beginArray();
  for (size_t i=0; i<Callback::parameters_.size(); ++i)
  {
    if (verbose)
    {
      parameterHelp(Callback::parameters_[i]);
    }
    else
    {
      response_.write(Callback::parameters_[i].getName());
    }
  }
  response_.endArray();

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
  // ? function
  // ? parameter
  // ? property
  // ? callback
  // ?? function
  // ?? parameter
  // ?? property
  // ?? callback
  else if (parameter_count == 1)
  {
    const char * param_string = (*request_json_array_ptr_)[1];
    int function_index = findFunctionIndex(param_string);
    if ((function_index >= 0) && (function_index < (int)functions_.size()))
    {
      // ? function
      param_error = false;
      response_.writeResultKey();
      functionHelp(functions_[function_index],verbose);
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
  // ? function parameter
  // ?? function parameter
  // ? property function
  // ?? property function
  else if (parameter_count == 2)
  {
    const char * method_string = (*request_json_array_ptr_)[1];
    int function_index = findFunctionIndex(method_string);
    if (function_index >= 0)
    {
      Function & function = functions_[function_index];

      int parameter_index = processParameterString(function,(*request_json_array_ptr_)[2]);
      if (parameter_index >= 0)
      {
        param_error = false;
        Parameter * parameter_ptr;
        parameter_ptr = function.parameter_ptrs_[parameter_index];
        response_.writeResultKey();
        parameterHelp(*parameter_ptr);
      }
    }
    else
    {
      int property_index = findPropertyIndex(method_string);
      if (property_index >= 0)
      {
        Property & property = properties_[property_index];
        property.updateFunctionsAndParameters();
        int property_function_index = property.findFunctionIndex((const char *)(*request_json_array_ptr_)[2]);
        if (property_function_index >= 0)
        {
          param_error = false;
          Function & function = property.functions_[property_function_index];

          response_.writeResultKey();
          functionHelp(function,verbose);
        }
        else
        {
          response_.returnPropertyFunctionNotFoundError();
          return;
        }
      }
      else
      {
        response_.returnMethodNotFoundError();
        return;
      }
    }
  }
  // ? property function parameter
  // ?? property function parameter
  else if (parameter_count == 3)
  {
    const char * method_string = (*request_json_array_ptr_)[1];
    int property_index = findPropertyIndex(method_string);
    if (property_index >= 0)
    {
      Property & property = properties_[property_index];
      property.updateFunctionsAndParameters();
      int property_function_index = property.findFunctionIndex((const char *)(*request_json_array_ptr_)[2]);
      if (property_function_index >= 0)
      {
        Function & function = property.functions_[property_function_index];
        int parameter_index = processParameterString(function,(*request_json_array_ptr_)[3]);
        if (parameter_index >= 0)
        {
          param_error = false;
          Parameter * parameter_ptr;
          parameter_ptr = function.parameter_ptrs_[parameter_index];
          response_.writeResultKey();
          parameterHelp(*parameter_ptr);
        }
      }
      else
      {
        response_.returnPropertyFunctionNotFoundError();
        return;
      }
    }
    else
    {
      response_.returnMethodNotFoundError();
      return;
    }
  }
  // ? unknown
  // ?? unknown
  // ? function unknown
  // ?? function unknown
  // ? property unknown
  // ?? property unknown
  // ? property function unknown
  // ?? property function unknown
  if (param_error)
  {
    response_.returnParameterInvalidError(constants::empty_constant_string);
    return;
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
  response_.write(constants::serial_number_constant_string,getSerialNumber());

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

    response_.writeKey(constants::interrupts_constant_string);
    response_.beginArray();
    const Vector<Interrupt> & interrupts = interrupts_.subVector(i);
    for (size_t j=0; j<interrupts.size(); ++j)
    {
      interruptHelp(interrupts[j],false);
    }
    response_.endArray();

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

void Server::interruptHelp(Interrupt & interrupt, bool verbose)
{
  if (response_.error())
  {
    return;
  }
  if (verbose)
  {
    response_.beginObject();

    response_.write(constants::name_constant_string,interrupt.getName());
    response_.write(constants::number_constant_string,interrupt.getNumber());
    response_.write(constants::pin_constant_string,interrupt.getPin());
    Callback * callback_ptr = interrupt.getCallbackPtr();
    if (callback_ptr != NULL)
    {
      response_.write(constants::callback_constant_string,callback_ptr->getName());
    }
    else
    {
      response_.writeNull(constants::callback_constant_string);
    }
    response_.write(constants::mode_constant_string,interrupt.getMode());

    response_.endObject();
  }
  else
  {
    response_.write(interrupt.getName());
  }
}

void Server::writeInterruptInfoToResponse()
{
  if (response_.error())
  {
    return;
  }
  response_.beginArray();
  for (size_t i=0; i<interrupts_.size(); ++i)
  {
    interruptHelp(interrupts_[i],true);
  }
  response_.endArray();
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
    response_.writeKey(constants::functions_constant_string);
    response_.beginArray();
    for (size_t function_index=0; function_index<functions_.size(); ++function_index)
    {
      if (function_index > private_function_index_)
      {
        Function & function = functions_[function_index];
        if (function.firmwareNameInArray(firmware_name_array))
        {
          const ConstantString & function_name = function.getName();
          response_.write(function_name);
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
    response_.writeKey(constants::functions_constant_string);
    response_.beginArray();
    for (size_t function_index=0; function_index<functions_.size(); ++function_index)
    {
      if (function_index > private_function_index_)
      {
        functionHelp(functions_[function_index],false);
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
void Server::getMethodIdsHandler()
{
  response_.writeResultKey();
  response_.beginObject();
  size_t method_index;
  for (size_t function_index=0; function_index<functions_.size(); ++function_index)
  {
    if (function_index > private_function_index_)
    {
      const ConstantString & function_name = functions_[function_index].getName();
      method_index = function_index;
      response_.write(function_name,method_index);
    }
  }
  for (size_t callback_index=0; callback_index<callbacks_.size(); ++callback_index)
  {
    const ConstantString & callback_name = callbacks_[callback_index].getName();
    method_index = callback_index + functions_.size();
    response_.write(callback_name,method_index);
  }
  for (size_t property_index=0; property_index<properties_.size(); ++property_index)
  {
    const ConstantString & property_name = properties_[property_index].getName();
    method_index = property_index + functions_.size() + callbacks_.size();
    response_.write(property_name,method_index);
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

void Server::getInterruptInfoHandler()
{
  response_.writeResultKey();
  writeInterruptInfoToResponse();
}

void Server::detachAllInterruptsHandler()
{
  for (size_t callback_index=0; callback_index<callbacks_.size(); ++callback_index)
  {
    callbacks_[callback_index].detachFromAll();
  }
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

void Server::getPropertyValuesHandler()
{
  response_.writeResultKey();
  response_.beginObject();
  for (size_t i=0; i<properties_.size(); ++i)
  {
    Property & property = properties_[i];
    writePropertyToResponse(property,true,false);
  }
  response_.endObject();
}

void Server::setPropertiesToDefaultsHandler()
{
  setPropertiesToDefaults();
}

}
