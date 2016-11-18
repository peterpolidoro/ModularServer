#PropertyTester

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
      "name":"property_tester",
      "form_factor":"5x3",
      "serial_number":0
    },
    "API":{
      "firmware":["all"],
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
        "getMemoryFree",
        "getDoubled",
        "getBool",
        "getLongArrayFixed",
        "getLongArrayVariable",
        "setLongArrayFixed",
        "setLongArrayVariable",
        "setLongArrayParameter",
        "getStringAll",
        "getStringSome",
        "getCount",
        "getCountArray",
        "getDirection",
        "getDirectionArray",
        "checkMode",
        "incrementMode"
      ],
      "parameters":[
        "firmware",
        "property_name",
        "property_value",
        "property_element_index",
        "long_array_parameter",
        "length_parameter",
        "count",
        "count_array",
        "direction",
        "direction_array"
      ],
      "properties":[
        "serial_number",
        "double",
        "bool",
        "long_array",
        "double_array",
        "bool_array",
        "string",
        "odd",
        "mode",
        "odd_array"
      ],
      "callbacks":[]
    }
  }
}
```

"functions" is an array of user functions. To execute a function, simply
type it into the input property and press the 'Send' button or press the
'Enter' key.

Example Function:

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

Example Function:

```shell
getPropertyValues
```

Example Response:

```json
{
  "id":"getPropertyValues",
  "result":{
    "serial_number":0,
    "double":3.141590,
    "bool":false,
    "long_array":[
      5,
      4,
      3,
      2
    ],
    "double_array":[
      -1.100000,
      2.200000,
      3.300000
    ],
    "bool_array":[
      false,
      true
    ],
    "string":"abcdef",
    "odd":5,
    "mode":"RISING",
    "odd_array":[
      1,
      5
    ]
  }
}
```

Example Function:

```shell
getPropertyValue long_array
```

Example Response:

```json
{
  "id":"getPropertyValue",
  "result":[
    5,
    4,
    3,
    2
  ]
}
```

Example Function:

```shell
getPropertyElementValue double_array 1
```

Example Response:

```json
{
  "id":"getPropertyElementValue",
  "result":2.200000
}
```

Example Function:

```shell
setPropertyValue bool_array [false,false]
```

Example Response:

```json
{
  "id":"setPropertyValue",
  "result":null
}
```

Example Function:

```shell
setPropertyValue string asdfghjkl
```

Example Response:

```json
{
  "id":"setPropertyValue",
  "result":null
}
```

Example Function:

```shell
getPropertyElementValue string 3
```

Example Response:

```json
{
  "id":"getPropertyElementValue",
  "result":"f"
}
```

Example Function:

```shell
setPropertyElementValue string 3 X
```

Example Response:

```json
{
  "id":"setPropertyElementValue",
  "result":null
}
```

Example Function:

```shell
setPropertyValue odd 2
```

Example Response:

```json
{
  "id":"setPropertyValue",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in subset: [1,3,5,7,9]",
    "code":-32602
  }
}
```

Example Function:

```shell
setPropertyValue odd 7
```

Example Response:

```json
{
  "id":"setPropertyValue",
  "result":null
}
```

Example Function:

```shell
setPropertyValue mode test
```

Example Response:

```json
{
  "id":"setPropertyValue",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in subset: [RISING,FALLING,CHANGE]",
    "code":-32602
  }
}
```

Example Function:

```shell
setPropertyValue mode CHANGE
```

Example Response:

```json
{
  "id":"setPropertyValue",
  "result":null
}
```

Example Function:

```shell
setPropertyElementValue mode 3 t
```

Example Response:

```json
{
  "id":"setPropertyElementValue",
  "error":{
    "message":"Invalid params",
    "data":"Cannot set element in string property with subset.",
    "code":-32602
  }
}
```

Example Function:

```shell
setAllPropertyElementValues odd_array 2
```

Example Response:

```json
{
  "id":"setAllPropertyElementValues",
  "error":{
    "message":"Invalid params",
    "data":"Array parameter element value not valid. Value not in subset: [1,3,5,7,9]",
    "code":-32602
  }
}
```

Example Function:

```shell
setAllPropertyElementValues odd_array 9
```

Example Response:

```json
{
  "id":"setAllPropertyElementValues",
  "result":null
}
```

Example Function:

```shell
getPropertyValues
```

Example Response:

```json
{
  "id":"getPropertyValues",
  "result":{
    "serial_number":0,
    "double":3.141590,
    "bool":false,
    "long_array":[
      5,
      4,
      3,
      2
    ],
    "double_array":[
      -1.100000,
      2.200000,
      3.300000
    ],
    "bool_array":[
      false,
      false
    ],
    "string":"asdXghjkl",
    "odd":7,
    "mode":"CHANGE",
    "odd_array":[
      9,
      9
    ]
  }
}
```

Use the getDeviceId function to get a unique set of values to identify
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
    "name":"property_tester",
    "form_factor":"5x3",
    "serial_number":0
  }
}
```

