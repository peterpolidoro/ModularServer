// ----------------------------------------------------------------------------
// ModularServer.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef MODULAR_SERVER_H
#define MODULAR_SERVER_H
#include "ModularServer/Server.h"


namespace modular_server
{
using FirmwareInfo = constants::FirmwareInfo;
using HardwareInfo = constants::HardwareInfo;
using SubsetMemberType = constants::SubsetMemberType;

class ModularServer
{
public:
  ModularServer();
  void setup();

  // Streams
  void addServerStream(Stream & stream);

  // Device ID
  void setDeviceName(const ConstantString & device_name);
  void setFormFactor(const ConstantString & form_factor);

  // Hardware
  template <size_t PINS_MAX_SIZE>
  void addHardware(const HardwareInfo & hardware_info,
    Pin (&pins)[PINS_MAX_SIZE]);
  void removeHardware();

  // Pins
  Pin & createPin(const ConstantString & pin_name,
    size_t pin_number);
  Pin & pin(const ConstantString & pin_name);

  // Firmware
  template <size_t PROPERTIES_MAX_SIZE,
    size_t PARAMETERS_MAX_SIZE,
    size_t FUNCTIONS_MAX_SIZE,
    size_t CALLBACKS_MAX_SIZE>
  void addFirmware(const FirmwareInfo & firmware_info,
    Property (&properties)[PROPERTIES_MAX_SIZE],
    Parameter (&parameters)[PARAMETERS_MAX_SIZE],
    Function (&functions)[FUNCTIONS_MAX_SIZE],
    Callback (&callbacks)[CALLBACKS_MAX_SIZE]);

  // Properties
  template <typename T>
  Property & createProperty(const ConstantString & property_name,
    const T & default_value);
  template <typename T,
    size_t N>
  Property & createProperty(const ConstantString & property_name,
    const T (&default_value)[N]);
  Property & property(const ConstantString & property_name);
  template <typename T>
  void setPropertiesToDefaults(T & firmware_name_array);

  // Parameters
  Parameter & createParameter(const ConstantString & parameter_name);
  Parameter & parameter(const ConstantString & parameter_name);
  Parameter & copyParameter(Parameter & parameter,
    const ConstantString & parameter_name);

  // Functions
  Function & createFunction(const ConstantString & function_name);
  Function & function(const ConstantString & function_name);
  Function & copyFunction(Function & function,
    const ConstantString & function_name);

  // Callbacks
  Callback & createCallback(const ConstantString & callback_name);
  Callback & callback(const ConstantString & callback_name);

  // Response
  Response & response();

  // Server
  void startServer();
  void stopServer();
  void handleServerRequests();

private:
  Server server_;
};
}
#include "ModularServer/ModularServerDefinitions.h"

#endif
