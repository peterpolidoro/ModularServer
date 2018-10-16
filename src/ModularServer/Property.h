// ----------------------------------------------------------------------------
// Property.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_PROPERTY_H_
#define _MODULAR_SERVER_PROPERTY_H_
#include <Arduino.h>
#include <SavedVariable.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConcatenatedArray.h>
#include <Functor.h>
#include <ArduinoJson.h>

#include "Parameter.h"
#include "Function.h"
#include "Response.h"
#include "Constants.h"


namespace modular_server
{

namespace property
{
enum{FUNCTION_PARAMETER_TYPE_COUNT=2};
enum{PARAMETER_COUNT_MAX=1};
enum{FUNCTION_COUNT_MAX=4};
enum{ARRAY_PARAMETER_COUNT_MAX=3};
enum{ARRAY_FUNCTION_COUNT_MAX=7};

// Parameters
extern ConstantString value_parameter_name;

// Functions
extern ConstantString get_value_function_name;
extern ConstantString set_value_function_name;
extern ConstantString get_default_value_function_name;
extern ConstantString set_value_to_default_function_name;

// Array Parameters
extern ConstantString element_index_parameter_name;
extern ConstantString element_value_parameter_name;
extern ConstantString array_length_parameter_name;

// Array Functions
extern ConstantString get_element_value_function_name;
extern ConstantString set_element_value_function_name;
extern ConstantString get_default_element_value_function_name;
extern ConstantString set_element_value_to_default_function_name;
extern ConstantString set_all_element_values_function_name;
extern ConstantString get_array_length_function_name;
extern ConstantString set_array_length_function_name;
}

class Property
{
public:
  Property();

  template <typename T>
  bool getValue(T & value);
  template <size_t N>
  bool getValue(Array<long,N> & value);
  template <size_t N>
  bool getValue(Array<double,N> & value);
  template <size_t N>
  bool getValue(Array<bool,N> & value);
  template <size_t N>
  bool getValue(Array<const ConstantString *,N> & value);
  template <typename T>
  bool getValue(T * value,
    const size_t N);
  template <typename T>
  bool getElementValue(const size_t element_index,
    T & element_value);
  template <typename T>
  bool getDefaultValue(T & default_value);
  template <size_t N>
  bool getDefaultValue(Array<long,N> & default_value);
  template <size_t N>
  bool getDefaultValue(Array<double,N> & default_value);
  template <size_t N>
  bool getDefaultValue(Array<bool,N> & default_value);
  template <size_t N>
  bool getDefaultValue(Array<const ConstantString *,N> & default_value);
  template <typename T>
  bool getDefaultValue(T * default_value,
    const size_t N);
  template <typename T>
  bool getDefaultElementValue(const size_t element_index,
    T & default_element_value);

  template <typename T>
  bool setElementValue(const size_t element_index,
    const T & element_value);
  template <typename T>
  bool setValue(const T & value);
  template <size_t N>
  bool setValue(Array<long,N> & value);
  template <size_t N>
  bool setValue(Array<double,N> & value);
  template <size_t N>
  bool setValue(Array<bool,N> & value);
  template <size_t N>
  bool setValue(Array<const ConstantString *,N> & value);
  bool setValue(ArduinoJson::JsonArray & value);
  template <typename T>
  bool setValue(T * value,
    const size_t N);
  template <typename T>
  bool setAllElementValues(const T & element_value);
  template <typename T>
  bool setDefaultValue(const T & default_value);
  template <size_t N>
  bool setDefaultValue(const long (&default_value)[N]);
  template <size_t N>
  bool setDefaultValue(const double (&default_value)[N]);
  template <size_t N>
  bool setDefaultValue(const bool (&default_value)[N]);
  template <size_t N>
  bool setDefaultValue(const char (&default_value)[N]);
  template <size_t N>
  bool setDefaultValue(const ConstantString * const (&default_value)[N]);
  bool setDefaultToRangeMin();
  bool setDefaultToRangeMax();
  bool setDefaultToSubsetElement(const size_t element_index);
  void setValueToDefault();
  void setElementValueToDefault(const size_t element_index);

  bool valueIsDefault();

  size_t getArrayLength();
  void setArrayLength(const size_t array_length);
  void setArrayLengthRange(const size_t array_length_min,
    const size_t array_length_max);
  size_t getArrayLengthMin();
  size_t getArrayLengthMax();

  size_t getArrayLengthDefault();
  void setArrayLengthDefault(const size_t array_length_default);
  void setArrayLengthToDefault();

  size_t getStringLength();

  void setUnits(const ConstantString & name);
  void setRange(const long min, const long max);
  void setRange(const double min, const double max);
  template <size_t MAX_SIZE>
  void setSubset(constants::SubsetMemberType (&subset)[MAX_SIZE], size_t size=MAX_SIZE);
  void setSubset(constants::SubsetMemberType * subset, size_t max_size, size_t size);
  void addValueToSubset(constants::SubsetMemberType & value);