The serial\_number property can be changed to uniquely identify devices
with the same name and form\_factor.

Use the getDeviceInfo function to get information about the hardware and
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
        "version":"1.0.0"
      },
      {
        "name":"PropertyTester",
        "version":"1.0.0"
      }
    ]
  }
}
```

Every function, parameter, and property belongs to one firmware set.

To get the API limited to one or more firmware sets, use the getApi
function.

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
      "getMemoryFree",
      "getDoubled",
      "getBool",
      "getLongArrayFixed",
      "getLongArrayVariable",
      "setLongArrayFixed",
      "setLongArrayVariable",
      "setLongArrayParameter",
      "getStringAll",
      "getStringSome",
      "getCount",
      "getCountArray",
      "getDirection",
      "getDirectionArray",
      "checkMode",
      "incrementMode"
    ],
    "parameters":[
      "firmware",
      "property_name",
      "property_value",
      "property_element_index",
      "long_array_parameter",
      "length_parameter",
      "count",
      "count_array",
      "direction",
      "direction_array"
    ],
    "properties":[
      "serial_number",
      "double",
      "bool",
      "long_array",
      "double_array",
      "bool_array",
      "string",
      "odd",
      "mode",
      "odd_array"
    ],
    "callbacks":[]
  }
}
```

Example:

