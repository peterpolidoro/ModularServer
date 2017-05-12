# ModularServer

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

This firmware requires about 4KB of SRAM and 75KB of flash memory, so
it needs at least Arduino Mega level resources. Known to work with the
Arduino Mega 2560, Teensy 3.2, Teensy 3.5, and Teensy 3.6.

## API

```json
{
  "id":"getApi",
  "result":{
    "firmware":["ModularServer"],
    "functions":[
      "getDeviceId",
      "getDeviceInfo",
      "getInterruptInfo",
      "detachAllInterrupts",
      "getApi",
      "getApiVerbose",
      "getPropertyDefaultValues",
      "getAllPropertyDefaultValues",
      "setPropertiesToDefaults",
      "setAllPropertiesToDefaults",
      "getPropertyValues",
      "getAllPropertyValues"
    ],
    "parameters":[
      "firmware"
    ],
    "properties":[
      "serialNumber"
    ],
    "callbacks":[]
  }
}
```

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
