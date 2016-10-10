// ----------------------------------------------------------------------------
// Response.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_RESPONSE_H_
#define _MODULAR_SERVER_RESPONSE_H_
#include "Streaming.h"
#include "Array.h"
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Functor.h"

#include "Parameter.h"
#include "Constants.h"


namespace modular_server
{
class Response
{
public:
  Response();

  template <typename K>
  void writeKey(K key);
  template <typename T>
  void write(T value);
  template <typename T, size_t N>
  void write(T (&value)[N]);
  void write(Vector<const constants::SubsetMemberType> & value, JsonStream::JsonTypes type);
  template <typename K, typename T>
  void write(K key, T value);
  template <typename K, typename T, size_t N>
  void write(K key, T (&value)[N]);
  template <typename T>
  void writeArray(T * value, size_t N);
  template <typename K, typename T>
  void writeArray(K key, T * value, size_t N);
  void writeNull();
  template <typename K>
  void writeNull(K key);
  template <typename T>
  void sendError(T error);
  void writeResultKey();
  template <typename T>
  void writeResult(T value);
  template <typename T, size_t N>
  void writeResult(T (&value)[N]);
  template <typename T>
  void writeResult(T * value, size_t N);
  void beginObject();
  void endObject();
  void beginArray();
  void endArray();

private:
  JsonStream json_stream_;
};
}
#include "ResponseDefinitions.h"

#endif