```shell
getApi ["PropertyTester"]
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["PropertyTester"],
    "functions":[
      "getDoubled",
      "getBool",
      "getLongArrayFixed",
      "getLongArrayVariable",
      "setLongArrayFixed",
      "setLongArrayVariable",
      "setLongArrayParameter",
      "getStringAll",
      "getStringSome",
      "getCount",
      "getCountArray",
      "getDirection",
      "getDirectionArray",
      "checkMode",
      "incrementMode"
    ],
    "parameters":[
      "long_array_parameter",
      "length_parameter",
      "count",
      "count_array",
      "direction",
      "direction_array"
    ],
    "properties":[
      "double",
      "bool",
      "long_array",
      "double_array",
      "bool_array",
      "string",
      "odd",
      "mode",
      "odd_array"
    ],
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
{'form_factor': '5x3', 'name': 'property_tester', 'serial_number': 0}
dev.get_functions()
['get_count_array',
 'get_long_array_fixed',
 'set_all_property_element_values',
 'get_direction',
 'get_property_default_values',
 'get_count',
 'set_property_value',
 'get_string_some',
 'get_device_info',
 'get_memory_free',
 'set_property_element_value',
 'set_long_array_variable',
 'set_properties_to_defaults',
 'get_device_id',
 'increment_mode',
 'get_direction_array',
 'set_long_array_fixed',
 'set_long_array_parameter',
 'get_string_all',
 'get_long_array_variable',
 'check_mode',
 'get_property_value',
 'get_property_element_value',
 'get_api',
 'get_api_verbose',
 'get_doubled',
 'get_bool',
 'set_property_to_default',
 'get_property_values']
dev.set_properties_to_defaults()
dev.get_property_values()
{'bool': False,
 'bool_array': [False, True],
 'double': 3.14159,
 'double_array': [-1.1, 2.2, 3.3],
 'long_array': [5, 4, 3, 2],
 'mode': 'RISING',
 'odd': 5,
 'odd_array': [1, 5],
 'serial_number': 0,
 'string': 'abcdef'}
dev.get_property_value('long_array')
[5, 4, 3, 2]
dev.get_property_element_value('double_array',1)
2.2
dev.set_property_value('bool_array',[False,False])
dev.set_property_value('string','asdfghjkl')
dev.get_property_element_value('string',3)
'f'
dev.set_property_element_value('string',3,'X')
dev.set_property_value('odd',2)
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in subset: [1,3,5,7,9], code: -32602
dev.set_property_value('odd',7)
dev.set_property_value('mode','test')
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in subset: [RISING,FALLING,CHANGE], code: -32602
dev.set_property_value('mode','CHANGE')
dev.set_property_element_value('mode',3,'t')
IOError: (from server) message: Invalid params, data: Cannot set element in string property with subset., code: -32602
dev.set_all_property_element_values('odd_array',2)
IOError: (from server) message: Invalid params, data: Array parameter element value not valid. Value not in subset: [1,3,5,7,9], code: -32602
dev.set_all_property_element_values('odd_array',9)
dev.get_property_values()
{'bool': False,
 'bool_array': [False, False],
 'double': 3.14159,
 'double_array': [-1.1, 2.2, 3.3],
 'long_array': [5, 4, 3, 2],
 'mode': 'CHANGE',
 'odd': 7,
 'odd_array': [9, 9],
 'serial_number': 0,
 'string': 'asdXghjkl'}
dev.get_api(["PropertyTester"])
{'properties': ['double',
  'bool',
  'long_array',
  'double_array',
  'bool_array',
  'string',
  'odd',
  'mode',
  'odd_array'],
 'firmware': ['PropertyTester'],
 'callbacks': [],
 'functions': ['getDoubled',
  'getBool',
  'getLongArrayFixed',
  'getLongArrayVariable',
  'setLongArrayFixed',
  'setLongArrayVariable',
  'setLongArrayParameter',
  'getStringAll',
  'getStringSome',
  'getCount',
  'getCountArray',
  'getDirection',
  'getDirectionArray',
  'checkMode',
  'incrementMode'],
 'parameters': ['long_array_parameter',
  'length_parameter',
  'count',
  'count_array',
  'direction',
  'direction_array']}
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
  name: 'property_tester'
  form_factor: '5x3'
  serial_number: 0
dev.getFunctions()                 % get device functions
  Modular Device Functions
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
  getDoubled
  getBool
  getLongArrayFixed
  getLongArrayVariable
  setLongArrayFixed
  setLongArrayVariable
  setLongArrayParameter
  getStringAll
  getStringSome
  getCount
  getCountArray
  getDirection
  getDirectionArray
  checkMode
  incrementMode
dev.getPropertyValues()
  ans =
  serial_number: 0
  double: 3.1416
  bool: 0
  long_array: [5 4 3 2]
  double_array: [-1.1000 2.2000 3.3000]
  bool_array: [0 1]
  string: 'abcdef'
  odd: 5
  mode: 'RISING'
  odd_array: [1 5]
dev.getPropertyValue('long_array')
ans =
  5   4   3   2
dev.getPropertyElementValue('double_array',1)
ans =
  2.2000
dev.setPropertyValue('bool_array',[0,0]);
dev.setPropertyValue('string','asdfghjkl');
dev.getPropertyElementValue('string',3)
ans =
  f
dev.setPropertyElementValue('string',3,'X');
dev.getPropertyValues()
ans =
  serial_number: 0
  double: 3.1416
  bool: 0
  long_array: [5 4 3 2]
  double_array: [-1.1000 2.2000 3.3000]
  bool_array: [0 0]
  string: 'asdXghjkl'
  odd: 5
  mode: 'RISING'
  odd_array: [1 5]
dev.setPropertiesToDefaults();
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
