// ----------------------------------------------------------------------------
// Response.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Response.h"


namespace modular_server
{
// public
void Response::writeResultKey()
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_ptr_->writeKey(constants::result_constant_string);
  }
}

void Response::write(Vector<constants::SubsetMemberType> & value, JsonStream::JsonTypes type)
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
      json_stream_ptr_->write(subset_elements_array);
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
      Array<const ConstantString *,constants::SUBSET_ELEMENT_COUNT_MAX> subset_elements_array;
      for (size_t i=0; i<value.size(); ++i)
      {
        subset_elements_array.push_back(value[i].cs_ptr);
      }
      json_stream_ptr_->write(subset_elements_array);
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
    case JsonStream::VOID_TYPE:
    {
      break;
    }
  }
}

void Response::writeNull()
{
  json_stream_ptr_->writeNull();
}

void Response::beginObject()
{
  json_stream_ptr_->beginObject();
}

void Response::endObject()
{
  json_stream_ptr_->endObject();
}

void Response::beginArray()
{
  json_stream_ptr_->beginArray();
}

void Response::endArray()
{
  json_stream_ptr_->endArray();
}

// private
Response::Response()
{
  json_stream_ptr_ = NULL;
  reset();
}

void Response::reset()
{
  error_ = false;
  result_key_in_response_ = false;
}

void Response::setJsonStream(JsonStream & json_stream)
{
  json_stream_ptr_ = &json_stream;
}

void Response::begin()
{
  reset();
  beginObject();
}

void Response::end()
{
  if (!error_ && !result_key_in_response_)
  {
    writeNull(constants::result_constant_string);
  }
  endObject();
  json_stream_ptr_->writeNewline();
}

void Response::setCompactPrint()
{
  json_stream_ptr_->setCompactPrint();
}

void Response::setPrettyPrint()
{
  json_stream_ptr_->setPrettyPrint();
}

bool Response::error()
{
  return error_;
}

void Response::returnRequestParseError(const char * const request)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::parse_error_message);
    write(constants::data_constant_string,request);
    write(constants::code_constant_string,constants::parse_error_code);
    endObject();
  }
}

void Response::returnParameterCountError(const size_t parameter_count, const size_t parameter_count_needed)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    char incorrect_parameter_number_str[constants::incorrect_parameter_number_error_data.length()+1];
    incorrect_parameter_number_str[0] = '\0';
    constants::incorrect_parameter_number_error_data.copy(incorrect_parameter_number_str);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    strcat(error_str,incorrect_parameter_number_str);
    char parameter_count_str[constants::STRING_LENGTH_PARAMETER_COUNT];
    parameter_count_str[0] = '\0';
    dtostrf(parameter_count,0,0,parameter_count_str);
    strcat(error_str,parameter_count_str);
    char given_str[constants::given_constant_string.length()+1];
    given_str[0] = '\0';
    constants::given_constant_string.copy(given_str);
    strcat(error_str,given_str);
    dtostrf(parameter_count_needed,0,0,parameter_count_str);
    strcat(error_str,parameter_count_str);
    char needed_str[constants::needed_constant_string.length()+1];
    needed_str[0] = '\0';
    constants::needed_constant_string.copy(needed_str);
    strcat(error_str,needed_str);
    write(constants::data_constant_string,error_str);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

void Response::returnProcedureNotFoundError()
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::procedure_not_found_error_message);
    write(constants::code_constant_string,constants::procedure_not_found_error_code);
    endObject();
  }
}

void Response::returnParameterNotFoundError()
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    write(constants::data_constant_string,constants::parameter_not_found_error_data);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

void Response::returnParameterArrayLengthError(const ConstantString & parameter_name,
                                               const char * const min_str,
                                               const char * const max_str)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    constants::array_parameter_length_error_error_data.copy(error_str);
    char value_not_in_range_str[constants::value_not_in_range_error_data.length() + 1];
    constants::value_not_in_range_error_data.copy(value_not_in_range_str);
    strcat(error_str,value_not_in_range_str);
    strcat(error_str,min_str);
    char less_than_equal_str[constants::less_than_equal_constant_string.length()+1];
    less_than_equal_str[0] = '\0';
    constants::less_than_equal_constant_string.copy(less_than_equal_str);
    strcat(error_str,less_than_equal_str);
    char parameter_name_str[parameter_name.length()+1];
    parameter_name_str[0] = '\0';
    parameter_name.copy(parameter_name_str);
    strcat(error_str,parameter_name_str);
    char array_length_str[constants::array_length_constant_string.length()+1];
    array_length_str[0] = '\0';
    constants::array_length_constant_string.copy(array_length_str);
    strcat(error_str,array_length_str);
    strcat(error_str,less_than_equal_str);
    strcat(error_str,max_str);
    write(constants::data_constant_string,error_str);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

