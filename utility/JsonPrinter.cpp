// ----------------------------------------------------------------------------
// JsonPrinter.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "JsonPrinter.h"

namespace ModularDevice
{
JsonDepthTracker::JsonDepthTracker()
{
  first_item_ = true;
  inside_object_ = true;
}

JsonDepthTracker::JsonDepthTracker(bool first_item, bool inside_object) :
  first_item_(first_item),
  inside_object_(inside_object)
{
}

JsonPrinter::JsonPrinter(HardwareSerial &serial)
{
  setSerial(serial);
  setCompactPrint();
  indent_level_ = 0;
}

void JsonPrinter::setSerial(HardwareSerial &serial)
{
  serial_ptr_ = &serial;
}

void JsonPrinter::startObject()
{
  if (!jdt_array_.empty())
  {
    stopArrayItem();
  }
  indent_level_++;
  jdt_array_.push_back(JsonDepthTracker(true,true));
  *serial_ptr_ << "{";
}

void JsonPrinter::stopObject()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_array_.back().first_item_))
  {
    *serial_ptr_ << endl;
    indent();
  }
  jdt_array_.pop_back();
  *serial_ptr_ << "}";
}

void JsonPrinter::startArray()
{
  indent_level_++;
  jdt_array_.push_back(JsonDepthTracker(true,false));
  *serial_ptr_ << "[";
}

void JsonPrinter::stopArray()
{
  indent_level_--;
  if (pretty_print_ && (!jdt_array_.back().first_item_))
  {
    *serial_ptr_ << endl;
    indent();
  }
  jdt_array_.pop_back();
  *serial_ptr_ << "]";
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
  *serial_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<const char*>(const char *value)
{
  stopArrayItem();
  *serial_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<char*>(char *value)
{
  stopArrayItem();
  *serial_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<String>(String value)
{
  indent();
  *serial_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<String&>(String &value)
{
  indent();
  *serial_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<ConstantString>(ConstantString value)
{
  indent();
  *serial_ptr_ << "\"" << value << "\"";
}

template <>
void JsonPrinter::add<unsigned char>(unsigned char value)
{
  stopArrayItem();
  *serial_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<int>(int value)
{
  stopArrayItem();
  *serial_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned int>(unsigned int value)
{
  stopArrayItem();
  *serial_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<long>(long value)
{
  stopArrayItem();
  *serial_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned long>(unsigned long value)
{
  stopArrayItem();
  *serial_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<long long>(long long value)
{
  stopArrayItem();
  *serial_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<unsigned long long>(unsigned long long value)
{
  stopArrayItem();
  *serial_ptr_ <<  _DEC(value);
}

template <>
void JsonPrinter::add<constants::ResponseCodes>(constants::ResponseCodes value)
{
  stopArrayItem();
  if (!pretty_print_)
  {
    *serial_ptr_ <<  value;
  }
  else
  {
    switch (value)
    {
      case constants::ERROR:
        *serial_ptr_ <<  "error";
        break;
      case constants::SUCCESS:
        *serial_ptr_ <<  "success";
        break;
    }
  }
}

template <>
void JsonPrinter::add<double>(double value)
{
  stopArrayItem();
  char value_char_array[constants::STRING_LENGTH_DOUBLE];
  // dtostre(value,value_char_array,constants::double_digits,0);
  dtostrf(value,constants::double_digits,constants::double_digits,value_char_array);
  *serial_ptr_ <<  value_char_array;
}

template <>
void JsonPrinter::add<float>(float value)
{
  stopArrayItem();
  char value_char_array[constants::STRING_LENGTH_DOUBLE];
  // dtostre((double)value,value_char_array,constants::double_digits,0);
  dtostrf((double)value,constants::double_digits,constants::double_digits,value_char_array);
  *serial_ptr_ <<  value_char_array;
}

template <>
void JsonPrinter::add<bool>(bool value)
{
  if (value)
  {
    *serial_ptr_ <<  "true";
  }
  else
  {
    *serial_ptr_ <<  "false";
  }
}

void JsonPrinter::addKey(const char *key)
{
  stopItem();
  *serial_ptr_ << "\"" << key << "\"" << ":";
}

void JsonPrinter::addNull()
{
  stopArrayItem();
  *serial_ptr_ << "null";
}

void JsonPrinter::addBool(const bool value)
{
  stopArrayItem();
  if (value)
  {
    *serial_ptr_ <<  "true";
  }
  else
  {
    *serial_ptr_ <<  "false";
  }
}

void JsonPrinter::indent()
{
  if (pretty_print_)
  {
    for (int i=0; i<(constants::response_indent*indent_level_); ++i)
    {
      *serial_ptr_ << " ";
    }
  }
}

void JsonPrinter::stopItem()
{
  if (!jdt_array_.back().first_item_)
  {
    *serial_ptr_ << ",";
  }
  else
  {
    jdt_array_.back().first_item_ = false;
  }
  if (pretty_print_)
  {
    *serial_ptr_ << endl;
  }
  indent();
}

void JsonPrinter::stopArrayItem()
{
  if (!jdt_array_.back().inside_object_)
  {
    stopItem();
  }
}
}
