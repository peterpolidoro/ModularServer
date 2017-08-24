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
  response_.setJsonStream(server_json_stream_);

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

  Parameter & verbosity_parameter = createParameter(constants::verbosity_constant_string);
  verbosity_parameter.setTypeString();
  verbosity_parameter.setSubset(constants::verbosity_ptr_subset);

  // Functions
  Function & get_method_ids_function = createFunction(constants::get_method_ids_function_name);
  get_method_ids_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getMethodIdsHandler));
  get_method_ids_function.setResultTypeObject();
  private_function_index_ = 0;

  Function & help_function = createFunction(constants::help_function_name);
  help_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::helpHandler));
  help_function.setResultTypeObject();
  private_function_index_++;

  Function & verbose_help_function = createFunction(constants::verbose_help_function_name);
  verbose_help_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::verboseHelpHandler));
  verbose_help_function.setResultTypeObject();
  private_function_index_++;

  Function & get_device_id_function = createFunction(constants::get_device_id_function_name);
  get_device_id_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getDeviceIdHandler));
  get_device_id_function.setResultTypeObject();

  Function & get_device_info_function = createFunction(constants::get_device_info_function_name);
  get_device_info_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getDeviceInfoHandler));
  get_device_info_function.setResultTypeObject();

  Function & get_api_function = createFunction(constants::get_api_function_name);
  get_api_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getApiHandler));
  get_api_function.addParameter(verbosity_parameter);
  get_api_function.addParameter(firmware_parameter);
  get_api_function.setResultTypeObject();

  Function & get_property_default_values_function = createFunction(constants::get_property_default_values_function_name);
  get_property_default_values_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getPropertyDefaultValuesHandler));
  get_property_default_values_function.addParameter(firmware_parameter);
  get_property_default_values_function.setResultTypeObject();

  Function & set_properties_to_defaults_function = createFunction(constants::set_properties_to_defaults_function_name);
  set_properties_to_defaults_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::setPropertiesToDefaultsHandler));
  set_properties_to_defaults_function.addParameter(firmware_parameter);

  Function & get_property_values_function = createFunction(constants::get_property_values_function_name);
  get_property_values_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getPropertyValuesHandler));
  get_property_values_function.addParameter(firmware_parameter);
  get_property_values_function.setResultTypeObject();

  Function & get_interrupt_info_function = createFunction(constants::get_interrupt_info_function_name);
  get_interrupt_info_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getInterruptInfoHandler));
  get_interrupt_info_function.setResultTypeArray();
  get_interrupt_info_function.setResultTypeObject();

  Function & detach_all_interrupts_function = createFunction(constants::detach_all_interrupts_function_name);
  detach_all_interrupts_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::detachAllInterruptsHandler));

#ifdef __AVR__
  Function & get_memory_free_function = createFunction(constants::get_memory_free_function_name);
  get_memory_free_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&Server::getMemoryFreeHandler));
  get_memory_free_function.setResultTypeLong();
#endif

  // Callbacks
  Callback::interrupt_name_array_ptr_ = &interrupt_name_array_;
  Callback::find_interrupt_ptr_by_chars_functor_ = makeFunctor((Functor1wRet<const char *, Interrupt *> *)0,*this,&Server::findInterruptPtrByChars);
  Callback::find_interrupt_ptr_by_constant_string_functor_ = makeFunctor((Functor1wRet<const ConstantString &, Interrupt *> *)0,*this,&Server::findInterruptPtrByConstantString);
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
    const ConstantString * hardware_name_ptr = hardware_info_array_.back()->name_ptr;
    interrupts_.back().setHardwareName(*hardware_name_ptr);
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

Interrupt * Server::findInterruptPtrByChars(const char * interrupt_name)
{
  int interrupt_index = findInterruptIndex(interrupt_name);
  if ((interrupt_index >= 0) && (interrupt_index < (int)interrupts_.size()))
  {
    return &interrupts_[interrupt_index];
  }
  return NULL;
}

Interrupt * Server::findInterruptPtrByConstantString(const ConstantString & interrupt_name)
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

void Server::setPropertiesToDefaults(ArduinoJson::JsonArray & firmware_name_array)
{
  for (size_t i=0; i<properties_.size(); ++i)
  {
    Property & property = properties_[i];
    if (property.parameter().firmwareNameInArray(firmware_name_array))
    {
      property.setValueToDefault();
    }
  }
}

void Server::setAllPropertiesToDefaults()
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

Parameter & Server::copyParameter(Parameter parameter,
                                  const ConstantString & parameter_name)
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

