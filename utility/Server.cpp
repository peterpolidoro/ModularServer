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
CONSTANT_STRING(status_constant_string,"status");
CONSTANT_STRING(error_message_constant_string,"error_message");
CONSTANT_STRING(name_constant_string,"name");
CONSTANT_STRING(type_constant_string,"type");
CONSTANT_STRING(array_element_type_constant_string,"array_element_type");
CONSTANT_STRING(received_request_constant_string,"received_request");
CONSTANT_STRING(method_id_constant_string,"method_id");
CONSTANT_STRING(method_constant_string,"method");
CONSTANT_STRING(methods_constant_string,"methods");
CONSTANT_STRING(parameter_constant_string,"parameter");
CONSTANT_STRING(parameters_constant_string,"parameters");
CONSTANT_STRING(unknown_method_constant_string,"Unknown method.");
CONSTANT_STRING(unknown_parameter_constant_string,"Unknown parameter.");
CONSTANT_STRING(long_constant_string,"long");
CONSTANT_STRING(double_constant_string,"double");
CONSTANT_STRING(bool_constant_string,"bool");
CONSTANT_STRING(string_constant_string,"string");
CONSTANT_STRING(object_constant_string,"object");
CONSTANT_STRING(array_constant_string,"array");
CONSTANT_STRING(min_constant_string,"min");
CONSTANT_STRING(max_constant_string,"max");
CONSTANT_STRING(model_number_constant_string,"model_number");
CONSTANT_STRING(serial_number_constant_string,"serial_number");
CONSTANT_STRING(firmware_version_constant_string,"firmware_version");
CONSTANT_STRING(major_constant_string,"major");
CONSTANT_STRING(minor_constant_string,"minor");
CONSTANT_STRING(patch_constant_string,"patch");
CONSTANT_STRING(response_success_constant_string,"response_success");
CONSTANT_STRING(response_error_constant_string,"response_error");
CONSTANT_STRING(device_info_constant_string,"device_info");
CONSTANT_STRING(incorrect_parameter_number_constant_string,"Incorrect number of parameters. ")
CONSTANT_STRING(invalid_json_object_constant_string," is not a valid JSON object.")
CONSTANT_STRING(invalid_json_array_constant_string," is not a valid JSON array.")


