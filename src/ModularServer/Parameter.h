// ----------------------------------------------------------------------------
// Parameter.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PARAMETER_H_
#define _MODULAR_SERVER_PARAMETER_H_
#include <Arduino.h>
#include <ConstantVariable.h>
#include <JsonStream.h>
#include <Vector.h>
#include <Functor.h>
#include <ArduinoJson.h>

#include "FirmwareElement.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{
class Parameter : private FirmwareElement
{
public:
  Parameter();

  void setTypeLong();
  void setTypeDouble();
  void setTypeBool();
  void setTypeString();
  void setTypeObject();
  void setTypeArray();
  void setTypeAny();
  void setType(JsonStream::JsonTypes type);

  void setUnits(const ConstantString & units);

  template <typename T>
  void setRange(T min,
    T max);
  void setRange(double min,
    double max);
  void setRange(float min,
    float max);
  void setRange(constants::NumberType min,
    constants::NumberType max);
  void removeRange();
  void setArrayLengthRange(size_t array_length_min,
    size_t array_length_max);
  void removeArrayLengthRange();
  template <size_t MAX_SIZE>
  void setSubset(constants::SubsetMemberType (&subset)[MAX_SIZE],
    size_t size=MAX_SIZE);
  void setSubset(constants::SubsetMemberType * subset,
    size_t max_size,
    size_t size);
  void setSubset(Vector<constants::SubsetMemberType> & subset);
  void addValueToSubset(constants::SubsetMemberType & value);
  void removeSubset();
  size_t getSubsetSize();
  size_t getSubsetMaxSize();

  template <typename T>
  bool getValue(T & value);
  bool getValue(const char * & value);
  template <typename U,
    size_t N>
  bool getValue(Array<U,N> & value);
  template <typename U>
  bool getValue(Vector<U> & value);
  bool getValue(ArduinoJson::JsonArray & value);
  bool getValue(ArduinoJson::JsonObject & value);
  bool getValue(const ConstantString * & value);

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
  void setup(const ConstantString & name);
  const ConstantString & getUnits();
  JsonStream::JsonTypes getType();
  JsonStream::JsonTypes getArrayElementType();
  bool rangeIsSet();
  template <typename T>
  bool valueInRange(T value);
  bool valueInRange(double value);
  bool valueInRange(float value);
  const constants::NumberType & getRangeMin();
  const constants::NumberType & getRangeMax();
  size_t getArrayLengthMin();
  size_t getArrayLengthMax();
  bool arrayLengthRangeIsSet();
  bool arrayLengthInRange(size_t array_length);
  bool subsetIsSet();
  int findSubsetValueIndex(long value);
  int findSubsetValueIndex(const char * value);
  int findSubsetValueIndex(const ConstantString * value);
  bool valueInSubset(long value);
  bool valueInSubset(const char * value);
  bool valueInSubset(const ConstantString * value);
  Vector<constants::SubsetMemberType> & getSubset();
  void writeApi(Response & response,
    bool write_name_only,
    bool write_method_type,
    bool is_property,
    bool write_firmware,
    bool write_instance_details);
  static Functor1wRet<const ConstantString &,ArduinoJson::JsonVariant> get_value_functor_;
  friend class Property;
  friend class Function;
  friend class Callback;
  friend class Server;

};
}
#include "ParameterDefinitions.h"

#endif
