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

  // Streams
  void addServerStream(Stream & stream);

  // Device ID
  void setDeviceName(const ConstantString & device_name);
  void setFormFactor(const ConstantString & form_factor);

  // Hardware Info
  void addHardwareInfo(const HardwareInfo & hardware_info);

  // Firmware
  template <size_t FIELDS_MAX_SIZE,
            size_t PARAMETERS_MAX_SIZE,
            size_t METHODS_MAX_SIZE,
            size_t CALLBACKS_MAX_SIZE>
  void addFirmware(const FirmwareInfo & firmware_info,
                   Field (&fields)[FIELDS_MAX_SIZE],
                   Parameter (&parameters)[PARAMETERS_MAX_SIZE],
                   Method (&methods)[METHODS_MAX_SIZE],
                   Callback (&callbacks)[CALLBACKS_MAX_SIZE]);

  // Fields
  template <typename T>
  Field & createField(const ConstantString & field_name,
                     const T & default_value);
  template <typename T, size_t N>
  Field & createField(const ConstantString & field_name,
                     const T (&default_value)[N]);
  Field & field(const ConstantString & field_name);
  void setFieldsToDefaults();

  // Parameters
  Parameter & createParameter(const ConstantString & parameter_name);
  Parameter & parameter(const ConstantString & parameter_name);
  Parameter & copyParameter(Parameter & parameter,const ConstantString & parameter_name);

  // Methods
  Method & createMethod(const ConstantString & method_name);
  Method & method(const ConstantString & method_name);
  Method & copyMethod(Method & method,const ConstantString & method_name);

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