Server::Server(GenericSerial &serial) :
  response_(serial)
{
  setSerial(serial);
  setName(default_device_name);
  model_number_ = 0;
  firmware_major_ = 0;
  firmware_minor_ = 0;
  firmware_patch_ = 0;
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

void Server::setFirmwareVersion(const unsigned char firmware_major,const unsigned char firmware_minor,const unsigned char firmware_patch)
{
  firmware_major_ = firmware_major;
  firmware_minor_ = firmware_minor;
  firmware_patch_ = firmware_patch;
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

ArduinoJson::Parser::JsonValue Server::getParameterValue(const ConstantString &name)
{
  int parameter_index = findParameterIndex(name);
  return request_json_array_[parameter_index+1];
}

void Server::addNullToResponse()
{
  response_.addNull();
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
    request_[request_length] = 0;
    error_ = false;
    response_.startObject();
    response_.setCompactPrint();
    if (request_[0] == constants::start_char_json_object)
    {
      addToResponse(status_constant_string,JsonPrinter::ERROR);
      char error_message[constants::STRING_LENGTH_ERROR];
      error_message[0] = 0;
      object_request_error_message.copy(error_message);
      addToResponse(error_message_constant_string,error_message);
      error_ = true;
    }
    else
    {
      if (request_[0] != constants::start_char_json_array)
      {
        response_.setPrettyPrint();
        char request[constants::STRING_LENGTH_REQUEST];
        request[0] = 0;
        strcat(request,"[");
        strcat(request,request_);
        strcat(request,"]");
        request_[0] = 0;
        strcat(request_,request);
      }
      request_json_array_ = parser_.parse(request_);
      if (request_json_array_.success())
      {
        processRequestArray();
      }
      else
      {
        addToResponse(status_constant_string,JsonPrinter::ERROR);
        char error_message[constants::STRING_LENGTH_ERROR];
        error_message[0] = 0;
        array_parse_error_message.copy(error_message);
        addToResponse(error_message_constant_string,error_message);
        addToResponse(received_request_constant_string,request_);
        error_ = true;
      }
      if (!error_)
      {
        addToResponse(status_constant_string,JsonPrinter::SUCCESS);
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
    if ((parameter_count == 1) && (strcmp(request_json_array_[1],"?") == 0))
    {
      methodHelp(request_method_index_);
    }
    else if ((parameter_count == 1) && (strcmp(request_json_array_[1],"??") == 0))
    {
      verboseMethodHelp(request_method_index_);
    }
    else if ((parameter_count == 2) && (strcmp(request_json_array_[2],"?") == 0) || (strcmp(request_json_array_[2],"??") == 0))
    {
      int parameter_index = processParameterString(request_json_array_[1]);
      Parameter& parameter = *(method_array_[request_method_index_].parameter_ptr_array_[parameter_index]);
      addKeyToResponse(parameter_constant_string);
      parameterHelp(parameter);
    }
    else if (parameter_count != method_array_[request_method_index_].parameter_count_)
    {
      addToResponse(status_constant_string,JsonPrinter::ERROR);
      char incorrect_parameter_number[incorrect_parameter_number_constant_string.length()+1];
      incorrect_parameter_number_constant_string.copy(incorrect_parameter_number);
      char error_str[constants::STRING_LENGTH_ERROR];
      error_str[0] = 0;
      strcat(error_str,incorrect_parameter_number);
      char parameter_count_str[constants::STRING_LENGTH_PARAMETER_COUNT];
      dtostrf(parameter_count,0,0,parameter_count_str);
      strcat(error_str,parameter_count_str);
      strcat(error_str," given. ");
      dtostrf(method_array_[request_method_index_].parameter_count_,0,0,parameter_count_str);
      strcat(error_str,parameter_count_str);
      strcat(error_str," needed.");
      addToResponse(error_message_constant_string,error_str);
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
  int method_id = atoi(method_string);
  if (strcmp(method_string,"0") == 0)
  {
    method_index = 0;
    addToResponse(method_id_constant_string,0);
  }
  else if (method_id > 0)
  {
    method_index = method_id;
    addToResponse(method_id_constant_string,method_id);
  }
  else
  {
    method_index = findMethodIndex(method_string);
    addToResponse(method_constant_string,method_string);
  }
  if ((method_index < 0) || (method_index >= (int)method_array_.size()))
  {
    addToResponse(status_constant_string,JsonPrinter::ERROR);
    addToResponse(error_message_constant_string,unknown_method_constant_string);
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

int Server::countJsonArrayElements(ArduinoJson::Parser::JsonArray &json_array)
{
  int elements_count = 0;
  for (ArduinoJson::Parser::JsonArrayIterator array_it=json_array.begin();
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
  addKeyToResponse(parameters_constant_string);
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
  addKeyToResponse(parameters_constant_string);
  response_.startArray();
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>& parameter_ptr_array = method_array_[method_index].parameter_ptr_array_;
  for (unsigned int i=0; i<parameter_ptr_array.size(); ++i)
  {
    parameterHelp(*(parameter_ptr_array[i]));
  }
  response_.stopArray();
}

int Server::processParameterString(char *parameter_string)
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
    parameter_index = findParameterIndex(parameter_string);
  }
  Array<Parameter*,constants::METHOD_PARAMETER_COUNT_MAX>& parameter_ptr_array = method_array_[request_method_index_].parameter_ptr_array_;
  if ((parameter_index < 0) || (parameter_index >= (int)parameter_ptr_array.size()))
  {
    addToResponse(status_constant_string,JsonPrinter::ERROR);
    addToResponse(error_message_constant_string,unknown_parameter_constant_string);
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
  startResponseObject();
  char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME];
  parameter_name[0] = 0;
  const ConstantString* parameter_name_ptr = parameter.getNamePointer();
  parameter_name_ptr->copy(parameter_name);
  addToResponse(name_constant_string,parameter_name);

  char parameter_units[constants::STRING_LENGTH_PARAMETER_UNITS];
  parameter_units[0] = 0;
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
        addToResponse(type_constant_string,long_constant_string);
        if (parameter.rangeIsSet())
        {
          long min = parameter.getMin().l;
          long max = parameter.getMax().l;
          addToResponse(min_constant_string,min);
          addToResponse(max_constant_string,max);
        }
        break;
      }
    case constants::DOUBLE_PARAMETER:
      {
        addToResponse(type_constant_string,double_constant_string);
        if (parameter.rangeIsSet())
        {
          double min = parameter.getMin().d;
          double max = parameter.getMax().d;
          addToResponse(min_constant_string,min);
          addToResponse(max_constant_string,max);
        }
        break;
      }
    case constants::BOOL_PARAMETER:
      {
        addToResponse(type_constant_string,bool_constant_string);
        break;
      }
    case constants::STRING_PARAMETER:
      {
        addToResponse(type_constant_string,string_constant_string);
        break;
      }
    case constants::OBJECT_PARAMETER:
      {
        addToResponse(type_constant_string,object_constant_string);
        break;
      }
    case constants::ARRAY_PARAMETER:
      {
        addToResponse(type_constant_string,array_constant_string);
        constants::ParameterType array_element_type = parameter.getArrayElementType();
        switch (array_element_type)
        {
          case constants::LONG_PARAMETER:
            {
              addToResponse(array_element_type_constant_string,long_constant_string);
              if (parameter.rangeIsSet())
              {
                long min = parameter.getMin().l;
                long max = parameter.getMax().l;
                addToResponse(min_constant_string,min);
                addToResponse(max_constant_string,max);
              }
              break;
            }
          case constants::DOUBLE_PARAMETER:
            {
              addToResponse(array_element_type_constant_string,double_constant_string);
              if (parameter.rangeIsSet())
              {
                double min = parameter.getMin().d;
                double max = parameter.getMax().d;
                addToResponse(min_constant_string,min);
                addToResponse(max_constant_string,max);
              }
              break;
            }
          case constants::BOOL_PARAMETER:
            {
              addToResponse(array_element_type_constant_string,bool_constant_string);
              break;
            }
          case constants::STRING_PARAMETER:
            {
              addToResponse(array_element_type_constant_string,string_constant_string);
              break;
            }
          case constants::OBJECT_PARAMETER:
            {
              addToResponse(array_element_type_constant_string,object_constant_string);
              break;
            }
          case constants::ARRAY_PARAMETER:
            {
              addToResponse(array_element_type_constant_string,array_constant_string);
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
  for (ArduinoJson::Parser::JsonArrayIterator request_it=request_json_array_.begin();
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

bool Server::checkParameter(int parameter_index, ArduinoJson::Parser::JsonValue json_value)
{
  bool out_of_range = false;
  bool object_parse_unsuccessful = false;
  bool array_parse_unsuccessful = false;
  Parameter& parameter = *(method_array_[request_method_index_].parameter_ptr_array_[parameter_index]);
  constants::ParameterType type = parameter.getType();
  char min_str[JsonPrinter::STRING_LENGTH_DOUBLE];
  min_str[0] = 0;
  char max_str[JsonPrinter::STRING_LENGTH_DOUBLE];
  max_str[0] = 0;
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
            dtostrf(min,0,0,min_str);
            dtostrf(max,0,0,max_str);
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
            dtostrf(min,0,JsonPrinter::DOUBLE_DIGITS,min_str);
            dtostrf(max,0,JsonPrinter::DOUBLE_DIGITS,max_str);
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
        ArduinoJson::Parser::JsonObject json_object = json_value;
        if (!json_object.success())
        {
          object_parse_unsuccessful = true;
        }
        break;
      }
    case constants::ARRAY_PARAMETER:
      {
        ArduinoJson::Parser::JsonArray json_array = json_value;
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
                  for (ArduinoJson::Parser::JsonArrayIterator it=json_array.begin();
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
            case constants::DOUBLE_PARAMETER:
              {
                if (parameter.rangeIsSet())
                {
                  double value;
                  double min = parameter.getMin().d;
                  double max = parameter.getMax().d;
                  for (ArduinoJson::Parser::JsonArrayIterator it=json_array.begin();
                       it!=json_array.end();
                       ++it)
                  {
                    value = (double)*it;
                    if ((value < min) || (value > max))
                    {
                      out_of_range = true;
                      dtostrf(min,0,JsonPrinter::DOUBLE_DIGITS,min_str);
                      dtostrf(max,0,JsonPrinter::DOUBLE_DIGITS,max_str);
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
    addToResponse(status_constant_string,JsonPrinter::ERROR);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = 0;
    if (type != constants::ARRAY_PARAMETER)
    {
      parameter_error_preamble_message.copy(error_str);
    }
    else
    {
      array_parameter_error_preamble_message.copy(error_str);
    }
    strcat(error_str,min_str);
    strcat(error_str," <= ");
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME];
    parameter_name[0] = 0;
    const ConstantString* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    strcat(error_str,parameter_name);
    if (type == constants::ARRAY_PARAMETER)
    {
      strcat(error_str," element");
    }
    strcat(error_str," <= ");
    strcat(error_str,max_str);
    addToResponse(error_message_constant_string,error_str);
    error_ = true;
  }
  else if (object_parse_unsuccessful)
  {
    addToResponse(status_constant_string,JsonPrinter::ERROR);
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME];
    parameter_name[0] = 0;
    const ConstantString* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = 0;
    strcat(error_str,parameter_name);
    char invalid_json_object[invalid_json_object_constant_string.length()+1];
    invalid_json_object_constant_string.copy(invalid_json_object);
    strcat(error_str,invalid_json_object);
    addToResponse(error_message_constant_string,error_str);
    error_ = true;
  }
  else if (array_parse_unsuccessful)
  {
    addToResponse(status_constant_string,JsonPrinter::ERROR);
    char parameter_name[constants::STRING_LENGTH_PARAMETER_NAME];
    parameter_name[0] = 0;
    const ConstantString* parameter_name_ptr = parameter.getNamePointer();
    parameter_name_ptr->copy(parameter_name);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = 0;
    strcat(error_str,parameter_name);
    char invalid_json_array[invalid_json_array_constant_string.length()+1];
    invalid_json_array_constant_string.copy(invalid_json_array);
    strcat(error_str,invalid_json_array);
    addToResponse(error_message_constant_string,error_str);
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
  char device_name[constants::STRING_LENGTH_DEVICE_NAME];
  device_name[0] = 0;
  name_ptr_->copy(device_name);
  addToResponse(name_constant_string,device_name);
  addToResponse(model_number_constant_string,model_number_);
  addToResponse(serial_number_constant_string,getSerialNumber());
  addKeyToResponse(firmware_version_constant_string);
  startResponseObject();
  addToResponse(major_constant_string,firmware_major_);
  addToResponse(minor_constant_string,firmware_minor_);
  addToResponse(patch_constant_string,firmware_patch_);
  stopResponseObject();
}

void Server::getMethodIdsCallback()
{
  char method_name[constants::STRING_LENGTH_METHOD_NAME];
  method_name[0] = 0;
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
  addToResponse(response_success_constant_string,JsonPrinter::SUCCESS);
  addToResponse(response_error_constant_string,JsonPrinter::ERROR);
}

void Server::getParametersCallback()
{
  addKeyToResponse(parameters_constant_string);
  response_.startArray();
  for (unsigned int i=0; i<parameter_array_.size(); ++i)
  {
    parameterHelp(parameter_array_[i]);
  }
  response_.stopArray();
}

void Server::help()
{
  addKeyToResponse(device_info_constant_string);
  startResponseObject();
  getDeviceInfoCallback();
  stopResponseObject();

  addKeyToResponse(methods_constant_string);
  startResponseArray();
  char method_name[constants::STRING_LENGTH_METHOD_NAME];
  method_name[0] = 0;
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
  addKeyToResponse(device_info_constant_string);
  startResponseObject();
  getDeviceInfoCallback();
  stopResponseObject();

  addKeyToResponse(methods_constant_string);
  startResponseArray();
  char method_name[constants::STRING_LENGTH_METHOD_NAME];
  method_name[0] = 0;
  const ConstantString* method_name_ptr;
  for (unsigned int i=0; i<method_array_.size(); ++i)
  {
    int method_index = i;
    if (!method_array_[method_index].isReserved())
    {
      startResponseObject();
      method_name_ptr = method_array_[i].getNamePointer();
      method_name_ptr->copy(method_name);
      addToResponse(name_constant_string,method_name);
      verboseMethodHelp(method_index);
      stopResponseObject();
    }
  }
  stopResponseArray();
}
}
