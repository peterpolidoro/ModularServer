#FieldTester

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

##Host Computer Interface

###Arduino Serial Monitor

Open the Serial Monitor in the Arduino IDE.

Set the baudrate to match the value in the Arduino sketch (9600).

Set the line ending to 'Newline'.

To get help information about the modular device, type a single
question mark ? into the input field and press the 'Send' button or
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
      "name":"field_tester",
      "form_factor":"5x3",
      "serial_number":0
    },
    "API":{
      "firmware":["All"],
      "methods":[
        "getDeviceId",
        "getDeviceInfo",
        "getApi",
        "getApiVerbose",
        "getFieldDefaultValues",
        "setFieldsToDefaults",
        "setFieldToDefault",
        "getFieldValues",
        "getFieldValue",
        "getFieldElementValue",
        "setFieldValue",
        "setFieldElementValue",
        "setAllFieldElementValues",
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
        "field_name",
        "field_value",
        "field_element_index",
        "long_array_parameter",
        "length_parameter",
        "count",
        "count_array",
        "direction",
        "direction_array"
      ],
      "fields":[
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
      ]
    }
  }
}
```

"methods" is an array of user methods. To execute a method, simply
type it into the input field and press the 'Send' button or press the
'Enter' key.

Example Method:

```shell
setFieldsToDefaults
```

Example Response:

```json
{
  "id":"setFieldsToDefaults",
  "result":null
}
```

Example Method:

```shell
getFieldValues
```

Example Response:

```json
{
  "id":"getFieldValues",
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

Example Method:

```shell
getFieldValue long_array
```

Example Response:

```json
{
  "id":"getFieldValue",
  "result":[
    5,
    4,
    3,
    2
  ]
}
```

Example Method:

```shell
getFieldElementValue double_array 1
```

Example Response:

```json
{
  "id":"getFieldElementValue",
  "result":2.200000
}
```

Example Method:

```shell
setFieldValue bool_array [false,false]
```

Example Response:

```json
{
  "id":"setFieldValue",
  "result":null
}
```

Example Method:

```shell
setFieldValue string asdfghjkl
```

Example Response:

```json
{
  "id":"setFieldValue",
  "result":null
}
```

Example Method:

```shell
getFieldElementValue string 3
```

Example Response:

```json
{
  "id":"getFieldElementValue",
  "result":"f"
}
```

Example Method:

```shell
setFieldElementValue string 3 X
```

Example Response:

```json
{
  "id":"setFieldElementValue",
  "result":null
}
```

Example Method:

```shell
setFieldValue odd 2
```

Example Response:

```json
{
  "id":"setFieldValue",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in subset: [1,3,5,7,9]",
    "code":-32602
  }
}
```

Example Method:

```shell
setFieldValue odd 7
```

Example Response:

```json
{
  "id":"setFieldValue",
  "result":null
}
```

Example Method:

```shell
setFieldValue mode test
```

Example Response:

```json
{
  "id":"setFieldValue",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in subset: [RISING,FALLING,CHANGE]",
    "code":-32602
  }
}
```

Example Method:

```shell
setFieldValue mode CHANGE
```

Example Response:

```json
{
  "id":"setFieldValue",
  "result":null
}
```

Example Method:

```shell
setFieldElementValue mode 3 t
```

Example Response:

```json
{
  "id":"setFieldElementValue",
  "error":{
    "message":"Invalid params",
    "data":"Cannot set element in string field with subset.",
    "code":-32602
  }
}
```

Example Method:

```shell
setAllFieldElementValues odd_array 2
```

Example Response:

```json
{
  "id":"setAllFieldElementValues",
  "error":{
    "message":"Invalid params",
    "data":"Array parameter element value not valid. Value not in subset: [1,3,5,7,9]",
    "code":-32602
  }
}
```

Example Method:

```shell
setAllFieldElementValues odd_array 9
```

Example Response:

```json
{
  "id":"setAllFieldElementValues",
  "result":null
}
```

Example Method:

```shell
getFieldValues
```

Example Response:

```json
{
  "id":"getFieldValues",
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
    "name":"field_tester",
    "form_factor":"5x3",
    "serial_number":0
  }
}
```

The serial\_number field can be changed to uniquely identify devices
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
        "version":"1.0.0"
      },
      {
        "name":"FieldTester",
        "version":"1.0.0"
      }
    ]
  }
}
```

Every method, parameter, and field belongs to one firmware set.

To get the API limited to one or more firmware sets, use the getApi
method.

Example:

```shell
getApi ["All"]
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["All"],
    "methods":[
      "getDeviceId",
      "getDeviceInfo",
      "getApi",
      "getApiVerbose",
      "getFieldDefaultValues",
      "setFieldsToDefaults",
      "setFieldToDefault",
      "getFieldValues",
      "getFieldValue",
      "getFieldElementValue",
      "setFieldValue",
      "setFieldElementValue",
      "setAllFieldElementValues",
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
      "field_name",
      "field_value",
      "field_element_index",
      "long_array_parameter",
      "length_parameter",
      "count",
      "count_array",
      "direction",
      "direction_array"
    ],
    "fields":[
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
    ]
  }
}
```

Example:

```shell
getApi ["FieldTester"]
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["FieldTester"],
    "methods":[
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
    "fields":[
      "double",
      "bool",
      "long_array",
      "double_array",
      "bool_array",
      "string",
      "odd",
      "mode",
      "odd_array"
    ]
  }
}
```

###Python

Example Python session:

```python
from modular_device import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '5x3', 'name': 'field_tester', 'serial_number': 0}
dev.get_methods()
['get_count_array',
 'get_long_array_fixed',
 'set_all_field_element_values',
 'get_direction',
 'get_field_default_values',
 'get_count',
 'set_field_value',
 'get_string_some',
 'get_device_info',
 'get_memory_free',
 'set_field_element_value',
 'set_long_array_variable',
 'set_fields_to_defaults',
 'get_device_id',
 'increment_mode',
 'get_direction_array',
 'set_long_array_fixed',
 'set_long_array_parameter',
 'get_string_all',
 'get_long_array_variable',
 'check_mode',
 'get_field_value',
 'get_field_element_value',
 'get_api',
 'get_api_verbose',
 'get_doubled',
 'get_bool',
 'set_field_to_default',
 'get_field_values']
