# ModularServer

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

Modular device server library.

This firmware requires at least Teensy 3.x level resources. Known to work with
Teensy 3.2, Teensy 3.5, and Teensy 3.6.

## API NAMES

```json
{
  "id": "getApi",
  "result": {
    "firmware": [
      "ModularServer"
    ],
    "verbosity": "NAMES",
    "functions": [
      "getDeviceId",
      "getDeviceInfo",
      "getApi",
      "getPropertyDefaultValues",
      "setPropertiesToDefaults",
      "getPropertyValues",
      "getPinInfo",
      "setPinMode",
      "getPinValue",
      "setPinValue"
    ],
    "parameters": [
      "firmware",
      "verbosity",
      "pin_name",
      "pin_mode",
      "pin_value"
    ],
    "properties": [
      "serialNumber"
    ]
  }
}
```

## API GENERAL

[API GENERAL](./api/)

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