void Response::returnParameterObjectParseError(const ConstantString & parameter_name)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    char parameter_name_str[parameter_name.length()+1];
    parameter_name_str[0] = '\0';
    parameter_name.copy(parameter_name_str);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    strcat(error_str,parameter_name_str);
    char invalid_json_object_str[constants::invalid_json_object_error_data.length()+1];
    invalid_json_object_str[0] = '\0';
    constants::invalid_json_object_error_data.copy(invalid_json_object_str);
    strcat(error_str,invalid_json_object_str);
    write(constants::data_constant_string,error_str);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

void Response::returnParameterArrayParseError(const ConstantString & parameter_name)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    char parameter_name_str[parameter_name.length()+1];
    parameter_name_str[0] = '\0';
    parameter_name.copy(parameter_name_str);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    strcat(error_str,parameter_name_str);
    char invalid_json_array_str[constants::invalid_json_array_error_data.length()+1];
    invalid_json_array_str[0] = '\0';
    constants::invalid_json_array_error_data.copy(invalid_json_array_str);
    strcat(error_str,invalid_json_array_str);
    write(constants::data_constant_string,error_str);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

void Response::returnParameterInvalidError(const ConstantString & error)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    write(constants::data_constant_string,error);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

void Response::returnParameterNotInSubsetError(const char * const subset_str,
                                               const JsonStream::JsonTypes & parameter_type)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    if (parameter_type != JsonStream::ARRAY_TYPE)
    {
      constants::parameter_error_error_data.copy(error_str);
    }
    else
    {
      constants::array_parameter_error_error_data.copy(error_str);
    }
    char value_not_in_subset_str[constants::value_not_in_subset_error_data.length() + 1];
    value_not_in_subset_str[0] = '\0';
    constants::value_not_in_subset_error_data.copy(value_not_in_subset_str);
    strcat(error_str,value_not_in_subset_str);
    size_t length_left = constants::STRING_LENGTH_ERROR - strlen(error_str) - 1;
    strncat(error_str,subset_str,length_left);
    write(constants::data_constant_string,error_str);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

void Response::returnParameterNotInRangeError(const ConstantString & parameter_name,
                                              const JsonStream::JsonTypes & parameter_type,
                                              const char * const min_str,
                                              const char * const max_str)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::invalid_params_error_message);
    char error_str[constants::STRING_LENGTH_ERROR];
    error_str[0] = '\0';
    if (parameter_type != JsonStream::ARRAY_TYPE)
    {
      constants::parameter_error_error_data.copy(error_str);
    }
    else
    {
      constants::array_parameter_error_error_data.copy(error_str);
    }
    char value_not_in_range_str[constants::value_not_in_range_error_data.length() + 1];
    value_not_in_range_str[0] = '\0';
    constants::value_not_in_range_error_data.copy(value_not_in_range_str);
    strcat(error_str,value_not_in_range_str);
    strcat(error_str,min_str);
    char less_than_equal_str[constants::less_than_equal_constant_string.length()+1];
    less_than_equal_str[0] = '\0';
    constants::less_than_equal_constant_string.copy(less_than_equal_str);
    strcat(error_str,less_than_equal_str);
    char parameter_name_str[parameter_name.length()+1];
    parameter_name_str[0] = '\0';
    parameter_name.copy(parameter_name_str);
    strcat(error_str,parameter_name_str);
    if (parameter_type == JsonStream::ARRAY_TYPE)
    {
      char element_str[constants::element_constant_string.length()+1];
      element_str[0] = '\0';
      constants::element_constant_string.copy(element_str);
      strcat(error_str,element_str);
    }
    strcat(error_str,less_than_equal_str);
    strcat(error_str,max_str);
    write(constants::data_constant_string,error_str);
    write(constants::code_constant_string,constants::invalid_params_error_code);
    endObject();
  }
}

}
