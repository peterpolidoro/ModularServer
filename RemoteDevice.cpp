// ----------------------------------------------------------------------------
// RemoteDevice.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "RemoteDevice.h"

RemoteDevice::RemoteDevice(Stream &stream)
{
  setRequestStream(stream);
  setName("");
  model_number_ = 0;
  serial_number_ = 0;
  firmware_number_ = 0;
  Method get_device_info_method("getDeviceInfo");
  get_device_info_method.attachReservedCallback(&RemoteDevice::getDeviceInfoCallback);
  addMethod(get_device_info_method);
  Method get_methods_method("getMethodIds");
  get_methods_method.attachReservedCallback(&RemoteDevice::getMethodIdsCallback);
  addMethod(get_methods_method);
  Method get_response_codes_method("getResponseCodes");
  get_response_codes_method.attachReservedCallback(&RemoteDevice::getResponseCodesCallback);
  addMethod(get_response_codes_method);
  Method help_method("?");
  help_method.attachReservedCallback(&RemoteDevice::help);
  addMethod(help_method);
}

void RemoteDevice::setRequestStream(Stream &stream)
{
  stream_ptr_ = &stream;
}

void RemoteDevice::processRequest()
{
  while (stream_ptr_->available() > 0)
  {
    int request_length = stream_ptr_->readBytesUntil(EOL,request_,STRING_LENGTH_REQUEST);
    if (request_length == 0)
    {
      continue;
    }
    request_[request_length] = '\0';
    response = Generator::JsonObject<JSON_OBJECT_SIZE_RESPONSE>();
    if (request_[0] == START_CHAR_JSON_OBJECT)
    {
      request_type_ = OBJECT_REQUEST;
      Parser::JsonObject json_object = parser_.parse(request_);
      if (json_object.success())
      {
        processObjectRequest(json_object);
      }
      else
      {
        response["status"] = ERROR;
        response["error_message"] = "Parsing JSON object request failed! Could be invalid JSON or too many tokens.";
        response["received_request"] = request_;
      }
    }
    else
    {
      request_type_ = ARRAY_REQUEST;
      if (request_[0] != START_CHAR_JSON_ARRAY)
      {
        request_type_ = COMMAND_LINE_REQUEST;
        String request_string = String("[") + String(request_) + String("]");
        request_string.toCharArray(request_,STRING_LENGTH_REQUEST);
      }
      Parser::JsonArray json_array = parser_.parse(request_);
      if (json_array.success())
      {
        processArrayRequest(json_array);
      }
      else
      {
        response["status"] = ERROR;
        response["error_message"] = "Parsing JSON array request failed! Could be invalid JSON or too many tokens.";
        response["received_request"] = request_;
      }
      if (!response.containsKey("status"))
      {
        response["status"] = SUCCESS;
      }
      if (request_type_ == COMMAND_LINE_REQUEST)
      {
        int status = response["status"];
        switch (status)
        {
          case ERROR:
            response["status"] = "error";
            break;
          case SUCCESS:
            response["status"] = "success";
            break;
        }
        if (response.containsKey("method"))
        {
          // Make help response a little less cluttered
          if (strcmp(response["method"],"?") == 0)
          {
            response.remove("method");
            response.remove("status");
          }
        }
        response.prettyPrintTo(*stream_ptr_);
      }
      else
      {
        response.printTo(*stream_ptr_);
      }
      *stream_ptr_ << endl;
    }
  }
}

void RemoteDevice::addMethod(Method method)
{
  char* name = method.getName();
  if (String(name).length() > 0)
  {
    int method_index = getMethodIndexByName(name);
    if (method_index < 0)
    {
      method_vector_.push_back(method);
    }
    else
    {
      method_vector_[method_index] = Method(name);
    }
  }
}

void RemoteDevice::setName(char *name)
{
  strncpy(name_,name,STRING_LENGTH_DEVICE_NAME);
}

void RemoteDevice::setModelNumber(int model_number)
{
  model_number_ = model_number;
}

void RemoteDevice::setFirmwareNumber(int firmware_number)
{
  firmware_number_ = firmware_number;
}

void RemoteDevice::processObjectRequest(Parser::JsonObject &json_object)
{
}

