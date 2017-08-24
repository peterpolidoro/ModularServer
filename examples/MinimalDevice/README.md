# MinimalDevice

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Host Computer Interface

### Arduino Serial Monitor

Open the Serial Monitor in the Arduino IDE.

Set the baudrate to match the value in the Arduino sketch (115200).

Set the line ending to 'Newline'.

To get help information about the modular device, type a single
question mark ? into the input property and press the 'Send' button or
press the 'Enter' key.

Request:

```shell
?
```

Response:

```json
{
  "id":"?",
  "result":{
    "device_id":{
      "name":"minimal_device",
      "form_factor":"3x2",
      "serial_number":0
    },
    "api":{
      "firmware":["MinimalDevice"],
      "verbosity":"NAMES"
    }
  }
}
```

The form\_factor and serial\_number may be different on your board than the ones
shown above.

The minimal device adds no functions, parameters, properties, or callbacks to
the ones provided by the parent class ModularServer.

To get more verbose help about the modular device, including all API firmware,
type two question marks ?? into the input property and press the 'Send' button
or press the 'Enter' key.

Request:

```shell
??
```

Response:

```json
{
  "id":"??",
  "result":{
    "device_id":{
      "name":"minimal_device",
      "form_factor":"3x2",
      "serial_number":0
    },
    "api":{
      "firmware":["ALL"],
      "verbosity":"NAMES",
      "functions":[
        "getDeviceId",
        "getDeviceInfo",
        "getApi",
        "getPropertyDefaultValues",
        "setPropertiesToDefaults",
        "getPropertyValues",
        "getInterruptInfo",
        "detachAllInterrupts"
      ],
      "parameters":[
        "firmware",
        "verbosity"
      ],
      "properties":[
        "serialNumber"
      ]
    }
  }
}
```

"functions" is an array of user functions. To execute a function, simply
type it into the input property and press the 'Send' button or press the
'Enter' key.

After uploading new firmware to the device for the first time, usually you want
to set all properties to their default values, so the values will be known and
valid.

Request:

```shell
setPropertiesToDefaults ["ALL"]
```

Response:

```json
{
  "id":"setPropertiesToDefaults",
  "result":null
}
```

Request:

```shell
getApi
```

Response:

```json
{
  "id":"getApi",
  "error":{
    "message":"Invalid params",
    "data":"Incorrect number of parameters. 0 given. 2 needed.",
    "code":-32602
  }
}
```

When a function executes successfully, the response will contain a
"result" property. In some cases the result may be null, but the function
execution was still successful. When there is an error, there will not
exist a "result" property, but there will exist an "error" property.

To get more information about a function, enter the function followed by
a question mark.

Request:

```shell
getApi ?
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "name":"getApi",
    "firmware":"ModularServer",
    "parameters":[
      "verbosity",
      "firmware"
    ],
    "result_info":{
      "type":"object"
    }
  }
}
```

Or you can enter a question mark followed by the function.

Request:

```shell
? getApi
```

Response:

```json
{
  "id":"?",
  "result":{
    "name":"getApi",
    "firmware":"ModularServer",
    "parameters":[
      "verbosity",
      "firmware"
    ],
    "result_info":{
      "type":"object"
    }
  }
}

```

To get more verbose information about all of the parameters a function
takes, enter the function followed by two questions marks.

Request:

```shell
getApi ??
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "name":"getApi",
    "firmware":"ModularServer",
    "parameters":[
      {
        "name":"verbosity",
        "type":"string",
        "subset":[
          "NAMES",
          "GENERAL",
          "DETAILED"
        ]
      },
      {
        "name":"firmware",
        "type":"array",
        "array_element_type":"string",
        "array_element_subset":[
          "ALL",
          "ModularServer",
          "MinimalDevice"
        ],
        "array_length_min":1,
        "array_length_max":2
      }
    ],
    "result_info":{
      "type":"object"
    }
  }
}
```

Request:

