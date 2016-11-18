// ----------------------------------------------------------------------------
// Parameter.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PARAMETER_H_
#define _MODULAR_SERVER_PARAMETER_H_
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "ConstantVariable.h"
#include "JsonStream.h"
#include "Vector.h"
#include "Functor.h"
#include "ArduinoJson.h"

#include "FirmwareElement.h"
#include "Constants.h"


namespace modular_server
{
class Parameter : private FirmwareElement
{
public:
  Parameter();

  void setUnits(const ConstantString & name);
  void setTypeLong();
  void setTypeDouble();
  void setTypeBool();
  void setTypeString();
  void setTypeObject();
  void setTypeArray();
  void setTypeAny();
  void setType(JsonStream::JsonTypes type);
  template <typename T>
  void setRange(const T min, const T max);
  void setRange(const double min, const double max);
  void setRange(const float min, const float max);
  void setRange(const constants::NumberType min, const constants::NumberType max);
  void removeRange();
  void setArrayLengthRange(const size_t array_length_min,
                           const size_t array_length_max);
  void removeArrayLengthRange();
  template <size_t MAX_SIZE>
  void setSubset(constants::SubsetMemberType (&subset)[MAX_SIZE], size_t size=MAX_SIZE);
  void setSubset(constants::SubsetMemberType * subset, size_t max_size, size_t size);
  void setSubset(Vector<constants::SubsetMemberType> & subset);
  void addValueToSubset(constants::SubsetMemberType & value);
  void removeSubset();

  template <typename T>
  bool getValue(T & value);
  bool getValue(const char * & value);
  bool getValue(ArduinoJson::JsonArray * & value);
  bool getValue(ArduinoJson::JsonObject * & value);

  Parameter getElementParameter();

private:
  const ConstantString * units_ptr_;
  JsonStream::JsonTypes type_;
  JsonStream::JsonTypes array_element_type_;
  constants::NumberType min_;
  constants::NumberType max_;
  bool range_is_set_;
  size_t array_length_min_;
  size_t array_length_max_;
  bool array_length_range_is_set_;
  Vector<constants::SubsetMemberType> subset_;
  bool subset_is_set_;
  Parameter(const ConstantString & name);
  const ConstantString & getUnits();
  JsonStream::JsonTypes getType();
  JsonStream::JsonTypes getArrayElementType();
  bool rangeIsSet();
  template <typename T>
  bool valueInRange(const T value);
  bool valueInRange(const double value);
  bool valueInRange(const float value);
  constants::NumberType getMin();
  constants::NumberType getMax();
  size_t getArrayLengthMin();
  size_t getArrayLengthMax();
  bool arrayLengthRangeIsSet();
  bool arrayLengthInRange(const size_t array_length);
  bool subsetIsSet();
  int findSubsetValueIndex(const long value);
  int findSubsetValueIndex(const char * value);
  bool valueInSubset(const long value);
  bool valueInSubset(const char * value);
  Vector<constants::SubsetMemberType> & getSubset();
  static Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> get_value_functor_;
  friend class Property;
  friend class Method;
  friend class Server;

};
}
#include "ParameterDefinitions.h"

#endif
