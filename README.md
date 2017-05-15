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
        "parameters":[],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getDeviceInfo",
        "parameters":[],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getInterruptInfo",
        "parameters":[],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"detachAllInterrupts",
        "parameters":[]
      },
      {
        "name":"getApi",
        "parameters":[
          "firmware"
        ],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getApiVerbose",
        "parameters":[
          "firmware"
        ],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getPropertyDefaultValues",
        "parameters":[],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getAllPropertyDefaultValues",
        "parameters":[],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"setPropertiesToDefaults",
        "parameters":[]
      },
      {
        "name":"setAllPropertiesToDefaults",
        "parameters":[]
      },
      {
        "name":"getPropertyValues",
        "parameters":[],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getAllPropertyValues",
        "parameters":[],
        "result_info":{
          "type":"object"
        }
      }
    ],
    "parameters":[
      {
        "name":"firmware",
        "type":"array",
        "array_element_type":"string"
      }
    ],
    "properties":[
      {
        "name":"serialNumber",
        "type":"long",
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
