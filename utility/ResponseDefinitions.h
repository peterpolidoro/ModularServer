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
// public
template <typename T>
void Response::returnResult(T value)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_ptr_->write(constants::result_constant_string,value);
  }
}

template <typename T, size_t N>
void Response::returnResult(T (&value)[N])
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_ptr_->write(constants::result_constant_string,value);
  }
}

template <typename T>
void Response::returnResult(T * value, size_t N)
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_ptr_->writeArray(constants::result_constant_string,value,N);
  }
}

template <typename T>
void Response::returnError(T error)
{
  // Prevent multiple errors in one response
  if (!error_)
  {
    error_ = true;
    writeKey(constants::error_constant_string);
    beginObject();
    write(constants::message_constant_string,constants::server_error_error_message);
    write(constants::data_constant_string,error);
    write(constants::code_constant_string,constants::server_error_error_code);
    endObject();
  }
}

template <typename K>
void Response::writeKey(K key)
{
  json_stream_ptr_->writeKey(key);
}

template <typename T>
void Response::write(T value)
{
  json_stream_ptr_->write(value);
}

template <typename T, size_t N>
void Response::write(T (&value)[N])
{
  json_stream_ptr_->write(value);
}

template <typename K, typename T>
void Response::write(K key, T value)
{
  json_stream_ptr_->write(key,value);
}

template <typename K, typename T, size_t N>
void Response::write(K key, T (&value)[N])
{
  json_stream_ptr_->write(key,value);
}

template <typename T>
void Response::writeArray(T * value, size_t N)
{
  json_stream_ptr_->writeArray(value,N);
}

template <typename K, typename T>
void Response::writeArray(K key, T * value, size_t N)
{
  json_stream_ptr_->writeArray(key,value,N);
}

template <typename K>
void Response::writeNull(K key)
{
  json_stream_ptr_->writeNull(key);
}

// protected

}
#endif
