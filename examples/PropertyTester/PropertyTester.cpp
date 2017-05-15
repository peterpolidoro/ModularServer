// ----------------------------------------------------------------------------
// PropertyTester.cpp
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "PropertyTester.h"


void PropertyTester::setup()
{
  // Server Setup
  modular_server_.setup();

  // Pin Setup

  // Add Server Streams
  modular_server_.addServerStream(Serial);

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);
  modular_server_.setFormFactor(constants::form_factor);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
                              interrupts_);

  // Interrupts

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              properties_,
                              parameters_,
                              functions_,
                              callbacks_);

  // Properties
  modular_server::Property & double_property = modular_server_.createProperty(constants::double_property_name,constants::double_default);
  double_property.setRange(constants::double_min,constants::double_max);

  modular_server::Property & bool_property = modular_server_.createProperty(constants::bool_property_name,constants::bool_default);
  bool_property.attachPreSetValueFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::preSetPropertyValueHandler));
  bool_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::postSetPropertyValueHandler));

  modular_server::Property & long_array_property = modular_server_.createProperty(constants::long_array_property_name,constants::long_array_default);
  long_array_property.setRange(constants::long_array_element_min,constants::long_array_element_max);
  long_array_property.setArrayLengthRange(1,constants::LONG_ARRAY_LENGTH);

  modular_server::Property & double_array_property = modular_server_.createProperty(constants::double_array_property_name,constants::double_array_default);
  double_array_property.setRange(constants::double_array_element_min,constants::double_array_element_max);

  modular_server::Property & bool_array_property = modular_server_.createProperty(constants::bool_array_property_name,constants::bool_array_default);
  bool_array_property.attachPreSetValueFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::preSetPropertyValueHandler));
  bool_array_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::postSetPropertyValueHandler));
  bool_array_property.attachPreSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&PropertyTester::preSetPropertyElementValueHandler));
  bool_array_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&PropertyTester::postSetPropertyElementValueHandler));

  modular_server::Property & string_property = modular_server_.createProperty(constants::string_property_name,constants::string_default);
  string_property.attachPreSetValueFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::preSetPropertyValueHandler));
  string_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::postSetPropertyValueHandler));
  string_property.attachPreSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&PropertyTester::preSetPropertyElementValueHandler));
  string_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<const size_t> *)0,*this,&PropertyTester::postSetPropertyElementValueHandler));

  modular_server::Property & odd_property = modular_server_.createProperty(constants::odd_property_name,constants::odd_default);
  odd_property.setSubset(constants::odd_subset);

  modular_server::Property & mode_property = modular_server_.createProperty(constants::mode_property_name,constants::mode_ptr_default);
  mode_property.setSubset(constants::mode_ptr_subset);

  modular_server::Property & mode_array_property = modular_server_.createProperty(constants::mode_array_property_name,constants::mode_array_default);
  mode_array_property.setSubset(constants::mode_ptr_subset);

  modular_server::Property & odd_array_property = modular_server_.createProperty(constants::odd_array_property_name,constants::odd_array_default);
  odd_array_property.setSubset(constants::odd_subset);

  // Parameters
  modular_server::Parameter & long_array_parameter = modular_server_.createParameter(constants::long_array_parameter_name);
  long_array_parameter.setTypeLong();
  long_array_parameter.setTypeArray();

  modular_server::Parameter & length_parameter = modular_server_.createParameter(constants::length_parameter_name);
  length_parameter.setRange(constants::length_min,constants::length_max);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setTypeLong();
  count_parameter.setSubset(constants::count_subset);

  modular_server::Parameter & count_array_parameter = modular_server_.createParameter(constants::count_array_parameter_name);
  count_array_parameter.setTypeLong();
  count_array_parameter.setRange(constants::count_array_element_min,constants::count_array_element_max);
  count_array_parameter.setArrayLengthRange(constants::count_array_length_min,constants::count_array_length_max);
  count_array_parameter.setSubset(constants::count_subset);

  modular_server::Parameter & direction_parameter = modular_server_.createParameter(constants::direction_parameter_name);
  direction_parameter.setTypeString();
  direction_parameter.setSubset(constants::direction_ptr_subset);

  modular_server::Parameter & direction_array_parameter = modular_server_.createParameter(constants::direction_array_parameter_name);
  direction_array_parameter.setTypeString();
  direction_array_parameter.setArrayLengthRange(constants::direction_array_length_min,constants::direction_array_length_max);
  direction_array_parameter.setSubset(constants::direction_ptr_subset);

  modular_server::Parameter & subset_index_parameter = modular_server_.createParameter(constants::subset_index_parameter_name);
  subset_index_parameter.setTypeLong();

  // Functions
  modular_server::Function & get_doubled_function = modular_server_.createFunction(constants::get_doubled_function_name);
  get_doubled_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getDoubledHandler));
  get_doubled_function.setResultTypeDouble();

  modular_server::Function & get_bool_function = modular_server_.createFunction(constants::get_bool_function_name);
  get_bool_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getBoolHandler));
  get_bool_function.setResultTypeBool();

  modular_server::Function & get_long_array_fixed_function = modular_server_.createFunction(constants::get_long_array_fixed_function_name);
  get_long_array_fixed_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getLongArrayFixedHandler));
  get_long_array_fixed_function.setResultTypeArray();

  modular_server::Function & get_long_array_variable_function = modular_server_.createFunction(constants::get_long_array_variable_function_name);
  get_long_array_variable_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getLongArrayVariableHandler));
  get_long_array_variable_function.setResultTypeArray();

  modular_server::Function & set_long_array_fixed_function = modular_server_.createFunction(constants::set_long_array_fixed_function_name);
  set_long_array_fixed_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::setLongArrayFixedHandler));
  set_long_array_fixed_function.setResultTypeBool();

  modular_server::Function & set_long_array_variable_function = modular_server_.createFunction(constants::set_long_array_variable_function_name);
  set_long_array_variable_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::setLongArrayVariableHandler));
  set_long_array_variable_function.setResultTypeBool();

  modular_server::Function & set_long_array_parameter_function = modular_server_.createFunction(constants::set_long_array_parameter_function_name);
  set_long_array_parameter_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::setLongArrayParameterHandler));
  set_long_array_parameter_function.addParameter(long_array_parameter);
  set_long_array_parameter_function.setResultTypeBool();

  modular_server::Function & get_string_all_function = modular_server_.createFunction(constants::get_string_all_function_name);
  get_string_all_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getStringAllHandler));
  get_string_all_function.setResultTypeString();

  modular_server::Function & get_string_some_function = modular_server_.createFunction(constants::get_string_some_function_name);
  get_string_some_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getStringSomeHandler));
  get_string_some_function.addParameter(length_parameter);
  get_string_some_function.setResultTypeString();

  modular_server::Function & get_count_function = modular_server_.createFunction(constants::get_count_function_name);
  get_count_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getCountHandler));
  get_count_function.addParameter(count_parameter);
  get_count_function.setResultTypeString();

  modular_server::Function & get_count_array_function = modular_server_.createFunction(constants::get_count_array_function_name);
  get_count_array_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getCountArrayHandler));
  get_count_array_function.addParameter(count_array_parameter);
  get_count_array_function.setResultTypeArray();

  modular_server::Function & get_direction_function = modular_server_.createFunction(constants::get_direction_function_name);
  get_direction_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getDirectionHandler));
  get_direction_function.addParameter(direction_parameter);
  get_direction_function.setResultTypeString();

  modular_server::Function & get_direction_array_function = modular_server_.createFunction(constants::get_direction_array_function_name);
  get_direction_array_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::getDirectionArrayHandler));
  get_direction_array_function.addParameter(direction_array_parameter);
  get_direction_array_function.setResultTypeArray();

  modular_server::Function & check_mode_function = modular_server_.createFunction(constants::check_mode_function_name);
  check_mode_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::checkModeHandler));
  check_mode_function.setResultTypeString();

  modular_server::Function & increment_mode_function = modular_server_.createFunction(constants::increment_mode_function_name);
  increment_mode_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::incrementModeHandler));
  increment_mode_function.setResultTypeString();

  modular_server::Function & set_new_double_range_function = modular_server_.createFunction(constants::set_new_double_range_function_name);
  set_new_double_range_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::setNewDoubleRangeHandler));

  modular_server::Function & set_new_odd_subset_function = modular_server_.createFunction(constants::set_new_odd_subset_function_name);
  set_new_odd_subset_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::setNewOddSubsetHandler));

  modular_server::Function & set_new_odd_default_function = modular_server_.createFunction(constants::set_new_odd_default_function_name);
  set_new_odd_default_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&PropertyTester::setNewOddDefaultHandler));
  set_new_odd_default_function.addParameter(subset_index_parameter);

  // Callbacks

  // Begin Streams
  Serial.begin(constants::baudrate);

  // Start Modular Device Server
  modular_server_.startServer();
}

