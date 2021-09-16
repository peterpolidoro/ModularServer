// ----------------------------------------------------------------------------
// Response.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_RESPONSE_H_
#define _MODULAR_SERVER_RESPONSE_H_
#include <ConstantVariable.h>
#include <Array.h>
#include <JsonStream.h>

#include "Constants.h"


namespace modular_server
{
class Response
{
public:
  template <typename T>
  void returnResult(T value);
  template <typename T,
    size_t N>
  void returnResult(T (&value)[N]);
  template <typename T>
  void returnResult(T * value,
    size_t N);

  template <typename T>
  void returnError(T error);

  void writeResultKey();
  template <typename K>
  void writeKey(K key);
  template <typename T>
  void write(T value);
  template <typename T,
    size_t N>
  void write(T (&value)[N]);
  void write(Vector<constants::SubsetMemberType> & value,
    JsonStream::JsonTypes type);
  template <typename K,
    typename T>
  void write(K key,
    T value);
  template <typename K,
    typename T,
    size_t N>
  void write(K key,
    T (&value)[N]);
  template <typename T>
  void writeArray(T * value,
    size_t N);
  template <typename K,
    typename T>
  void writeArray(K key,
    T * value,
    size_t N);
  void writeNull();
  template <typename K>
  void writeNull(K key);

  void beginObject();
  void endObject();
  void beginArray();
  void endArray();

  long pipeFrom(Stream & stream);
  long pipeFrom(JsonStream & json_stream);

  bool error();

private:
  JsonStream * json_stream_ptr_;
  bool error_;
  bool result_key_in_response_;

  Response();
  void reset();
  void setJsonStream(JsonStream & json_stream);
  void begin();
  void end();
  void setCompactPrint();
  void setPrettyPrint();
  void returnRequestParseError(const char * const request);
  void returnParameterCountError(size_t parameter_count,
    size_t parameter_count_needed);
  void returnMethodNotFoundError();
  void returnParameterNotFoundError();
  void returnParameterIncorrectTypeError(const ConstantString & parameter_name);
  void returnParameterArrayLengthError(const ConstantString & parameter_name,
    const char * const min_str,
    const char * const max_str);
  void returnParameterInvalidError(const ConstantString & error);
  void returnParameterNotInSubsetError(const char * const subset_str,
    const JsonStream::JsonTypes & parameter_type);
  void returnParameterNotInRangeError(const ConstantString & parameter_name,
    const JsonStream::JsonTypes & parameter_type,
    const char * const min_str,
    const char * const max_str);
  void returnPropertyFunctionNotFoundError();
  void returnPropertyParameterCountError(size_t parameter_count,
    size_t parameter_count_needed);
  void returnCallbackFunctionNotFoundError();
  void returnCallbackParameterCountError(size_t parameter_count,
    size_t parameter_count_needed);
  friend class Server;
  friend class Property;
};
}
#include "ResponseDefinitions.h"

#endif
