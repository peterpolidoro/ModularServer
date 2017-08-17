// ----------------------------------------------------------------------------
// ModularServer.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef MODULAR_SERVER_H
#define MODULAR_SERVER_H
#include "utility/Server.h"


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
  template <size_t INTERRUPTS_MAX_SIZE>
  void addHardware(const HardwareInfo & hardware_info,
                   Interrupt (&interrupts)[INTERRUPTS_MAX_SIZE]);

  // Interrupts
  Interrupt & createInterrupt(const ConstantString & interrupt_name,
                              const size_t pin);
  Interrupt & interrupt(const ConstantString & interrupt_name);

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
  template <typename T, size_t N>
  Property & createProperty(const ConstantString & property_name,
                            const T (&default_value)[N]);
  Property & property(const ConstantString & property_name);
  void setPropertiesToDefaults(ArduinoJson::JsonArray & firmware_name_array);
  void setAllPropertiesToDefaults();

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
#include "ModularServerDefinitions.h"

#endif