dev.set_fields_to_defaults()
dev.get_field_values()
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
dev.get_field_value('long_array')
[5, 4, 3, 2]
dev.get_field_element_value('double_array',1)
2.2
dev.set_field_value('bool_array',[False,False])
dev.set_field_value('string','asdfghjkl')
dev.get_field_element_value('string',3)
'f'
dev.set_field_element_value('string',3,'X')
dev.set_field_value('odd',2)
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in subset: [1,3,5,7,9], code: -32602
dev.set_field_value('odd',7)
dev.set_field_value('mode','test')
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in subset: [RISING,FALLING,CHANGE], code: -32602
dev.set_field_value('mode','CHANGE')
dev.set_field_element_value('mode',3,'t')
IOError: (from server) message: Invalid params, data: Cannot set element in string field with subset., code: -32602
dev.set_all_field_element_values('odd_array',2)
IOError: (from server) message: Invalid params, data: Array parameter element value not valid. Value not in subset: [1,3,5,7,9], code: -32602
dev.set_all_field_element_values('odd_array',9)
dev.get_field_values()
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
dev.get_api(["FieldTester"])
{'fields': ['double',
  'bool',
  'long_array',
  'double_array',
  'bool_array',
  'string',
  'odd',
  'mode',
  'odd_array'],
 'firmware': ['FieldTester'],
 'methods': ['getDoubled',
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
serial_port = 'COM4'             % example Windows serial port
dev = ModularClient(serial_port) % creates a device object
dev.open()                       % opens a serial connection to the device
dev.getDeviceId()
ans =
  name: 'field_tester'
  form_factor: '5x3'
  serial_number: 0
dev.getMethods()                 % get device methods
Modular Device Methods
---------------------
getDeviceId
getDeviceInfo
getApi
getFieldDefaultValues
setFieldsToDefaults
getFieldValues
getFieldValue
getFieldElementValue
setFieldValue
setFieldElementValue
setAllFieldElementValues
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
dev.getFieldValues()
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
dev.getFieldValue('long_array')
ans =
  5   4   3   2
dev.getFieldElementValue('double_array',1)
ans =
  2.2000
dev.setFieldValue('bool_array',[0,0]);
dev.setFieldValue('string','asdfghjkl');
dev.getFieldElementValue('string',3)
ans =
  f
dev.setFieldElementValue('string',3,'X');
dev.getFieldValues()
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
dev.setFieldsToDefaults();
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
