// ----------------------------------------------------------------------------
// JsonPrinter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonPrinter.h"

namespace RemoteDevice
{
JsonDepthTracker::JsonDepthTracker()
{
  first_item_ = true;
  inside_object_ = true;
}

JsonDepthTracker::JsonDepthTracker(boolean first_item, boolean inside_object) :
  first_item_(first_item),
  inside_object_(inside_object)
{
}

JsonPrinter::JsonPrinter(Stream &stream)
{
  setStream(stream);
  setCompactPrint();
  indent_level_ = 0;
  jdt_vector_.reserve(RESPONSE_DEPTH_MAX);
}

void JsonPrinter::setStream(Stream &stream)
{
  stream_ptr_ = &stream;
}

void JsonPrinter::startObject()
{
  if (jdt_vector_.size() > 0)
  {
    stopArrayItem();
  }
  indent_level_++;
  jdt_vector_.push_back(JsonDepthTracker(true,true));
  *stream_ptr_ << "{";
}

void JsonPrinter::stopObject()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_vector_.back().first_item_))
  {
    *stream_ptr_ << endl;
    indent();
  }
  jdt_vector_.pop_back();
  *stream_ptr_ << "}";
}

void JsonPrinter::startArray()
{
  indent_level_++;
  jdt_vector_.push_back(JsonDepthTracker(true,false));
  *stream_ptr_ << "[";
}

void JsonPrinter::stopArray()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_vector_.back().first_item_))
  {
    *stream_ptr_ << endl;
    indent();
  }
  jdt_vector_.pop_back();
  *stream_ptr_ << "]";
}

void JsonPrinter::setCompactPrint()
{
  pretty_print_ = false;
}

void JsonPrinter::setPrettyPrint()
{
  pretty_print_ = true;
}

template <>
void JsonPrinter::add<char>(char value)
{
  stopArrayItem();
  *stream_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<const char*>(const char *value)
{
  stopArrayItem();
  *stream_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<char*>(char *value)
{
  stopArrayItem();
  *stream_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<String>(String value)
{
  indent();
  *stream_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<String&>(String &value)
{
  indent();
  *stream_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<unsigned char>(unsigned char value)
{
  stopArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<int>(int value)
{
  stopArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned int>(unsigned int value)
{
  stopArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<long>(long value)
{
  stopArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned long>(unsigned long value)
{
  stopArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<long long>(long long value)
{
  stopArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned long long>(unsigned long long value)
{
  stopArrayItem();
  *stream_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<ResponseCodes>(ResponseCodes value)
{
  stopArrayItem();
  if (!pretty_print_)
  {
    *stream_ptr_ <<  value;
  }
  else
  {
    switch (value)
    {
      case ERROR:
        *stream_ptr_ <<  "error";
        break;
      case SUCCESS:
        *stream_ptr_ <<  "success";
        break;
    }
  }
}

template <>
void JsonPrinter::add<double>(double value)
{
  stopArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  // dtostre(value,value_char_array,DOUBLE_DIGITS,0);
  dtostrf(value,DOUBLE_DIGITS,DOUBLE_DIGITS,value_char_array);
  *stream_ptr_ <<  value_char_array;
}

template <>
void JsonPrinter::add<float>(float value)
{
  stopArrayItem();
  char value_char_array[STRING_LENGTH_DOUBLE];
  // dtostre((double)value,value_char_array,DOUBLE_DIGITS,0);
  dtostrf((double)value,DOUBLE_DIGITS,DOUBLE_DIGITS,value_char_array);
  *stream_ptr_ <<  value_char_array;
}

void JsonPrinter::addKey(const char *key)
{
  stopItem();
  *stream_ptr_ << "\"" << key << "\"" << ":";
}

void JsonPrinter::addNull()
{
  stopArrayItem();
  *stream_ptr_ << "null";
}

void JsonPrinter::addBoolean(const boolean value)
{
  stopArrayItem();
  if (value)
  {
    *stream_ptr_ <<  "true";
  }
  else
  {
    *stream_ptr_ <<  "false";
  }
}

void JsonPrinter::indent()
{
  if (pretty_print_)
  {
    for (int i=0;i<(RESPONSE_INDENT*indent_level_);i++)
    {
      *stream_ptr_ << " ";
    }
  }
}

void JsonPrinter::stopItem()
{
  if (!jdt_vector_.back().first_item_)
  {
    *stream_ptr_ << ",";
  }
  else
  {
    jdt_vector_.back().first_item_ = false;
  }
  if (pretty_print_)
  {
    *stream_ptr_ << endl;
  }
  indent();
}

void JsonPrinter::stopArrayItem()
{
  if (!jdt_vector_.back().inside_object_)
  {
    stopItem();
  }
}
}