Function & Server::copyFunction(Function function,
                                const ConstantString & function_name)
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
  if (server_running_ && (server_stream_ptrs_.size() > 0) && (server_json_stream_.available() > 0))
  {
    long bytes_read = server_json_stream_.readJsonIntoBuffer(request_);
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
    size_t array_elements_count = countJsonArrayElements((*request_json_array_ptr_));
    size_t parameter_count = (array_elements_count > 0) ? (array_elements_count - 1) : 0;
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
        function.writeApi(response_,false,true,false);
      }
      // function ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        function.writeApi(response_,false,true,true);
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
          Parameter & parameter = *(function.parameter_ptrs_[parameter_index]);
          response_.writeResultKey();
          parameter.writeApi(response_,false,false,true,true);
        }
      }
      // execute private function without checking parameters
      else if (request_method_index_ <= (int)private_function_index_)
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
        ++iterator;
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
        callback.writeApi(response_,false,true,false,false,true);
      }
      // callback ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        callback.writeApi(response_,false,true,true,true,true);
      }
      // check parameter count
      else if (parameter_count == 0)
      {
        // shortcut for callback call function
        callback.updateFunctionsAndParameters();
        Function & function = callback.function(callback::trigger_function_name);
        function.functor();
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

        size_t callback_parameter_count = (parameter_count > 0) ? (parameter_count - 1) : 0;

        // callback function ?
        if ((callback_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_str) == 0))
        {
          response_.writeResultKey();
          function.writeApi(response_,false,true,false);
        }
        // callback function ??
        else if ((callback_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_double_str) == 0))
        {
          response_.writeResultKey();
          function.writeApi(response_,false,true,true);
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
            Parameter & parameter = *(function.parameter_ptrs_[parameter_index]);
            response_.writeResultKey();
            parameter.writeApi(response_,false,false,true,true);
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
          ++iterator;
          // do not check callback method at begin + 1
          ++iterator;
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
        property.writeApi(response_,false,true,false,true);
      }
      // property ??
      else if ((parameter_count == 1) && (strcmp((*request_json_array_ptr_)[1],question_double_str) == 0))
      {
        response_.writeResultKey();
        property.writeApi(response_,false,true,true,true);
      }
      // check parameter count
      else if (parameter_count == 0)
      {
        // shortcut for property getValue function
        property.updateFunctionsAndParameters();
        Function & function = property.function(property::get_value_function_name);
        function.functor();
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

        size_t property_parameter_count = (parameter_count > 0) ? (parameter_count - 1) : 0;

        // property function ?
        if ((property_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_str) == 0))
        {
          response_.writeResultKey();
          function.writeApi(response_,false,true,false);
        }
        // property function ??
        else if ((property_parameter_count == 1) && (strcmp((*request_json_array_ptr_)[2],question_double_str) == 0))
        {
          response_.writeResultKey();
          function.writeApi(response_,false,true,true);
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
            Parameter & parameter = *(function.parameter_ptrs_[parameter_index]);
            response_.writeResultKey();
            parameter.writeApi(response_,false,false,true,true);
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
          ++iterator;
          // do not check property method at begin + 1
          ++iterator;
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

size_t Server::countJsonArrayElements(ArduinoJson::JsonArray & json_array)
{
  size_t elements_count = 0;
  for (ArduinoJson::JsonArray::iterator it=json_array.begin();
       it!=json_array.end();
       ++it)
  {
    ++elements_count;
  }
  return elements_count;
}

int Server::processParameterString(Function & function,
                                   const char * parameter_string)
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

bool Server::checkParameters(Function & function,
                             ArduinoJson::JsonArray::iterator iterator)
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

bool Server::checkParameter(Parameter & parameter,
                            ArduinoJson::JsonVariant & json_value)
{
  bool in_subset = true;
  bool in_range = true;
  bool array_length_in_range = true;
  bool array_elements_ok = true;
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
        long min = parameter.getRangeMin().l;
        long max = parameter.getRangeMax().l;
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
        double min = parameter.getRangeMin().d;
        double max = parameter.getRangeMax().d;
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
            array_elements_ok = false;
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
  bool parameter_ok = in_subset && in_range && array_length_in_range && array_elements_ok && (!object_parse_unsuccessful) && (!array_parse_unsuccessful);
  return parameter_ok;
}

bool Server::checkArrayParameterElement(Parameter & parameter,
                                        ArduinoJson::JsonVariant & json_value)
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
            long min = parameter.getRangeMin().l;
            long max = parameter.getRangeMax().l;
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
            double min = parameter.getRangeMin().d;
            double max = parameter.getRangeMax().d;
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
    setAllPropertiesToDefaults();
  }
  eeprom_initialized_ = true;
}

