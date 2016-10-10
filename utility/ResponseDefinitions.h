// ----------------------------------------------------------------------------
// ResponseDefinitions.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_RESPONSE_DEFINITIONS_H_
#define _MODULAR_SERVER_RESPONSE_DEFINITIONS_H_


namespace modular_server
{
template <typename K>
void Response::writeKeyToResponse(K key)
{
  json_stream_.writeKey(key);
}

template <typename T>
void Response::writeToResponse(T value)
{
  json_stream_.write(value);
}

template <typename T, size_t N>
void Response::writeToResponse(T (&value)[N])
{
  json_stream_.write(value);
}

template <typename K, typename T>
void Response::writeToResponse(K key, T value)
{
  json_stream_.write(key,value);
}

template <typename K, typename T, size_t N>
void Response::writeToResponse(K key, T (&value)[N])
{
  json_stream_.write(key,value);
}

template <typename T>
void Response::writeArrayToResponse(T * value, size_t N)
{
  json_stream_.writeArray(value,N);
}

template <typename K, typename T>
void Response::writeArrayToResponse(K key, T * value, size_t N)
{
  json_stream_.writeArray(key,value,N);
}

template <typename K>
void Response::writeNullToResponse(K key)
{
  json_stream_.writeNull(key);
}

template <typename T>
void Response::sendErrorResponse(T error)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKeyToResponse(constants::error_constant_string);
    beginResponseObject();
    writeToResponse(constants::message_constant_string,constants::server_error_error_message);
    writeToResponse(constants::data_constant_string,error);
    writeToResponse(constants::code_constant_string,constants::server_error_error_code);
    endResponseObject();
  }
}

template <typename T>
void Response::writeResultToResponse(T value)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.write(constants::result_constant_string,value);
  }
}

template <typename T, size_t N>
void Response::writeResultToResponse(T (&value)[N])
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.write(constants::result_constant_string,value);
  }
}

template <typename T>
void Response::writeResultToResponse(T * value, size_t N)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.writeArray(constants::result_constant_string,value,N);
  }
}

}
#endif