```shell
getApi GENERAL ["ALL"]
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["ALL"],
    "verbosity":"GENERAL",
    "functions":[
      {
        "name":"getDeviceId",
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getDeviceInfo",
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getApi",
        "parameters":[
          "verbosity",
          "firmware"
        ],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getPropertyDefaultValues",
        "parameters":[
          "firmware"
        ],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"setPropertiesToDefaults",
        "parameters":[
          "firmware"
        ]
      },
      {
        "name":"getPropertyValues",
        "parameters":[
          "firmware"
        ],
        "result_info":{
          "type":"object"
        }
      },
      {
        "name":"getInterruptInfo",
        "result_info":{
          "type":"array",
          "array_element_type":"object"
        }
      },
      {
        "name":"detachAllInterrupts"
      }
    ],
    "parameters":[
      {
        "name":"firmware",
        "type":"array",
        "array_element_type":"string"
      },
      {
        "name":"verbosity",
        "type":"string"
      }
    ],
    "properties":[
      {
        "name":"serialNumber",
        "type":"long",
        "functions":[
          {
            "name":"getValue",
            "result_info":{
              "type":"long"
            }
          },
          {
            "name":"setValue",
            "parameters":[
              "value"
            ],
            "result_info":{
              "type":"long"
            }
          },
          {
            "name":"getDefaultValue",
            "result_info":{
              "type":"long"
            }
          },
          {
            "name":"setValueToDefault",
            "result_info":{
              "type":"long"
            }
          }
        ],
        "parameters":[
          {
            "name":"value",
            "type":"long"
          }
        ]
      }
    ]
  }
}
```

Properties are special functions that get and set field values that are
stored on the device, even after the device is powered off.

Properties have their own set of functions with their own set of
property function parameters.

Request:

```shell
serialNumber ?
```

Response:

```json
{
  "id":"serialNumber",
  "result":{
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
}
```

Request:

```shell
serialNumber setValue ?
```

Response:

```json
{
  "id":"serialNumber",
  "result":{
    "name":"setValue",
    "firmware":"ModularServer",
    "parameters":[
      "value"
    ],
    "result_info":{
      "type":"long"
    }
  }
}
```

Request:

```shell
serialNumber setValue value ?
```

Response:

```json
{
  "id":"serialNumber",
  "result":{
    "name":"value",
    "firmware":"ModularServer",
    "type":"long",
    "min":0,
    "max":65535
  }
}
```

Request:

```shell
serialNumber setValue -1
```

Response:

```json
{
  "id":"serialNumber",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in range: 0 <= value <= 65535",
    "code":-32602
  }
}
```

Request:

```shell
serialNumber setValue 32
```

Response:

```json
{
  "id":"serialNumber",
  "result":32
}
```

The serial number setting persists even after the device is powered
off. The serial number is used to differentiate several identical
devices connected to a single host machine at one time.

Request:

```shell
getPropertyValues ["ALL"]
```

Response:

```json
{
  "id":"getPropertyValues",
  "result":{
    "serialNumber":32
  }
}
```

Use the setPropertiesToDefault function to set all properties to their default
values.

Or to reset the just one property to the default value, use the
setValueToDefault property function.

Request:

```shell
serialNumber setValueToDefault
```

Response:

```json
{
  "id":"serialNumber",
  "result":0
}
```

Use the getDeviceId function to get a unique set of values to identify
the device.

Request:

```shell
getDeviceId
```

Response:

```json
{
  "id":"getDeviceId",
  "result":{
    "name":"minimal_device",
    "form_factor":"3x2",
    "serial_number":0
  }
}
```

The serial\_number property can be changed to uniquely identify devices
with the same name and form\_factor.

Use the getDeviceInfo function to get information about the hardware and
firmware of the device.

Request:

```shell
getDeviceInfo
```

Response:

```json
{
  "id":"getDeviceInfo",
  "result":{
    "processor":"MK20DX256",
    "hardware":[
      {
        "name":"Teensy",
        "version":"3.2"
      }
    ],
    "firmware":[
      {
        "name":"ModularServer",
        "version":"3.0.0"
      },
      {
        "name":"MinimalDevice",
        "version":"2.0.0"
      }
    ]
  }
}
```

Every function, parameter, property, and callback belongs to one firmware set.

To get the API limited to one or more firmware sets, use the getApi
function.