void Server::incrementServerStream()
{
  if (server_stream_ptrs_.size() > 0)
  {
    server_stream_index_ = (server_stream_index_ + 1) % server_stream_ptrs_.size();
    server_json_stream_.setStream(*server_stream_ptrs_[server_stream_index_]);
  }
}

void Server::help(bool verbose)
{
  if (response_.error())
  {
    return;
  }

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

    response_.writeKey(constants::api_constant_string);

    ArduinoJson::StaticJsonBuffer<constants::FIRMWARE_NAME_JSON_BUFFER_SIZE> json_buffer;
    ArduinoJson::JsonArray& firmware_name_array = json_buffer.createArray();

    if (verbose)
    {
      // Write ALL firmware API to response
      char all_str[constants::all_constant_string.length() + 1];
      all_str[0] = '\0';
      constants::all_constant_string.copy(all_str);
      firmware_name_array.add<char *>(all_str);
      writeApiToResponse(constants::verbosity_names,firmware_name_array);
    }
    else
    {
      // Write only the highest level firmware API to response
      constants::SubsetMemberType firmware_name = firmware_name_array_.back();
      char firmware_name_str[firmware_name.cs_ptr->length() + 1];
      firmware_name_str[0] = '\0';
      firmware_name.cs_ptr->copy(firmware_name_str);
      firmware_name_array.add<char *>(firmware_name_str);
      writeApiToResponse(constants::verbosity_names,firmware_name_array);
    }

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
      Function & function = functions_[function_index];
      function.writeApi(response_,false,true,verbose);
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
        Parameter & parameter = parameters_[parameter_index];
        parameter.writeApi(response_,false,false,true,true);
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
          Property & property = properties_[property_index];
          property.writeApi(response_,false,true,verbose,true);
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
            Callback & callback = callbacks_[callback_index];
            callback.writeApi(response_,false,true,verbose,verbose,true);
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
        Parameter & parameter = *(function.parameter_ptrs_[parameter_index]);
        response_.writeResultKey();
        parameter.writeApi(response_,false,false,true,true);
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
          function.writeApi(response_,false,true,verbose);
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
          Parameter & parameter = *(function.parameter_ptrs_[parameter_index]);
          response_.writeResultKey();
          parameter.writeApi(response_,false,false,true,true);
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

    Vector<Interrupt> & interrupts = interrupts_.subVector(i);
    if (interrupts.size() > 0)
    {
      response_.writeKey(constants::interrupts_constant_string);
      response_.beginArray();
      for (size_t j=0; j<interrupts.size(); ++j)
      {
        Interrupt & interrupt = interrupts[j];
        interrupt.writeApi(response_,true);
      }
      response_.endArray();
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

void Server::writeInterruptInfoToResponse()
{
  if (response_.error())
  {
    return;
  }

  response_.beginArray();
  for (size_t i=0; i<interrupts_.size(); ++i)
  {
    Interrupt & interrupt = interrupts_[i];
    interrupt.writeApi(response_,false);
  }
  response_.endArray();
}

void Server::writeApiToResponse(const ConstantString & verbosity,
                                ArduinoJson::JsonArray & firmware_name_array)
{
  if (response_.error())
  {
    return;
  }

  response_.beginObject();

  response_.write(constants::firmware_constant_string,&firmware_name_array);

  response_.write(constants::verbosity_constant_string,verbosity);

  bool write_names_only = false;
  bool write_instance_details = false;
  if (&verbosity == &constants::verbosity_names)
  {
    write_names_only = true;
  }
  else if (&verbosity == &constants::verbosity_detailed)
  {
    write_instance_details = true;
  }

  size_t functions_count = getFunctionsCount(firmware_name_array);
  if (functions_count > 0)
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
          function.writeApi(response_,write_names_only,false,false);
        }
      }
    }
    response_.endArray();
  }

  size_t parameters_count = getParametersCount(firmware_name_array);
  if (parameters_count > 0)
  {
    response_.writeKey(constants::parameters_constant_string);
    response_.beginArray();
    for (size_t parameter_index=0; parameter_index<parameters_.size(); ++parameter_index)
    {
      Parameter & parameter = parameters_[parameter_index];
      if (parameter.firmwareNameInArray(firmware_name_array))
      {
        parameter.writeApi(response_,write_names_only,false,false,write_instance_details);
      }
    }
    response_.endArray();
  }

  size_t properties_count = getPropertiesCount(firmware_name_array);
  if (properties_count > 0)
  {
    response_.writeKey(constants::properties_constant_string);
    response_.beginArray();
    for (size_t property_index=0; property_index<properties_.size(); ++property_index)
    {
      Property & property = properties_[property_index];
      if (property.firmwareNameInArray(firmware_name_array))
      {
        property.writeApi(response_,write_names_only,false,true,write_instance_details);
      }
    }
    response_.endArray();
  }

  size_t callbacks_count = getCallbacksCount(firmware_name_array);
  if (callbacks_count > 0)
  {
    response_.writeKey(constants::callbacks_constant_string);
    response_.beginArray();
    for (size_t callback_index=0; callback_index<callbacks_.size(); ++callback_index)
    {
      Callback & callback = callbacks_[callback_index];
      if (callback.firmwareNameInArray(firmware_name_array))
      {
        callback.writeApi(response_,write_names_only,false,true,false,write_instance_details);
      }
    }
    response_.endArray();
  }

  response_.endObject();
}