  void attachPreSetValueFunctor(const Functor0 & functor);
  void attachPreSetElementValueFunctor(const Functor1<const size_t> & functor);
  void attachPostSetValueFunctor(const Functor0 & functor);
  void attachPostSetElementValueFunctor(const Functor1<const size_t> & functor);
  void disableFunctors();
  void reenableFunctors();

private:
  static Parameter property_parameters_[property::PARAMETER_COUNT_MAX];
  static Function property_functions_[property::FUNCTION_COUNT_MAX];
  static Parameter property_array_parameters_[property::ARRAY_PARAMETER_COUNT_MAX];
  static Function property_array_functions_[property::ARRAY_FUNCTION_COUNT_MAX];
  static ConcatenatedArray<Parameter,property::FUNCTION_PARAMETER_TYPE_COUNT> parameters_;
  static ConcatenatedArray<Function,property::FUNCTION_PARAMETER_TYPE_COUNT> functions_;
  static Response * response_ptr_;
  static Functor1wRet<const ConstantString &, ArduinoJson::JsonVariant> get_parameter_value_functor_;

  template <typename T>
  static int findParameterIndex(T const & parameter_name)
  {
    int parameter_index = -1;
    for (size_t i=0; i<parameters_.size(); ++i)
    {
      if (parameters_[i].compareName(parameter_name))
      {
        parameter_index = i;
        break;
      }
    }
    return parameter_index;
  };
  static Parameter & createParameter(const ConstantString & parameter_name);
  static Parameter & parameter(const ConstantString & parameter_name);
  static Parameter & copyParameter(Parameter parameter, const ConstantString & parameter_name);

  template <typename T>
  static int findFunctionIndex(T const & function_name)
  {
    int function_index = -1;
    for (size_t i=0; i<functions_.size(); ++i)
    {
      if (functions_[i].compareName(function_name))
      {
        function_index = i;
        break;
      }
    }
    return function_index;
  };
  static Function & createFunction(const ConstantString & function_name);
  static Function & function(const ConstantString & function_name);

  Parameter parameter_;
  SavedVariable saved_variable_;

  Functor0 pre_set_value_functor_;
  Functor1<const size_t> pre_set_element_value_functor_;
  Functor0 post_set_value_functor_;
  Functor1<const size_t> post_set_element_value_functor_;
  bool functors_enabled_;

  bool string_saved_as_char_array_;

  size_t array_length_min_;
  size_t array_length_max_;

  template <typename T>
  Property(const ConstantString & name,
    const T & default_value);
  template <size_t N>
  Property(const ConstantString & name,
    const long (&default_value)[N]);
  template <size_t N>
  Property(const ConstantString & name,
    const double (&default_value)[N]);
  template <size_t N>
  Property(const ConstantString & name,
    const bool (&default_value)[N]);
  template <size_t N>
  Property(const ConstantString & name,
    const char (&default_value)[N]);
  template <size_t N>
  Property(const ConstantString & name,
    const ConstantString * const (&default_value)[N]);

  void setup();
  Parameter & parameter();
  bool compareName(const char * name_to_compare);
  bool compareName(const ConstantString & name_to_compare);
  const ConstantString & getName();
  const ConstantString &  getFirmwareName();
  bool firmwareNameInArray(ArduinoJson::JsonArray & firmware_name_array);
  JsonStream::JsonTypes getType();
  JsonStream::JsonTypes getArrayElementType();
  bool rangeIsSet();
  bool subsetIsSet();
  bool stringSavedAsCharArray();
  int findSubsetValueIndex(const long value);
  int findSubsetValueIndex(const char * value);
  int findSubsetValueIndex(const ConstantString * value);
  bool valueInSubset(const long value);
  bool valueInSubset(const char * value);
  bool valueInSubset(const ConstantString * value);
  Vector<constants::SubsetMemberType> & getSubset();
  void preSetValueFunctor();
  void preSetElementValueFunctor(const size_t element_index);
  void postSetValueFunctor();
  void postSetElementValueFunctor(const size_t element_index);
  void writeValue(Response & response,
    bool write_key=false,
    bool write_default=false,
    int element_index=-1);
  void writeApi(Response & response,
    bool write_name_only,
    bool write_firmware,
    bool write_function_parameter_details,
    bool write_instance_details);
  void updateFunctionsAndParameters();

  // Handlers
  void getValueHandler();
  void setValueHandler();
  void getDefaultValueHandler();
  void setValueToDefaultHandler();

  void getElementValueHandler();
  void setElementValueHandler();
  void getDefaultElementValueHandler();
  void setElementValueToDefaultHandler();
  void setAllElementValuesHandler();
  void getArrayLengthHandler();
  void setArrayLengthHandler();

  friend class Callback;
  friend class Server;
};
}
#include "PropertyDefinitions.h"

#endif
