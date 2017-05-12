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

## API Verbose

```json
{
  "id":"getApiVerbose",
  "result":{
    "firmware":["ModularServer"],
    "functions":[
      {
        "name":"getDeviceId",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"getDeviceInfo",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"getInterruptInfo",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"detachAllInterrupts",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"getApi",
        "firmware":"ModularServer",
        "parameters":[
          "firmware"
        ],
        "result_type":"object"
      },
      {
        "name":"getApiVerbose",
        "firmware":"ModularServer",
        "parameters":[
          "firmware"
        ],
        "result_type":"object"
      },
      {
        "name":"getPropertyDefaultValues",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"getAllPropertyDefaultValues",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"setPropertiesToDefaults",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"setAllPropertiesToDefaults",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"getPropertyValues",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"getAllPropertyValues",
        "firmware":"ModularServer",
        "parameters":[],
        "result_type":"object"
      }
    ],
    "parameters":[
      {
        "name":"firmware",
        "firmware":"ModularServer",
        "type":"array",
        "array_element_type":"string",
        "array_element_subset":[
          "ALL",
          "ModularServer",
          "ModularDeviceBase",
          "HBridgeController"
        ],
        "array_length_min":1,
        "array_length_max":8
      }
    ],
    "properties":[
      {
        "name":"serialNumber",
        "firmware":"ModularServer",
        "type":"long",
        "min":0,
        "max":65535,
        "value":0,
        "default_value":0,
        "functions":[
          "getValue",
          "setValue",
          "getDefaultValue",
          "setValueToDefault"
        ],
        "parameters":[
          "value"
        ]
      }
    ],
    "callbacks":[]
  }
}
```

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
