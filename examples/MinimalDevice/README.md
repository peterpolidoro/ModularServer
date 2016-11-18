#MinimalDevice

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

##Host Computer Interface

###Arduino Serial Monitor

Open the Serial Monitor in the Arduino IDE.

Set the baudrate to match the value in the Arduino sketch (115200).

Set the line ending to 'Newline'.

To get help information about the modular device, type a single
question mark ? into the input property and press the 'Send' button or
press the 'Enter' key.

```shell
?
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "device_id":{
      "name":"minimal_device",
      "form_factor":"5x3",
      "serial_number":0
    },
    "API":{
      "firmware":["all"],
      "methods":[
        "getDeviceId",
        "getDeviceInfo",
        "getApi",
        "getApiVerbose",
        "getPropertyDefaultValues",
        "getPropertyValues",
        "getMemoryFree"
      ],
      "parameters":[
        "firmware"
      ],
      "properties":[
        "serial_number"
      ],
      "callbacks":[
        "setPropertiesToDefaults"
      ]
    }
  }
}
```

"methods" is an array of user methods. To execute a method, simply
type it into the input property and press the 'Send' button or press the
'Enter' key.

Example Method (This method only exists on AVR processors):

```shell
getMemoryFree
```

Example Response:

```json
{
  "id":"getMemoryFree",
  "result":4800
}
```

To get more verbose help about the modular device, including
information about the parameters each method takes, type two question
marks ?? into the input property and press the 'Send' button or press the
'Enter' key.

```shell
??
```

Example Response:

```json
{
  "id":"??",
  "result":{
    "device_id":{
      "name":"minimal_device",
      "form_factor":"5x3",
      "serial_number":0
    },
    "device_info":{
      "processor":"ATmega2560",
      "hardware":[
        {
          "name":"Mega2560"
        }
      ],
      "firmware":[
        {
          "name":"ModularServer",
          "version":"2.0.0"
        },
        {
          "name":"MinimalDevice",
          "version":"1.0.0"
        }
      ]
    },
    "API":{
      "firmware":["all"],
      "methods":[
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
          "name":"getPropertyValues",
          "firmware":"ModularServer",
          "parameters":[],
          "result_type":"object"
        },
        {
          "name":"getMemoryFree",
          "firmware":"ModularServer",
          "parameters":[],
          "result_type":"long"
        }
      ],
      "parameters":[
        {
          "name":"firmware",
          "firmware":"ModularServer",
          "type":"array",
          "array_element_type":"string",
          "array_element_subset":[
            "all",
            "ModularServer",
            "MinimalDevice"
          ],
          "array_length_min":1,
          "array_length_max":8
        }
      ],
      "properties":[
        {
          "name":"serial_number",
          "firmware":"ModularServer",
          "type":"long",
          "min":0,
          "max":65535,
          "value":0,
          "default_value":0,
          "methods":[
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
      "callbacks":[
        {
          "name":"setPropertiesToDefaults",
          "firmware":"ModularServer",
          "properties":[],
          "parameters":[],
          "result_type":null
        }
      ]
    }
  }
}
```

Example Method with Parameters:

```shell
getApi
```

Example Response:

```json
{
  "id":"getApi",
  "error":{
    "message":"Invalid params",
    "data":"Incorrect number of parameters. 0 given. 1 needed.",
    "code":-32602
  }
}
```

When a method executes successfully, the response will contain a
"result" property. In some cases the result may be null, but the method
execution was still successful. When there is an error, there will not
exist a "result" property, but there will exist an "error" property.

To get more information about a method, enter the method followed by
a question mark.

Example Method Help:

```shell
getApi ?
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "name":"getApi",
    "firmware":"ModularServer",
    "parameters":[
      "firmware"
    ],
    "result_type":"object"
  }
}
```

Or you can enter a question mark followed by the method.

Example Method Help:

```shell
? getApi
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "name":"getApi",
    "firmware":"ModularServer",
    "parameters":[
      "firmware"
    ],
    "result_type":"object"
  }
}
```

To get more verbose information about all of the parameters a method
takes, enter the method followed by two questions marks.

```shell
getApi ??
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "name":"getApi",
    "firmware":"ModularServer",
    "parameters":[
      {
        "name":"firmware",
        "firmware":"ModularServer",
        "type":"array",
        "array_element_type":"string",
        "array_element_subset":[
          "all",
          "ModularServer",
          "MinimalDevice"
        ],
        "array_length_min":1,
        "array_length_max":8
      }
    ],
    "result_type":"object"
  }
}
```

Example Method:

```shell
getApi ["MinimalDevice"]
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["MinimalDevice"],
    "methods":[],
    "parameters":[],
    "properties":[],
    "callbacks":[]
  }
}
```

Properties are special methods that get and set field values that are
stored on the device, even after the device is powered off.

Properties have their own set of methods with their own set of
property method parameters.