void PropertyTester::update()
{
  modular_server_.handleServerRequests();
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.property(property_name).getValue(value) value type must match the property default type
// modular_server_.property(property_name).setValue(value) value type must match the property default type
// modular_server_.property(property_name).getElementValue(element_index,value) value type must match the property array element default type
// modular_server_.property(property_name).setElementValue(element_index,value) value type must match the property array element default type

void PropertyTester::getDoubledHandler()
{
  double value;
  modular_server_.property(constants::double_property_name).getValue(value);
  value *= 2;
  modular_server_.response().returnResult(value);
}

void PropertyTester::getBoolHandler()
{
  bool value;
  modular_server_.property(constants::bool_property_name).getValue(value);
  modular_server_.response().returnResult(value);
}

void PropertyTester::getLongArrayFixedHandler()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  modular_server_.property(constants::long_array_property_name).getValue(long_array);
  modular_server_.response().returnResult(long_array);
}

void PropertyTester::getLongArrayVariableHandler()
{
  modular_server::Property & property = modular_server_.property(constants::long_array_property_name);
  size_t array_length = property.getArrayLength();
  long long_array[array_length];
  property.getValue(long_array,array_length);
  modular_server_.response().returnResult(long_array,array_length);
}

void PropertyTester::setLongArrayFixedHandler()
{
  long long_array[constants::LONG_ARRAY_LENGTH];
  long_array[0] = 1;
  long_array[1] = 2;
  long_array[2] = 9;
  long_array[3] = 10;
  bool success = modular_server_.property(constants::long_array_property_name).setValue(long_array);
  modular_server_.response().returnResult(success);
}

