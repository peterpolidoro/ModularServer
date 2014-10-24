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
JsonPrinter::JsonPrinter(Stream &stream)
{
  setStream(stream);
  setCompactPrint();
  items_count_ = 0;
  indent_level_ = 0;
  object_mode_ = true;
}

void JsonPrinter::setStream(Stream &stream)
{
  stream_ptr_ = &stream;
}

void JsonPrinter::startObject()
{
  items_count_ = 0;
  indent_level_++;
  object_mode_ = true;
  *stream_ptr_ << "{";
}

void JsonPrinter::stopObject()
{
  indent_level_--;
  if (pretty_print_ && (items_count_ > 0))
  {
    *stream_ptr_ << endl;
    indent();
  }
  object_mode_ = false;
  *stream_ptr_ << "}";
}

void JsonPrinter::startArray()
{
  items_count_ = 0;
  indent_level_++;
  object_mode_ = false;
  *stream_ptr_ << "[";
}

void JsonPrinter::stopArray()
{
  indent_level_--;
  if (pretty_print_ && (items_count_ > 0))
  {
    *stream_ptr_ << endl;
    indent();
  }
  object_mode_ = true;
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
void JsonPrinter::add<String&>(String &value)
{
  indent();
  *stream_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<int>(int value)
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

// void JsonPrinter::addEmptyItem(char *key) {
//   addKey(key);
//   *stream_ptr_ << "\"" << "\"";
// }

// void JsonPrinter::addDblItem(char *key, double value) {
//   char valueStr[DP_STR_LEN];
//   addKey(key);
//   dtostre(value,valueStr,DP_DOUBLE_PREC,0);
//   *stream_ptr_ <<  valueStr;
// }

// void JsonPrinter::addFltItem(char *key, float value) {
//   char valueStr[DP_STR_LEN];
//   addKey(key);
//   dtostre((double)value, valueStr, DP_DOUBLE_PREC,0);
//   *stream_ptr_ <<  valueStr;
// }

// void JsonPrinter::addLongTuple(char *key, uint8_t num, ...) {
//   va_list args;
//   long value;
//   addKey(key);
//   *stream_ptr_ << "(";
//   va_start(args,num);
//   for (uint8_t i=0; i<num; i++) {
//     value = va_arg(args,long);
//     *stream_ptr_ << _DEC(value);
//     if (i < num-1) {
//       *stream_ptr_ << ",";
//     }
//   }
//   *stream_ptr_ << ")";
// }

// int JsonPrinter::length()
// {
//   return items_count_array_[depth_];
// }

void JsonPrinter::addKey(const char *key)
{
  stopItem();
  *stream_ptr_ << "\"" << key << "\"" << ":";
}

void JsonPrinter::stopItem()
{
  if (items_count_ > 0)
  {
    *stream_ptr_ << ",";
  }
  if (pretty_print_)
  {
    *stream_ptr_ << endl;
  }
  items_count_++;
  indent();
}

void JsonPrinter::stopArrayItem()
{
  if (!object_mode_)
  {
    stopItem();
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
}
