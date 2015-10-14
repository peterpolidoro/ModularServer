// ----------------------------------------------------------------------------
// Server.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Server.h"


using namespace ArduinoJson;

namespace ModularDevice
{

CONSTANT_STRING(default_device_name,"");
CONSTANT_STRING(get_device_info_method_name,"getDeviceInfo");
CONSTANT_STRING(get_method_ids_method_name,"getMethodIds");
CONSTANT_STRING(get_response_codes_method_name,"getResponseCodes");
CONSTANT_STRING(get_parameters_method_name,"getParameters");
CONSTANT_STRING(help_method_name,"?");
CONSTANT_STRING(verbose_help_method_name,"??");
CONSTANT_STRING(object_request_error_message,"JSON object requests not supported. Must use compact JSON array format for requests.");
CONSTANT_STRING(array_parse_error_message,"Parsing JSON array request failed! Could be invalid JSON or too many tokens.");
CONSTANT_STRING(eeprom_initialized_saved_variable_name,"eeprom_initialized");
CONSTANT_STRING(serial_number_saved_variable_name,"serial_number");
CONSTANT_STRING(parameter_error_preamble_message,"Parameter value out of range: ");
CONSTANT_STRING(array_parameter_error_preamble_message,"Array parameter element value out of range: ");


Server::Server(GenericSerial &serial) :
  response_(serial)
{
  setSerial(serial);
  setName(default_device_name);
  model_number_ = 0;
  firmware_number_ = 0;
  request_method_index_ = -1;
  parameter_count_ = 0;
  error_ = false;

  Method& get_device_info_method = createMethod(get_device_info_method_name);
  get_device_info_method.attachReservedCallback(&Server::getDeviceInfoCallback);

  Method& get_method_ids_method = createMethod(get_method_ids_method_name);
  get_method_ids_method.attachReservedCallback(&Server::getMethodIdsCallback);

  Method& get_response_codes_method = createMethod(get_response_codes_method_name);
  get_response_codes_method.attachReservedCallback(&Server::getResponseCodesCallback);

  Method& get_parameters_method = createMethod(get_parameters_method_name);
  get_parameters_method.attachReservedCallback(&Server::getParametersCallback);

  Method& help_method = createMethod(help_method_name);
  help_method.attachReservedCallback(&Server::help);

  Method& verbose_help_method = createMethod(verbose_help_method_name);
  verbose_help_method.attachReservedCallback(&Server::verboseHelp);

  eeprom_index_ = 0;
  eeprom_initialized_index_ = eeprom_index_;
  eeprom_init_name_ptr_ = &eeprom_initialized_saved_variable_name;
  eeprom_uninitialized_ = true;
  saved_variable_array_.push_back(SavedVariable(*eeprom_init_name_ptr_,
                                                eeprom_index_,
                                                constants::eeprom_initialized_value));
  eeprom_index_ += sizeof(constants::eeprom_initialized_value);
  createSavedVariable(serial_number_saved_variable_name,constants::serial_number_default);
}

void Server::setSerial(GenericSerial &serial)
{
  generic_serial_ = serial;
}

void Server::setName(const ConstantString &name)
{
  name_ptr_ = &name;
}

void Server::setModelNumber(const unsigned int model_number)
{
  model_number_ = model_number;
}

void Server::setSerialNumber(const unsigned int serial_number)
{
  setSavedVariableValue(serial_number_saved_variable_name,serial_number);
}

void Server::setFirmwareNumber(const unsigned int firmware_number)
{
  firmware_number_ = firmware_number;
}

Method& Server::createMethod(const ConstantString &method_name)
{
  int method_index = findMethodIndex(method_name);
  if (method_index < 0)
  {
    method_array_.push_back(Method(method_name));
    return method_array_.back();
  }
  else
  {
    method_array_[method_index] = Method(method_name);
    return method_array_[method_index];
  }
}

Method& Server::copyMethod(Method method,const ConstantString &method_name)
{
  method_array_.push_back(method);
  method_array_.back().setName(method_name);
  return method_array_.back();
}

Parameter& Server::createParameter(const ConstantString &parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if (parameter_index < 0)
  {
    parameter_array_.push_back(Parameter(parameter_name));
    return parameter_array_.back();
  }
  else
  {
    parameter_array_[parameter_index] = Parameter(parameter_name);
    return parameter_array_[parameter_index];
  }
}

Parameter& Server::copyParameter(Parameter parameter,const ConstantString &parameter_name)
{
  parameter_array_.push_back(parameter);
  parameter_array_.back().setName(parameter_name);
  return parameter_array_.back();
}

Parser::JsonValue Server::getParameterValue(const ConstantString &name)
{
  int parameter_index = findParameterIndex(name);
  return request_json_array_[parameter_index+1];
}

void Server::addNullToResponse(const char *key)
{
  response_.addNull(key);
}

void Server::addNullToResponse()
{
  response_.addNull();
}

void Server::addBoolToResponse(const char *key, const bool value)
{
  response_.addBool(key,value);
}

void Server::addBoolToResponse(const bool value)
{
  response_.addBool(value);
}

void Server::addKeyToResponse(const char *key)
{
  response_.addKey(key);
}

void Server::startResponseObject()
{
  response_.startObject();
}

void Server::stopResponseObject()
{
  response_.stopObject();
}

void Server::startResponseArray()
{
  response_.startArray();
}

void Server::stopResponseArray()
{
  response_.stopArray();
}

void Server::resetDefaults()
{
  for (unsigned int i=0; i<saved_variable_array_.size(); ++i)
  {
    saved_variable_array_[i].setDefaultValue();
  }
}

void Server::startServer(const int baudrate)
{
  if (eeprom_uninitialized_)
  {
    initializeEeprom();
  }
  generic_serial_.getSerial().begin(baudrate);
}

void Server::handleRequest()
{
  while (generic_serial_.getSerial().available() > 0)
  {
    int request_length = generic_serial_.getSerial().readBytesUntil(constants::eol,request_,constants::STRING_LENGTH_REQUEST);
    if (request_length == 0)
    {
      continue;
    }
    request_[request_length] = '\0';
    error_ = false;
    response_.startObject();
    response_.setCompactPrint();
    if (request_[0] == constants::start_char_json_object)
    {
      addToResponse("status",constants::ERROR);
      char error_message[constants::STRING_LENGTH_ERROR] = {0};
      object_request_error_message.copy(error_message);
      addToResponse("error_message",error_message);
      error_ = true;
    }
    else
    {
      if (request_[0] != constants::start_char_json_array)
      {
        response_.setPrettyPrint();
        String request_string = String("[") + String(request_) + String("]");
        request_string.toCharArray(request_,constants::STRING_LENGTH_REQUEST);
      }
      request_json_array_ = parser_.parse(request_);
      if (request_json_array_.success())
      {
        processRequestArray();
      }
      else
      {
        addToResponse("status",constants::ERROR);
        char error_message[constants::STRING_LENGTH_ERROR] = {0};
        array_parse_error_message.copy(error_message);
        addToResponse("error_message",error_message);
        addToResponse("received_request",request_);
        error_ = true;
      }
      if (!error_)
      {
        addToResponse("status",constants::SUCCESS);
      }
    }
    response_.stopObject();
    generic_serial_.getSerial() << endl;
  }
}

void Server::processRequestArray()
{
  char* method_string = request_json_array_[0];
  request_method_index_ = processMethodString(method_string);
  if (!(request_method_index_ < 0))
  {
    int array_elements_count = countJsonArrayElements(request_json_array_);
    int parameter_count = array_elements_count - 1;
    if ((parameter_count == 1) && (String((char*)request_json_array_[1]).equals("?")))
    {
      methodHelp(request_method_index_);
    }
    else if ((parameter_count == 1) && (String((char*)request_json_array_[1]).equals("??")))
    {
      verboseMethodHelp(request_method_index_);
    }
    else if ((parameter_count == 2) && (String((char*)request_json_array_[2]).equals("?")))
    {
      int parameter_index = processParameterString(request_json_array_[1]);
      Parameter& parameter = *(method_array_[request_method_index_].parameter_ptr_array_[parameter_index]);
      parameterHelp(parameter);
    }
    else if (parameter_count != method_array_[request_method_index_].parameter_count_)
    {
      addToResponse("status",constants::ERROR);
      String error_request = String("Incorrect number of parameters. ");
      error_request += String(parameter_count) + String(" given. ");
      error_request += String(method_array_[request_method_index_].parameter_count_);
      error_request += String(" needed.");
      char error_str[constants::STRING_LENGTH_ERROR];
      error_request.toCharArray(error_str,constants::STRING_LENGTH_ERROR);
      addToResponse("error_message",error_str);
      error_ = true;
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

int Server::processMethodString(char *method_string)
{
  int method_index = -1;
  int method_id = String(method_string).toInt();
  if (String(method_string).compareTo("0") == 0)
  {
    method_index = 0;
    addToResponse("method_id",0);
  }
  else if (method_id > 0)
  {
    method_index = method_id;
    addToResponse("method_id",method_id);
  }
  else
  {
    method_index = findMethodIndex(method_string);
    addToResponse("method",method_string);
  }
  if ((method_index < 0) || (method_index >= (int)method_array_.size()))
  {
    addToResponse("status",constants::ERROR);
    addToResponse("error_message","Unknown method.");
    error_ = true;
    method_index = -1;
  }
  return method_index;
}

int Server::findMethodIndex(char *method_name)
{
  int method_index = -1;
  for (unsigned int i=0; i<method_array_.size(); ++i)
  {
    if (method_array_[i].compareName(method_name))
    {
      method_index = i;
      break;
    }
  }
  return method_index;
}

int Server::findMethodIndex(const ConstantString &method_name)
{
  int method_index = -1;
  for (unsigned int i=0; i<method_array_.size(); ++i)
  {
    if (method_array_[i].compareName(method_name))
    {
      method_index = i;
      break;
    }
  }
  return method_index;
}

int Server::countJsonArrayElements(Parser::JsonArray &json_array)
{
  int elements_count = 0;
  for (Parser::JsonArrayIterator array_it=json_array.begin();
       array_it!=json_array.end();
       ++array_it)
  {
    elements_count++;
  }
  return elements_count;
}

void Server::executeMethod()
{
  if (method_array_[request_method_index_].isReserved())
  {
    method_array_[request_method_index_].reservedCallback(this);
  }
  else
  {
    method_array_[request_method_index_].callback();
  }
}

void Server::methodHelp(int method_index)
{
  addKeyToResponse("parameters");
  response_.startArray();
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>& parameter_ptr_array = method_array_[method_index].parameter_ptr_array_;
  const ConstantString* parameter_name_ptr;
  char parameter_name_char_array[constants::STRING_LENGTH_PARAMETER_NAME];
  for (unsigned int i=0; i<parameter_ptr_array.size(); ++i)
  {
    parameter_name_ptr = parameter_ptr_array[i]->getNamePointer();
    parameter_name_ptr->copy(parameter_name_char_array);
    addToResponse(parameter_name_char_array);
  }
  response_.stopArray();
}

void Server::verboseMethodHelp(int method_index)
{
  addKeyToResponse("parameters");
  response_.startArray();
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>& parameter_ptr_array = method_array_[method_index].parameter_ptr_array_;
  for (unsigned int i=0; i<parameter_ptr_array.size(); ++i)
  {
    startResponseObject();
    parameterHelp(*(parameter_ptr_array[i]));
    stopResponseObject();
  }
  response_.stopArray();
}

int Server::processParameterString(char *parameter_string)
{
  int parameter_index = -1;
  int parameter_id = String(parameter_string).toInt();
  if (String(parameter_string).compareTo("0") == 0)
  {
    parameter_index = 0;
  }
  else if (parameter_id > 0)
  {
    parameter_index = parameter_id;
  }
  else
  {
    parameter_index = findParameterIndex(parameter_string);
  }
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>& parameter_ptr_array = method_array_[request_method_index_].parameter_ptr_array_;
  if ((parameter_index < 0) || (parameter_index >= (int)parameter_ptr_array.size()))
  {
    addToResponse("status",constants::ERROR);
    addToResponse("error_message","Unknown parameter.");
    error_ = true;
    parameter_index = -1;
  }
  return parameter_index;
}

int Server::findParameterIndex(const char *parameter_name)
{
  int parameter_index = -1;
  if (request_method_index_ >= 0)
  {
    Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>& parameter_ptr_array = method_array_[request_method_index_].parameter_ptr_array_;
    for (unsigned int i=0; i<parameter_ptr_array.size(); ++i)
    {
      if (parameter_ptr_array[i]->compareName(parameter_name))
      {
        parameter_index = i;
        break;
      }
    }
  }
  return parameter_index;
}

int Server::findParameterIndex(const ConstantString &parameter_name)
{
  int parameter_index = -1;
  if (request_method_index_ >= 0)
  {
    Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>& parameter_ptr_array = method_array_[request_method_index_].parameter_ptr_array_;
    for (unsigned int i=0; i<parameter_ptr_array.size(); ++i)
    {
      if (parameter_ptr_array[i]->compareName(parameter_name))
      {
        parameter_index = i;
        break;
      }
    }
  }
  return parameter_index;
}

void Server::parameterHelp(Parameter &parameter)
{
  char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME] = {0};
  const ConstantString* parameter_name_ptr = parameter.getNamePointer();
  parameter_name_ptr->copy(parameter_name);
  addKeyToResponse(parameter_name);

  startResponseObject();

  char parameter_units[constants::STRING_LENGTH_PARAMETER_UNITS] = {0};
  const ConstantString* parameter_units_ptr = parameter.getUnitsPointer();
  parameter_units_ptr->copy(parameter_units);
  if (strcmp(parameter_units,"") != 0)
  {
    addToResponse("units",parameter_units);
  }
  constants::ParameterType type = parameter.getType();
  switch (type)
  {
    case constants::LONG_PARAMETER:
      {
        addToResponse("type","long");
        if (parameter.rangeIsSet())
        {
          long min = parameter.getMin().l;
          long max = parameter.getMax().l;
          addToResponse("min",min);
          addToResponse("max",max);
        }
        break;
      }
    case constants::DOUBLE_PARAMETER:
      {
        addToResponse("type","double");
        if (parameter.rangeIsSet())
        {
          double min = parameter.getMin().d;
          double max = parameter.getMax().d;
          addToResponse("min",min);
          addToResponse("max",max);
        }
        break;
      }
    case constants::BOOL_PARAMETER:
      {
        addToResponse("type","bool");
        break;
      }
    case constants::STRING_PARAMETER:
      {
        addToResponse("type","string");
        break;
      }
    case constants::OBJECT_PARAMETER:
      {
        addToResponse("type","object");
        break;
      }
    case constants::ARRAY_PARAMETER:
      {
        addToResponse("type","array");
        constants::ParameterType array_element_type = parameter.getArrayElementType();
        switch (array_element_type)
        {
          case constants::LONG_PARAMETER:
            {
              addToResponse("array_element_type","long");
              if (parameter.rangeIsSet())
              {
                long min = parameter.getMin().l;
                long max = parameter.getMax().l;
                addToResponse("min",min);
                addToResponse("max",max);
              }
              break;
            }
          case constants::DOUBLE_PARAMETER:
            {
              addToResponse("array_element_type","double");
              if (parameter.rangeIsSet())
              {
                double min = parameter.getMin().d;
                double max = parameter.getMax().d;
                addToResponse("min",min);
                addToResponse("max",max);
              }
              break;
            }
          case constants::BOOL_PARAMETER:
            {
              addToResponse("array_element_type","bool");
              break;
            }
          case constants::STRING_PARAMETER:
            {
              addToResponse("array_element_type","string");
              break;
            }
          case constants::OBJECT_PARAMETER:
            {
              addToResponse("array_element_type","object");
              break;
            }
          case constants::ARRAY_PARAMETER:
            {
              addToResponse("array_element_type","array");
              break;
            }
        }
        break;
      }
  }
  stopResponseObject();
}

bool Server::checkParameters()
{
  int parameter_index = 0;
  for (Parser::JsonArrayIterator request_it=request_json_array_.begin();
       request_it!=request_json_array_.end();
       ++request_it)
  {
    if (request_it!=request_json_array_.begin())
    {
      if (checkParameter(parameter_index,*request_it))
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

bool Server::checkParameter(int parameter_index, Parser::JsonValue json_value)
{
  bool out_of_range = false;
  bool object_parse_unsuccessful = false;
  bool array_parse_unsuccessful = false;
  Parameter& parameter = *(method_array_[request_method_index_].parameter_ptr_array_[parameter_index]);
  constants::ParameterType type = parameter.getType();
  String min_string = "";
  String max_string = "";
  switch (type)
  {
    case constants::LONG_PARAMETER:
      {
        if (parameter.rangeIsSet())
        {
          long value = (long)json_value;
          long min = parameter.getMin().l;
          long max = parameter.getMax().l;
          if ((value < min) || (value > max))
          {
            out_of_range = true;
            min_string = String(min);
            max_string = String(max);
          }
        }
        break;
      }
    case constants::DOUBLE_PARAMETER:
      {
        if (parameter.rangeIsSet())
        {
          double value = (double)json_value;
          double min = parameter.getMin().d;
          double max = parameter.getMax().d;
          if ((value < min) || (value > max))
          {
            out_of_range = true;
            char temp_string[constants::STRING_LENGTH_DOUBLE];
            dtostrf(min,constants::double_digits,constants::double_digits,temp_string);
            min_string = String(temp_string);
            dtostrf(max,constants::double_digits,constants::double_digits,temp_string);
            max_string = String(temp_string);
          }
        }
        break;
      }
    case constants::BOOL_PARAMETER:
      break;
    case constants::STRING_PARAMETER:
      break;
    case constants::OBJECT_PARAMETER:
      {
        Parser::JsonObject json_object = json_value;
        if (!json_object.success())
        {
          object_parse_unsuccessful = true;
        }
        break;
      }
    case constants::ARRAY_PARAMETER:
      {
        Parser::JsonArray json_array = json_value;
        if (!json_array.success())
        {
          array_parse_unsuccessful = true;
        }
        else
        {
          constants::ParameterType array_element_type = parameter.getArrayElementType();
          switch (array_element_type)
          {
            case constants::LONG_PARAMETER:
              {
                if (parameter.rangeIsSet())
                {
                  long value;
                  long min = parameter.getMin().l;
                  long max = parameter.getMax().l;
                  for (Parser::JsonArrayIterator i=json_array.begin();
                       i!=json_array.end();
                       ++i)
                  {
                    value = (long)*i;
                    if ((value < min) || (value > max))
                    {
                      out_of_range = true;
                      min_string = String(min);
                      max_string = String(max);
                      break;
                    }
                  }
                }
                break;
              }
            case constants::DOUBLE_PARAMETER:
              {
                if (parameter.rangeIsSet())
                {
                  double value;
                  double min = parameter.getMin().d;
                  double max = parameter.getMax().d;
                  for (Parser::JsonArrayIterator i=json_array.begin();
                       i!=json_array.end();
                       ++i)
                  {
                    value = (double)*i;
                    if ((value < min) || (value > max))
                    {
                      out_of_range = true;
                      char temp_string[constants::STRING_LENGTH_DOUBLE];
                      dtostrf(min,constants::double_digits,constants::double_digits,temp_string);
                      min_string = String(temp_string);
                      dtostrf(max,constants::double_digits,constants::double_digits,temp_string);
                      max_string = String(temp_string);
                      break;
                    }
                  }
                }
                break;
              }
            case constants::BOOL_PARAMETER:
              {
                break;
              }
            case constants::STRING_PARAMETER:
              {
                break;
              }
            case constants::OBJECT_PARAMETER:
              {
                break;
              }
            case constants::ARRAY_PARAMETER:
              {
                break;
              }
          }
        }
        break;
      }
  }
  if (out_of_range)
  {
    addToResponse("status",constants::ERROR);
    String error;
    char error_preamble[constants::STRING_LENGTH_ERROR_PREAMBLE] = {0};
    if (type != constants::ARRAY_PARAMETER)
    {
      parameter_error_preamble_message.copy(error_preamble);
      error = String(error_preamble);
    }
    else
    {
      array_parameter_error_preamble_message.copy(error_preamble);
      error = String(error_preamble);
    }
    error += min_string;
    error += String(" <= ");
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME] = {0};
    const ConstantString* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    error += String(parameter_name);
    if (type == constants::ARRAY_PARAMETER)
    {
      error += String(" element");
    }
    error += String(" <= ");
    error += max_string;
    char error_str[constants::STRING_LENGTH_ERROR];
    error.toCharArray(error_str,constants::STRING_LENGTH_ERROR);
    addToResponse("error_message",error_str);
    error_ = true;
  }
  else if (object_parse_unsuccessful)
  {
    addToResponse("status",constants::ERROR);
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME] = {0};
    const ConstantString* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    String error = String(parameter_name);
    error += String(" is not a valid JSON object.");
    char error_str[constants::STRING_LENGTH_ERROR];
    error.toCharArray(error_str,constants::STRING_LENGTH_ERROR);
    addToResponse("error_message",error_str);
    error_ = true;
  }
  else if (array_parse_unsuccessful)
  {
    addToResponse("status",constants::ERROR);
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME] = {0};
    const ConstantString* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    String error = String(parameter_name);
    error += String(" is not a valid JSON array.");
    char error_str[constants::STRING_LENGTH_ERROR];
    error.toCharArray(error_str,constants::STRING_LENGTH_ERROR);
    addToResponse("error_message",error_str);
    error_ = true;
  }
  bool parameter_ok = (!out_of_range) && (!object_parse_unsuccessful) && (!array_parse_unsuccessful);
  return parameter_ok;
}

int Server::findSavedVariableIndex(const ConstantString &saved_variable_name)
{
  int saved_variable_index = -1;
  for (unsigned int i=0; i<saved_variable_array_.size(); ++i)
  {
    if (saved_variable_array_[i].compareName(saved_variable_name))
    {
      saved_variable_index = i;
      break;
    }
  }
  return saved_variable_index;
}

unsigned int Server::getSerialNumber()
{
  unsigned int serial_number;
  getSavedVariableValue(serial_number_saved_variable_name,serial_number);
  return serial_number;
}

void Server::initializeEeprom()
{
  saved_variable_array_[eeprom_initialized_index_].setValue(constants::eeprom_initialized_value);
  eeprom_uninitialized_ = false;
}

void Server::getDeviceInfoCallback()
{
  char device_name[constants::STRING_LENGTH_DEVICE_NAME] = {0};
  name_ptr_->copy(device_name);
  addToResponse("name",device_name);
  addToResponse("model_number",model_number_);
  addToResponse("serial_number",getSerialNumber());
  addToResponse("firmware_number",firmware_number_);
}

void Server::getMethodIdsCallback()
{
  char method_name[constants::STRING_LENGTH_METHOD_NAME] = {0};
  const ConstantString* method_name_ptr;
  for (unsigned int i=0; i<method_array_.size(); ++i)
  {
    int method_index = i;
    if (!method_array_[method_index].isReserved())
    {
      method_name_ptr = method_array_[i].getNamePointer();
      method_name_ptr->copy(method_name);
      int method_id = method_index;
      addToResponse(method_name,method_id);
    }
  }
}

void Server::getResponseCodesCallback()
{
  addToResponse("response_success",constants::SUCCESS);
  addToResponse("response_error",constants::ERROR);
}

void Server::getParametersCallback()
{
  addKeyToResponse("parameters");
  response_.startArray();
  for (unsigned int i=0; i<parameter_array_.size(); ++i)
  {
    startResponseObject();
    parameterHelp(parameter_array_[i]);
    stopResponseObject();
  }
  response_.stopArray();
}

void Server::help()
{
  addKeyToResponse("device_info");
  startResponseObject();
  getDeviceInfoCallback();
  stopResponseObject();

  addKeyToResponse("methods");
  startResponseArray();
  char method_name[constants::STRING_LENGTH_METHOD_NAME] = {0};
  const ConstantString* method_name_ptr;
  for (unsigned int i=0; i<method_array_.size(); ++i)
  {
    int method_index = i;
    if (!method_array_[method_index].isReserved())
    {
      method_name_ptr = method_array_[i].getNamePointer();
      method_name_ptr->copy(method_name);
      addToResponse(method_name);
    }
  }
  stopResponseArray();
}

void Server::verboseHelp()
{
  addKeyToResponse("device_info");
  startResponseObject();
  getDeviceInfoCallback();
  stopResponseObject();

  addKeyToResponse("methods");
  startResponseArray();
  char method_name[constants::STRING_LENGTH_METHOD_NAME] = {0};
  const ConstantString* method_name_ptr;
  for (unsigned int i=0; i<method_array_.size(); ++i)
  {
    int method_index = i;
    if (!method_array_[method_index].isReserved())
    {
      method_name_ptr = method_array_[i].getNamePointer();
      method_name_ptr->copy(method_name);
      startResponseObject();
      addKeyToResponse(method_name);
      startResponseObject();
      methodHelp(method_index);
      stopResponseObject();
      stopResponseObject();
    }
  }
  stopResponseArray();
}
}