void PropertyTester::setLongArrayVariableHandler()
{
  modular_server::Property & property = modular_server_.property(constants::long_array_property_name);
  size_t array_length = property.getArrayLength();
  long long_array[array_length-1];
  long_array[0] = -1;
  long_array[1] = -2;
  long_array[2] = 7;
  bool success = property.setValue(long_array,array_length-1);
  modular_server_.response().returnResult(success);
}

void PropertyTester::setLongArrayParameterHandler()
{
  ArduinoJson::JsonArray * long_array_ptr;
  modular_server_.parameter(constants::long_array_parameter_name).getValue(long_array_ptr);
  bool success = modular_server_.property(constants::long_array_property_name).setValue(*long_array_ptr);
  modular_server_.response().returnResult(success);
}

void PropertyTester::getStringAllHandler()
{
  modular_server::Property & property = modular_server_.property(constants::string_property_name);
  size_t array_length = property.getArrayLength();
  char string[array_length];
  property.getValue(string,array_length);
  modular_server_.response().returnResult(string);
}

void PropertyTester::getStringSomeHandler()
{
  long length;
  modular_server_.parameter(constants::length_parameter_name).getValue(length);
  size_t array_length = length + 1;
  char string[array_length];
  modular_server_.property(constants::string_property_name).getValue(string,array_length);
  modular_server_.response().returnResult(string);
}