void RemoteDevice::processArrayRequest(Parser::JsonArray &json_array)
{
  char* method_string = json_array[0];
  int method_index = processMethodString(method_string);
  if (!(method_index < 0))
  {
    int array_elements_count = countJsonArrayElements(json_array);
    int argument_count = array_elements_count - 1;
    if ((argument_count == 1) && (String((char*)json_array[1]).equals("?")))
    {
      methodHelp(method_index);
    }
    else if (argument_count != method_vector_[method_index].parameter_count_)
    {
      response["status"] = ERROR;
      String error_request = "Incorrect number of parameters. ";
      error_request += String(argument_count) + String(" given. ");
      error_request += String(method_vector_[method_index].parameter_count_);
      error_request += String(" needed.");
      char error_str[STRING_LENGTH_ERROR];
      error_request.toCharArray(error_str,STRING_LENGTH_ERROR);
      response["error_message"] = error_str;
    }
    else
    {
      createParametersObject(method_index,json_array);
      if (parameters.success())
      {
      executeMethod(method_index);
      }
      else
      {
        response["status"] = ERROR;
        response["error_message"] = "Parsing JSON parameters string failed! Could be invalid JSON or too many tokens.";
      }
    }
  }
}

void RemoteDevice::createParametersObject(int method_index, Parser::JsonArray &json_array)
{
  int parameter_index = 0;
  String parameters_string = "{";
  String parameter_template = "\"{key}\":{value}";
  String parameter_string;
  for (Parser::JsonArrayIterator it=json_array.begin(); it!=json_array.end(); ++it)
  {
    if (it!=json_array.begin())
    {
      parameter_string = parameter_template;
      char *parameter_name = method_vector_[method_index].parameter_vector_[parameter_index].getName();
      parameter_string.replace("{key}",parameter_name);
      char* value_char_array = (char*)*it;
      parameter_string.replace("{value}",value_char_array);
      parameters_string = parameters_string + parameter_string + ",";
      parameter_index++;
    }
  }
  parameters_string = parameters_string + "}";
  char parameters_char_array[STRING_LENGTH_PARAMETERS];
  parameters_string.toCharArray(parameters_char_array,STRING_LENGTH_PARAMETERS);
  parameters = parser_.parse(parameters_char_array);
}

void RemoteDevice::executeMethod(int method_index)
{
  if (method_vector_[method_index].isReserved())
  {
    method_vector_[method_index].reservedCallback(this);
  }
  else
  {
    method_vector_[method_index].callback();
  }
}

void RemoteDevice::methodHelp(int method_index)
{
  response["parameters"] = method_vector_[method_index].help();
}

int RemoteDevice::processMethodString(char *method_string)
{
  int method_index = -1;
  int method_id = String(method_string).toInt();
  if (String(method_string).compareTo("0") == 0)
  {
    method_index = 0;
    response["method_id"] = 0;
  }
  else if (method_id > 0)
  {
    method_index = method_id;
    response["method_id"] = method_id;
  }
  else
  {
    method_index = getMethodIndexByName(method_string);
    response["method"] = method_string;
  }
  if ((method_index < 0) || (method_index >= method_vector_.size()))
  {
    response["status"] = ERROR;
    response["error_message"] = "Unknown method.";
    method_index = -1;
  }
  return method_index;
}

int RemoteDevice::getMethodIndexByName(char *method_name)
{
  int method_index = -1;
  for (std::vector<Method>::iterator it = method_vector_.begin();
       it != method_vector_.end();
       ++it)
  {
      if (it->compareName(method_name))
      {
        method_index = std::distance(method_vector_.begin(),it);
        break;
      }
    }
  return method_index;
}

int RemoteDevice::countJsonArrayElements(Parser::JsonArray &json_array)
{
  int elements_count = 0;
  for (Parser::JsonArrayIterator i=json_array.begin(); i!=json_array.end(); ++i)
  {
    elements_count++;
  }
  return elements_count;
}

void RemoteDevice::getDeviceInfoCallback()
{
  response["name"] = name_;
  response["model_number"] = model_number_;
  response["serial_number"] = serial_number_;
  response["firmware_number"] = firmware_number_;
}

void RemoteDevice::getMethodIdsCallback()
{
  for (std::vector<Method>::iterator it = method_vector_.begin();
       it != method_vector_.end();
       ++it)
  {
    int method_index = std::distance(method_vector_.begin(),it);
    if (!method_vector_[method_index].isReserved())
    {
      char* method_name = it->getName();
      int method_id = method_index;
      response[method_name] = method_id;
    }
  }
}

void RemoteDevice::getResponseCodesCallback()
{
  response["response_success"] = SUCCESS;
  response["response_error"] = ERROR;
}

void RemoteDevice::help()
{
  response["name"] = name_;
  Generator::JsonArray<JSON_OBJECT_SIZE_DEVICE_HELP> methods;
  for (std::vector<Method>::iterator it = method_vector_.begin();
       it != method_vector_.end();
       ++it)
  {
    int method_index = std::distance(method_vector_.begin(),it);
    if (!method_vector_[method_index].isReserved())
    {
      char* method_name = it->getName();
      methods.add(method_name);
    }
  }
  response["methods"] = methods;
}

RemoteDevice remote_device(Serial);