size_t Server::getPropertiesCount(ArduinoJson::JsonArray & firmware_name_array)
{
  size_t count = 0;
  for (size_t property_index=0; property_index<properties_.size(); ++property_index)
  {
    if (properties_[property_index].firmwareNameInArray(firmware_name_array))
    {
      ++count;
    }
  }
  return count;
}

size_t Server::getParametersCount(ArduinoJson::JsonArray & firmware_name_array)
{
  size_t count = 0;
  for (size_t property_index=0; property_index<parameters_.size(); ++property_index)
  {
    if (parameters_[property_index].firmwareNameInArray(firmware_name_array))
    {
      ++count;
    }
  }
  return count;
}

size_t Server::getFunctionsCount(ArduinoJson::JsonArray & firmware_name_array)
{
  size_t count = 0;
  for (size_t property_index=0; property_index<functions_.size(); ++property_index)
  {
    if (functions_[property_index].firmwareNameInArray(firmware_name_array))
    {
      ++count;
    }
  }
  return count;
}

size_t Server::getCallbacksCount(ArduinoJson::JsonArray & firmware_name_array)
{
  size_t count = 0;
  for (size_t property_index=0; property_index<callbacks_.size(); ++property_index)
  {
    if (callbacks_[property_index].firmwareNameInArray(firmware_name_array))
    {
      ++count;
    }
  }
  return count;
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
  const char * verbosity;
  parameter(constants::verbosity_constant_string).getValue(verbosity);

  ArduinoJson::JsonArray * firmware_name_array_ptr;
  parameter(constants::firmware_constant_string).getValue(firmware_name_array_ptr);
  response_.writeResultKey();

  if (verbosity == constants::verbosity_names)
  {
    writeApiToResponse(constants::verbosity_names,*firmware_name_array_ptr);
  }
  else if (verbosity == constants::verbosity_general)
  {
    writeApiToResponse(constants::verbosity_general,*firmware_name_array_ptr);
  }
  else if (verbosity == constants::verbosity_detailed)
  {
    writeApiToResponse(constants::verbosity_detailed,*firmware_name_array_ptr);
  }
}

#ifdef __AVR__
void Server::getMemoryFreeHandler()
{
  response_.returnResult(freeMemory());
}
#endif

void Server::getPropertyDefaultValuesHandler()
{
  ArduinoJson::JsonArray * firmware_name_array_ptr;
  parameter(constants::firmware_constant_string).getValue(firmware_name_array_ptr);

  response_.writeResultKey();
  response_.beginObject();
  for (size_t i=0; i<properties_.size(); ++i)
  {
    Property & property = properties_[i];
    if (property.parameter().firmwareNameInArray(*firmware_name_array_ptr))
    {
      property.writeValue(response_,true,true);
    }
  }
  response_.endObject();
}

void Server::getPropertyValuesHandler()
{
  ArduinoJson::JsonArray * firmware_name_array_ptr;
  parameter(constants::firmware_constant_string).getValue(firmware_name_array_ptr);

  response_.writeResultKey();
  response_.beginObject();
  for (size_t i=0; i<properties_.size(); ++i)
  {
    Property & property = properties_[i];
    if (property.parameter().firmwareNameInArray(*firmware_name_array_ptr))
    {
      property.writeValue(response_,true,false);
    }
  }
  response_.endObject();
}

void Server::setPropertiesToDefaultsHandler()
{
  ArduinoJson::JsonArray * firmware_name_array_ptr;
  parameter(constants::firmware_constant_string).getValue(firmware_name_array_ptr);

  setPropertiesToDefaults(*firmware_name_array_ptr);
}

}