Example Property:
```shell
? serial_number
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "name":"serial_number",
    "firmware":"ModularServer",
    "type":"long",
    "min":0,
    "max":65535,
    "value":0,
    "default_value":0,
    "methods":[
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

To get information about a property method:

```shell
? serial_number setValue
```

```json
{
  "id":"?",
  "result":{
    "name":"setValue",
    "firmware":"ModularServer",
    "parameters":[
      "value"
    ],
    "result_type":null
  }
}
```

To get information about a property method parameter:

```shell
? serial_number setValue value
```

```json
{
  "id":"?",
  "result":{
    "name":"value",
    "firmware":"ModularServer",
    "type":"long",
    "min":0,
    "max":65535
  }
}
```

Example property method call:

```shell
serial_number setValue -1
```

```json
{
  "id":"serial_number",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in range: 0 <= value <= 65535",
    "code":-32602
  }
}
```

Example property method call:

```shell
serial_number setValue 32
```

```json
{
  "id":"serial_number",
  "result":null
}
```

The serial number setting persists even after the device is powered
off. The serial number is used to differentiate several identical
devices connected to a single host machine at one time.

Example Method:

```shell
getPropertyValues
```

Example Response:

```json
{
  "id":"getPropertyValues",
  "result":{
    "serial_number":32
  }
}
```

To reset the serial number to the default value, use the
setValueToDefault property method.

Callbacks are methods that take no parameters and return null. They
can be used like methods or called from interrupts.

Example Callback:

```shell
? setPropertiesToDefaults
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "name":"setPropertiesToDefaults",
    "firmware":"ModularServer",
    "properties":[],
    "parameters":[],
    "result_type":null
  }
}
```

Example Callback:

```shell
setPropertiesToDefaults
```

Example Response:

```json
{
  "id":"setPropertiesToDefaults",
  "result":null
}
```

Example Method:

```shell
getPropertyValues
```

Example Response:

```json
{
  "id":"getPropertyValues",
  "result":{
    "serial_number":0
  }
}
```

Use the getDeviceId method to get a unique set of values to identify
the device.

Example:

```shell
getDeviceId
```

Example Response:

```json
{
  "id":"getDeviceId",
  "result":{
    "name":"minimal_device",
    "form_factor":"5x3",
    "serial_number":0
  }
}
```

The serial\_number property can be changed to uniquely identify devices
with the same name and form\_factor.

Use the getDeviceInfo method to get information about the hardware and
firmware of the device.

Example:

```shell
getDeviceInfo
```

Example Response:

```json
{
  "id":"getDeviceInfo",
  "result":{
    "processor":"ATmega2560",
    "hardware":[
      {
        "name":"Mega2560"
      }
    ],
    "firmware":[
      {
        "name":"ModularServer",
        "version":"2.0.0"
      },
      {
        "name":"MinimalDevice",
        "version":"1.0.0"
      }
    ]
  }
}
```

Every method, parameter, and property belongs to one firmware set.

To get the API limited to one or more firmware sets, use the getApi
method.

Example:

```shell
getApi ["all"]
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["all"],
    "methods":[
      "getDeviceId",
      "getDeviceInfo",
      "getApi",
      "getApiVerbose",
      "getPropertyDefaultValues",
      "getPropertyValues",
      "getMemoryFree"
    ],
    "parameters":[
      "firmware"
    ],
    "properties":[
      "serial_number"
    ],
    "callbacks":[
      "setPropertiesToDefaults"
    ]
  }
}
```

Example:

```shell
getApi ["MinimalDevice"]
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["MinimalDevice"],
    "methods":[],
    "parameters":[],
    "properties":[],
    "callbacks":[]
  }
}
```

###Python

Example Python session:

```python
from modular_device import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '5x3', 'name': 'minimal_device', 'serial_number': 0}
dev.get_procedures()
['get_memory_free',
 'get_api',
 'get_api_verbose',
 'get_property_values',
 'get_device_id',
 'serial_number',
 'get_property_default_values',
 'set_properties_to_defaults',
 'get_device_info']
dev.get_memory_free()
4800
dev.get_api()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 1 needed., code: -32602
dev.get_api('?')
{'firmware': 'ModularServer',
 'name': 'getApi',
 'parameters': ['firmware'],
 'result_type': 'object'}
dev.get_api(['MinimalDevice'])
{'callbacks': [],
 'firmware': ['MinimalDevice'],
 'methods': [],
 'parameters': [],
 'properties': []}
dev.serial_number('setValue',-1)
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in range: 0 <= value <= 65535, code: -32602
dev.serial_number('setValue',32)
dev.get_property_values()
{'serial_number': 32}
result = dev.call_server_procedure('?')
32
dev.convert_to_json(result['device_id'])
'{"serial_number":12,"name":"minimal_device","form_factor":"5x3"}'
dev.send_json_request('["set_properties_to_defaults"]')
dev.serial_number('getValue')
0
dev.get_api(["MinimalDevice"])
{'callbacks': [],
 'firmware': ['MinimalDevice'],
 'methods': [],
 'parameters': [],
 'properties': []}
```

For more details on the Python interface:

<https://github.com/janelia-pypi/modular_client_python>

###Matlab

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
  getDeviceId
  getDeviceInfo
  getApi
  getApiVerbose
  getPropertyDefaultValues
  setPropertiesToDefaults
  getPropertyValues
  getPropertyValue
  getPropertyElementValue
  setPropertyValue
  setPropertyElementValue
  setAllPropertyElementValues
  getMemoryFree
dev.getMemoryFree()
ans =
  4800
dev.getPropertyValue()
(from server) message: Invalid params, Incorrect number of parameters. 0 given. 1 needed., code: -32602
method_info = dev.getPropertyValue('?')
method_info
ans =
  name: 'getPropertyValue'
  firmware: 'ModularServer'
  parameters: {'property_name'}
  result_type: 'value'
dev.getPropertyValue('serial_number')
ans =
  0
dev.setPropertyValue('serial_number',-1)
(from server) message: Invalid params, Parameter value out of range: 0 <= serial_number <= 65535, code: -32602
dev.setPropertyValue('serial_number',13);
result = dev.callServerMethod('?');
result.device_id.serial_number
ans =
  13
json = dev.convertToJson(result.device_info.firmware_version)
json =
  {"major": 0,"minor": 1,"patch": 0}
dev.sendJsonRequest('["setPropertiesToDefaults"]')
dev.getPropertyValue('serial_number')
ans =
  0
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
