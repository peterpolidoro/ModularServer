// ----------------------------------------------------------------------------
// Server.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Server.h"


using namespace ArduinoJson;

namespace RemoteDevice
{

FLASH_STRING(default_device_name,"");
FLASH_STRING(get_device_info_method_name,"getDeviceInfo");
FLASH_STRING(get_method_ids_method_name,"getMethodIds");
FLASH_STRING(get_response_codes_method_name,"getResponseCodes");
FLASH_STRING(get_parameters_method_name,"getParameters");
FLASH_STRING(help_method_name,"?");
FLASH_STRING(verbose_help_method_name,"??");
FLASH_STRING(object_request_error_message,"JSON object requests not supported. Must use compact JSON array format for requests.");
FLASH_STRING(array_parse_error_message,"Parsing JSON array request failed! Could be invalid JSON or too many tokens.");
FLASH_STRING(eeprom_initialized_saved_variable_name,"eeprom_initialized");
FLASH_STRING(serial_number_saved_variable_name,"serial_number");

Server::Server(Stream &stream) :
  response_(stream)
{
  setStream(stream);
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
  saved_variable_vector_.push_back(SavedVariable(*eeprom_init_name_ptr_,
                                                 eeprom_index_,
                                                 EEPROM_INITIALIZED_VALUE));
  eeprom_index_ += sizeof(EEPROM_INITIALIZED_VALUE);
  createSavedVariable(serial_number_saved_variable_name,SERIAL_NUMBER_DEFAULT);
}

void Server::setStream(Stream &stream)
{
  stream_ptr_ = &stream;
}

void Server::setName(const _FLASH_STRING &name)
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

Method& Server::createMethod(const _FLASH_STRING &method_name)
{
  int method_index = findMethodIndex(method_name);
  if (method_index < 0)
  {
    method_vector_.push_back(Method(method_name));
    return method_vector_.back();
  }
  else
  {
    method_vector_[method_index] = Method(method_name);
    return method_vector_[method_index];
  }
}

Method& Server::copyMethod(Method method,const _FLASH_STRING &method_name)
{
  method_vector_.push_back(method);
  method_vector_.back().setName(method_name);
  return method_vector_.back();
}

Parameter& Server::createParameter(const _FLASH_STRING &parameter_name)
{
  int parameter_index = findParameterIndex(parameter_name);
  if (parameter_index < 0)
  {
    parameter_vector_.push_back(Parameter(parameter_name));
    return parameter_vector_.back();
  }
  else
  {
    parameter_vector_[parameter_index] = Parameter(parameter_name);
    return parameter_vector_[parameter_index];
  }
}

Parameter& Server::copyParameter(Parameter parameter,const _FLASH_STRING &parameter_name)
{
  parameter_vector_.push_back(parameter);
  parameter_vector_.back().setName(parameter_name);
  return parameter_vector_.back();
}

Parser::JsonValue Server::getParameterValue(const _FLASH_STRING &name)
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

void Server::addBooleanToResponse(const char *key, const boolean value)
{
  response_.addBoolean(key,value);
}

void Server::addBooleanToResponse(const boolean value)
{
  response_.addBoolean(value);
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
  for (std::vector<SavedVariable>::iterator saved_variable_it = saved_variable_vector_.begin();
       saved_variable_it != saved_variable_vector_.end();
       ++saved_variable_it)
  {
    saved_variable_it->setDefaultValue();
  }
}

void Server::startServer(const int baudrate)
{
  if (eeprom_uninitialized_)
  {
    initializeEeprom();
  }
  Serial.begin(baudrate);
}

void Server::handleRequest()
{
  while (stream_ptr_->available() > 0)
  {
    int request_length = stream_ptr_->readBytesUntil(EOL,request_,STRING_LENGTH_REQUEST);
    if (request_length == 0)
    {
      continue;
    }
    request_[request_length] = '\0';
    error_ = false;
    response_.startObject();
    response_.setCompactPrint();
    if (request_[0] == START_CHAR_JSON_OBJECT)
    {
      addToResponse("status",ERROR);
      char error_message[STRING_LENGTH_ERROR] = {0};
      object_request_error_message.copy(error_message);
      addToResponse("error_message",error_message);
      error_ = true;
    }
    else
    {
      if (request_[0] != START_CHAR_JSON_ARRAY)
      {
        response_.setPrettyPrint();
        String request_string = String("[") + String(request_) + String("]");
        request_string.toCharArray(request_,STRING_LENGTH_REQUEST);
      }
      request_json_array_ = parser_.parse(request_);
      if (request_json_array_.success())
      {
        processRequestArray();
      }
      else
      {
        addToResponse("status",ERROR);
        char error_message[STRING_LENGTH_ERROR] = {0};
        array_parse_error_message.copy(error_message);
        addToResponse("error_message",error_message);
        addToResponse("received_request",request_);
        error_ = true;
      }
      if (!error_)
      {
        addToResponse("status",SUCCESS);
      }
    }
    response_.stopObject();
    *stream_ptr_ << endl;
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
      Parameter& parameter = *(method_vector_[request_method_index_].parameter_ptr_vector_[parameter_index]);
      parameterHelp(parameter);
    }
    else if (parameter_count != method_vector_[request_method_index_].parameter_count_)
    {
      addToResponse("status",ERROR);
      String error_request = "Incorrect number of parameters. ";
      error_request += String(parameter_count) + String(" given. ");
      error_request += String(method_vector_[request_method_index_].parameter_count_);
      error_request += String(" needed.");
      char error_str[STRING_LENGTH_ERROR];
      error_request.toCharArray(error_str,STRING_LENGTH_ERROR);
      addToResponse("error_message",error_str);
      error_ = true;
    }
    else
    {
      boolean parameters_ok = checkParameters();
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
  if ((method_index < 0) || (method_index >= method_vector_.size()))
  {
    addToResponse("status",ERROR);
    addToResponse("error_message","Unknown method.");
    error_ = true;
    method_index = -1;
  }
  return method_index;
}

int Server::findMethodIndex(char *method_name)
{
  int method_index = -1;
  for (std::vector<Method>::iterator method_it = method_vector_.begin();
       method_it != method_vector_.end();
       ++method_it)
  {
    if (method_it->compareName(method_name))
    {
      method_index = std::distance(method_vector_.begin(),method_it);
      break;
    }
  }
  return method_index;
}

int Server::findMethodIndex(const _FLASH_STRING &method_name)
{
  int method_index = -1;
  for (std::vector<Method>::iterator method_it = method_vector_.begin();
       method_it != method_vector_.end();
       ++method_it)
  {
    if (method_it->compareName(method_name))
    {
      method_index = std::distance(method_vector_.begin(),method_it);
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
  if (method_vector_[request_method_index_].isReserved())
  {
    method_vector_[request_method_index_].reservedCallback(this);
  }
  else
  {
    method_vector_[request_method_index_].callback();
  }
}

void Server::methodHelp(int method_index)
{
  int parameter_index = 0;
  addKeyToResponse("parameters");
  response_.startArray();
  std::vector<Parameter*>& parameter_ptr_vector = method_vector_[method_index].parameter_ptr_vector_;
  const _FLASH_STRING* parameter_name_ptr;
  char parameter_name_char_array[STRING_LENGTH_PARAMETER_NAME];
  for (std::vector<Parameter*>::iterator param_ptr_it = parameter_ptr_vector.begin();
       param_ptr_it != parameter_ptr_vector.end();
       ++param_ptr_it)
  {
    if (parameter_index < PARAMETER_COUNT_MAX)
    {
      parameter_name_ptr = (*param_ptr_it)->getNamePointer();
      parameter_name_ptr->copy(parameter_name_char_array);
      addToResponse(parameter_name_char_array);
      parameter_index++;
    }
  }
  response_.stopArray();
}

void Server::verboseMethodHelp(int method_index)
{
  int parameter_index = 0;
  addKeyToResponse("parameters");
  response_.startArray();
  std::vector<Parameter*>& parameter_ptr_vector = method_vector_[method_index].parameter_ptr_vector_;
  for (std::vector<Parameter*>::iterator param_ptr_it = parameter_ptr_vector.begin();
       param_ptr_it != parameter_ptr_vector.end();
       ++param_ptr_it)
  {
    if (parameter_index < PARAMETER_COUNT_MAX)
    {
      startResponseObject();
      parameterHelp(**param_ptr_it);
      stopResponseObject();
    }
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
  std::vector<Parameter*>& parameter_ptr_vector = method_vector_[request_method_index_].parameter_ptr_vector_;
  if ((parameter_index < 0) || (parameter_index >= parameter_ptr_vector.size()))
  {
    addToResponse("status",ERROR);
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
    std::vector<Parameter*>& parameter_ptr_vector = method_vector_[request_method_index_].parameter_ptr_vector_;
    for (std::vector<Parameter*>::iterator param_ptr_it = parameter_ptr_vector.begin();
         param_ptr_it != parameter_ptr_vector.end();
         ++param_ptr_it)
    {
      if ((*param_ptr_it)->compareName(parameter_name))
      {
        parameter_index = std::distance(parameter_ptr_vector.begin(),param_ptr_it);
        break;
      }
    }
  }
  return parameter_index;
}

int Server::findParameterIndex(const _FLASH_STRING &parameter_name)
{
  int parameter_index = -1;
  if (request_method_index_ >= 0)
  {
    std::vector<Parameter*>& parameter_ptr_vector = method_vector_[request_method_index_].parameter_ptr_vector_;
    for (std::vector<Parameter*>::iterator param_ptr_it = parameter_ptr_vector.begin();
         param_ptr_it != parameter_ptr_vector.end();
         ++param_ptr_it)
    {
      if ((*param_ptr_it)->compareName(parameter_name))
      {
        parameter_index = std::distance(parameter_ptr_vector.begin(),param_ptr_it);
        break;
      }
    }
  }
  return parameter_index;
}

void Server::parameterHelp(Parameter &parameter)
{
  char parameter_name[STRING_LENGTH_PARAMETER_NAME] = {0};
  const _FLASH_STRING* parameter_name_ptr = parameter.getNamePointer();
  parameter_name_ptr->copy(parameter_name);
  addKeyToResponse(parameter_name);

  startResponseObject();

  char parameter_units[STRING_LENGTH_PARAMETER_UNITS] = {0};
  const _FLASH_STRING* parameter_units_ptr = parameter.getUnitsPointer();
  parameter_units_ptr->copy(parameter_units);
  if (strcmp(parameter_units,"") != 0)
  {
    addToResponse("units",parameter_units);
  }
  ParameterType type = parameter.getType();
  switch (type)
  {
    case LONG_PARAMETER:
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
    case DOUBLE_PARAMETER:
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
    case STRING_PARAMETER:
      {
        addToResponse("type","string");
        break;
      }
    case OBJECT_PARAMETER:
      {
        addToResponse("type","object");
        break;
      }
    case ARRAY_PARAMETER:
      {
        addToResponse("type","array");
        break;
      }
  }
  stopResponseObject();
}

boolean Server::checkParameters()
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

boolean Server::checkParameter(int parameter_index, Parser::JsonValue json_value)
{
  boolean out_of_range = false;
  boolean object_parse_unsuccessful = false;
  boolean array_parse_unsuccessful = false;
  Parameter& parameter = *(method_vector_[request_method_index_].parameter_ptr_vector_[parameter_index]);
  ParameterType type = parameter.getType();
  String min_string = "";
  String max_string = "";
  switch (type)
  {
    case LONG_PARAMETER:
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
    case DOUBLE_PARAMETER:
      {
        if (parameter.rangeIsSet())
        {
          double value = (double)json_value;
          double min = parameter.getMin().d;
          double max = parameter.getMax().d;
          if ((value < min) || (value > max))
          {
            out_of_range = true;
            char temp_string[STRING_LENGTH_DOUBLE];
            dtostrf(min,DOUBLE_DIGITS,DOUBLE_DIGITS,temp_string);
            min_string = String(temp_string);
            dtostrf(max,DOUBLE_DIGITS,DOUBLE_DIGITS,temp_string);
            max_string = String(temp_string);
          }
        }
        break;
      }
    case STRING_PARAMETER:
      break;
    case OBJECT_PARAMETER:
      {
        Parser::JsonObject json_object = json_value;
        if (!json_object.success())
        {
          object_parse_unsuccessful = true;
        }
        break;
      }
    case ARRAY_PARAMETER:
      {
        Parser::JsonArray json_array = json_value;
        if (!json_array.success())
        {
          array_parse_unsuccessful = true;
        }
        break;
      }
  }
  if (out_of_range)
  {
    addToResponse("status",ERROR);
    String error = String("Parameter value out of range: ");
    error += min_string;
    error += String(" <= ");
    char parameter_name[STRING_LENGTH_PARAMETER_NAME] = {0};
    const _FLASH_STRING* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    error += String(parameter_name);
    error += String(" <= ");
    error += max_string;
    char error_str[STRING_LENGTH_ERROR];
    error.toCharArray(error_str,STRING_LENGTH_ERROR);
    addToResponse("error_message",error_str);
    error_ = true;
  }
  else if (object_parse_unsuccessful)
  {
    addToResponse("status",ERROR);
    char parameter_name[STRING_LENGTH_PARAMETER_NAME] = {0};
    const _FLASH_STRING* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    String error = String(parameter_name);
    error += String(" is not a valid JSON object.");
    char error_str[STRING_LENGTH_ERROR];
    error.toCharArray(error_str,STRING_LENGTH_ERROR);
    addToResponse("error_message",error_str);
    error_ = true;
  }
  else if (array_parse_unsuccessful)
  {
    addToResponse("status",ERROR);
    char parameter_name[STRING_LENGTH_PARAMETER_NAME] = {0};
    const _FLASH_STRING* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    String error = String(parameter_name);
    error += String(" is not a valid JSON array.");
    char error_str[STRING_LENGTH_ERROR];
    error.toCharArray(error_str,STRING_LENGTH_ERROR);
    addToResponse("error_message",error_str);
    error_ = true;
  }
  boolean parameter_ok = (!out_of_range) && (!object_parse_unsuccessful) && (!array_parse_unsuccessful);
  return parameter_ok;
}

int Server::findSavedVariableIndex(const _FLASH_STRING &saved_variable_name)
{
  int saved_variable_index = -1;
  for (std::vector<SavedVariable>::iterator saved_variable_it = saved_variable_vector_.begin();
       saved_variable_it != saved_variable_vector_.end();
       ++saved_variable_it)
  {
    if (saved_variable_it->compareName(saved_variable_name))
    {
      saved_variable_index = std::distance(saved_variable_vector_.begin(),saved_variable_it);
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
  saved_variable_vector_[eeprom_initialized_index_].setValue(EEPROM_INITIALIZED_VALUE);
  eeprom_uninitialized_ = false;
}

void Server::getDeviceInfoCallback()
{
  char device_name[STRING_LENGTH_DEVICE_NAME] = {0};
  name_ptr_->copy(device_name);
  addToResponse("name",device_name);
  addToResponse("model_number",model_number_);
  addToResponse("serial_number",getSerialNumber());
  addToResponse("firmware_number",firmware_number_);
}

void Server::getMethodIdsCallback()
{
  char method_name[STRING_LENGTH_METHOD_NAME] = {0};
  const _FLASH_STRING* method_name_ptr;
  for (std::vector<Method>::iterator method_it = method_vector_.begin();
       method_it != method_vector_.end();
       ++method_it)
  {
    int method_index = std::distance(method_vector_.begin(),method_it);
    if (!method_vector_[method_index].isReserved())
    {
      method_name_ptr = method_it->getNamePointer();
      method_name_ptr->copy(method_name);
      int method_id = method_index;
      addToResponse(method_name,method_id);
    }
  }
}

void Server::getResponseCodesCallback()
{
  addToResponse("response_success",SUCCESS);
  addToResponse("response_error",ERROR);
}

void Server::getParametersCallback()
{
  int parameter_index = 0;
  addKeyToResponse("parameters");
  response_.startArray();
  for (std::vector<Parameter>::iterator param_it = parameter_vector_.begin();
       param_it != parameter_vector_.end();
       ++param_it)
  {
    if (parameter_index < PARAMETER_COUNT_MAX)
    {
      startResponseObject();
      parameterHelp(*param_it);
      stopResponseObject();
    }
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
  char method_name[STRING_LENGTH_METHOD_NAME] = {0};
  const _FLASH_STRING* method_name_ptr;
  for (std::vector<Method>::iterator it = method_vector_.begin();
       it != method_vector_.end();
       ++it)
  {
    int method_index = std::distance(method_vector_.begin(),it);
    if (!method_vector_[method_index].isReserved())
    {
      method_name_ptr = it->getNamePointer();
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
  char method_name[STRING_LENGTH_METHOD_NAME] = {0};
  const _FLASH_STRING* method_name_ptr;
  for (std::vector<Method>::iterator it = method_vector_.begin();
       it != method_vector_.end();
       ++it)
  {
    int method_index = std::distance(method_vector_.begin(),it);
    if (!method_vector_[method_index].isReserved())
    {
      method_name_ptr = it->getNamePointer();
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
