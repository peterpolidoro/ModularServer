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
        "result_type":"object"
      },
      {
        "name":"getDeviceInfo",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"getInterruptInfo",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"detachAllInterrupts",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"getApi",
        "parameters":[
          "firmware"
        ],
        "result_type":"object"
      },
      {
        "name":"getApiVerbose",
        "parameters":[
          "firmware"
        ],
        "result_type":"object"
      },
      {
        "name":"getPropertyDefaultValues",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"getAllPropertyDefaultValues",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"setPropertiesToDefaults",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"setAllPropertiesToDefaults",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"getPropertyValues",
        "parameters":[],
        "result_type":"object"
      },
      {
        "name":"getAllPropertyValues",
        "parameters":[],
        "result_type":"object"
      }
    ],
    "parameters":[
      {
        "name":"firmware",
        "type":"array"
      }
    ],
    "properties":[
      {
        "name":"serialNumber",
        "type":"long"
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
