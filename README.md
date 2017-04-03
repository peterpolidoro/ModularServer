# ModularServer

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

This firmware requires about 4KB of SRAM and 75KB of flash memory, so
it needs at least Arduino Mega level resources. Known to work with the
Arduino Mega 2560, Teensy 3.2, Teensy 3.5, and Teensy 3.6.

[Examples](./examples)

## API

```json
{
  "id":"getApi",
  "result":{
    "firmware":["ModularServer"],
    "functions":[
      "getDeviceId",
      "getDeviceInfo",
      "getApi",
      "getApiVerbose",
      "getPropertyDefaultValues",
      "setPropertiesToDefaults",
      "setPropertyToDefault",
      "getPropertyValues",
      "getPropertyValue",
      "getPropertyElementValue",
      "setPropertyValue",
      "setPropertyElementValue",
      "setAllPropertyElementValues",
      "getMemoryFree"
    ],
    "parameters":[
      "firmware",
      "property_name",
      "property_value",
      "property_element_index"
    ],
    "properties":[
      "serialNumber"
    ]
  }
}
```

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