void PropertyTester::getCountHandler()
{
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  switch (count)
  {
    case 10:
    {
      modular_server_.response().returnResult("ten");
      break;
    }
    case 20:
    {
      modular_server_.response().returnResult("twenty");
      break;
    }
    case 30:
    {
      modular_server_.response().returnResult("thirty");
      break;
    }
    case 40:
    {
      modular_server_.response().returnResult("forty");
      break;
    }
    case 50:
    {
      modular_server_.response().returnResult("fifty");
      break;
    }
    case 60:
    {
      modular_server_.response().returnResult("sixty");
      break;
    }
    case 70:
    {
      modular_server_.response().returnResult("seventy");
      break;
    }
    default:
    {
      modular_server_.response().returnResult("???");
      break;
    }
  }
}

void PropertyTester::getCountArrayHandler()
{
  ArduinoJson::JsonArray * count_array_ptr;
  bool success = modular_server_.parameter(constants::count_array_parameter_name).getValue(count_array_ptr);
  modular_server_.response().returnResult(count_array_ptr);
}

void PropertyTester::getDirectionHandler()
{
  const char * direction;
  modular_server_.parameter(constants::direction_parameter_name).getValue(direction);
  modular_server_.response().returnResult(direction);
}

void PropertyTester::getDirectionArrayHandler()
{
  ArduinoJson::JsonArray * direction_array_ptr;
  modular_server_.parameter(constants::direction_array_parameter_name).getValue(direction_array_ptr);
  modular_server_.response().returnResult(direction_array_ptr);
}

void PropertyTester::checkModeHandler()
{
  const ConstantString * mode_ptr;
  modular_server_.property(constants::mode_property_name).getValue(mode_ptr);
  if (mode_ptr == &constants::mode_rising)
  {
    modular_server_.response().returnResult("Mode set to rising!");
  }
  else if (mode_ptr == &constants::mode_falling)
  {
    modular_server_.response().returnResult("Mode set to falling!");
  }
  else if (mode_ptr == &constants::mode_change)
  {
    modular_server_.response().returnResult("Mode set to change!");
  }
  else
  {
    modular_server_.response().returnResult("???");
  }
}

void PropertyTester::incrementModeHandler()
{
  modular_server::Property & property = modular_server_.property(constants::mode_property_name);
  const ConstantString * mode_ptr;
  property.getValue(mode_ptr);
  if (mode_ptr == &constants::mode_rising)
  {
    property.setValue(&constants::mode_falling);
    modular_server_.response().returnResult("Mode was set to rising, now set to falling!");
  }
  else if (mode_ptr == &constants::mode_falling)
  {
    property.setValue(&constants::mode_change);
    modular_server_.response().returnResult("Mode was set to falling, now set to change!");
  }
  else if (mode_ptr == &constants::mode_change)
  {
    property.setValue(&constants::mode_rising);
    modular_server_.response().returnResult("Mode was set to change, now set to rising!");
  }
  else
  {
    modular_server_.response().returnResult("???");
  }
}

void PropertyTester::setNewDoubleRangeHandler()
{
  modular_server_.property(constants::double_property_name).setRange(constants::double_min_new,constants::double_max_new);
}

void PropertyTester::setNewOddSubsetHandler()
{
  modular_server_.property(constants::odd_property_name).setSubset(constants::odd_subset_new);
}

void PropertyTester::setNewOddDefaultHandler()
{
  long subset_index;
  modular_server_.parameter(constants::subset_index_parameter_name).getValue(subset_index);
  modular_server_.property(constants::odd_property_name).setDefaultToSubsetElement(subset_index);
}

void PropertyTester::preSetPropertyValueHandler()
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setPropertyValueFunctor

  // Serial << " setting property value! ";
}

void PropertyTester::postSetPropertyValueHandler()
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setPropertyValueFunctor

  // Serial << " set property value! ";
}

void PropertyTester::preSetPropertyElementValueHandler(const size_t element_index)
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setPropertyElementValueFunctor

  // Serial << " setting property element value at index " << element_index << "! ";
}

void PropertyTester::postSetPropertyElementValueHandler(const size_t element_index)
{
  // Writing to Serial like this makes response invalid!
  // Do not do this in a real device!
  // Uncomment next line just to demonstrate setPropertyElementValueFunctor

  // Serial << " set property element value at index " << element_index << "! ";
}
