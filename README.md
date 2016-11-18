#ModularServer

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

This firmware requires at least 3KB of SRAM and 50KB of flash
memory, so it needs at least Arduino Mega level resources. Known to
work with the Arduino Mega 2560 and Teensy 3.2.

[Examples](./examples)

##API

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
      "serial_number"
    ]
  }
}
```

##More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

##Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