Request:

```shell
getApi NAMES ["ALL"]
```

Response:

```json

{
  "id":"getApi",
  "result":{
    "firmware":["ALL"],
    "verbosity":"NAMES",
    "functions":[
      "getDeviceId",
      "getDeviceInfo",
      "getApi",
      "getPropertyDefaultValues",
      "setPropertiesToDefaults",
      "getPropertyValues",
      "getInterruptInfo",
      "detachAllInterrupts"
    ],
    "parameters":[
      "firmware",
      "verbosity"
    ],
    "properties":[
      "serialNumber"
    ]
  }
}
```

Request:

```shell
getApi NAMES ["MinimalDevice"]
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["MinimalDevice"],
    "verbosity":"NAMES"
  }
}
```

### Python

Example Python session:

```python
from modular_client import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '3x2', 'name': 'minimal_device', 'serial_number': 0}
dev.get_methods()
['get_interrupt_info',
 'get_api',
 'serial_number',
 'get_property_values',
 'get_device_id',
 'detach_all_interrupts',
 'get_property_default_values',
 'set_properties_to_defaults',
 'get_device_info']
dev.set_properties_to_defaults(['ALL'])
dev.get_api()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 2 needed., code: -32602
dev.get_api('?')
{'firmware': 'ModularServer',
 'name': 'getApi',
 'parameters': ['verbosity', 'firmware'],
 'result_info': {'type': 'object'}}
dev.get_api('NAMES',['ALL'])
{'firmware': ['ALL'],
 'functions': ['getDeviceId',
  'getDeviceInfo',
  'getApi',
  'getPropertyDefaultValues',
  'setPropertiesToDefaults',
  'getPropertyValues',
  'getInterruptInfo',
  'detachAllInterrupts'],
 'parameters': ['firmware', 'verbosity'],
 'properties': ['serialNumber'],
 'verbosity': 'NAMES'}
dev.serial_number('setValue',-1)
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in range: 0 <= value <= 65535, code: -32602
dev.serial_number('setValue',32)
32
dev.get_property_values(['ALL'])
{'serialNumber': 32}
result = dev.call_get_result('?')
result['device_id']['serial_number']
32
dev.convert_to_json(result['device_id'])
'{"serial_number":32,"name":"minimal_device","form_factor":"3x2"}'
dev.send_json_request('["set_properties_to_defaults",["ALL"]]')
dev.serial_number('getValue')
0
```

For more details on the Python interface:

<https://github.com/janelia-pypi/modular_client_python>

### Matlab

Example Matlab session:

```matlab
% Linux and Mac OS X
ls /dev/tty*
serial_port = '/dev/ttyACM0'     % example Linux serial port
serial_port = '/dev/tty.usbmodem262471' % example Mac OS X serial port
% Windows
getAvailableComPorts()
ans =
  'COM1'
  'COM4'
serial_port = 'COM4'             % example Windows serial port
dev = ModularClient(serial_port) % creates a device object
dev.open()                       % opens a serial connection to the device
dev.getDeviceId()
ans =
  name: 'minimal_device'
  form_factor: '5x3'
  serial_number: 0
dev.getMethods()                 % get device methods
  Modular Device Methods
  ---------------------
dev.getApi()
(from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 1 needed.,
code: -32602
dev.getApi('?')
ans =
dev.getApi('firmware','?')
ans =
dev.getApi({'MinimalDevice'})
ans =
dev.serialNumber('setValue',-1)
(from server) message: Invalid params, data: Parameter value not valid. Value not in range: 0 <=
value <= 65535, code: -32602
dev.serialNumber('setValue',32)
ans =
  32
dev.getPropertyValues()
ans =
  serialNumber: 32
result = dev.callServerMethod('?');
result.device_id.serial_number
ans =
  32
json = dev.convertToJson(result.device_id)
{"name": "minimal_device","form_factor": "5x3","serial_number": 32}
dev.sendJsonRequest('["setPropertiesToDefaults"]')
dev.serialNumber('getValue')
ans =
  0
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

## Installation

<https://github.com/janelia-arduino/arduino-libraries>
