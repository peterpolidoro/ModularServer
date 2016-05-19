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
    "device_info":{
      "name":"field_tester",
      "model_number":1003,
      "board":"mega",
      "serial_number":0,
      "firmware_version":{
        "major":0,
        "minor":1,
        "patch":0
      }
    },
    "methods":[
      "getMemoryFree",
      "getFieldDefaultValues",
      "setFieldsToDefaults",
      "setFieldToDefault",
      "getFieldValues",
      "getFieldValue",
      "getFieldElementValue",
      "setFieldValue",
      "setFieldElementValue",
      "setAllFieldElementValues",
      "getDoubled",
      "getBool",
      "getLongArrayFixed",
      "getLongArrayVariable",
      "setLongArrayFixed",
      "setLongArrayVariable",
      "setLongArrayParameter",
      "getStringAll",
      "getStringSome"
    ],
    "parameters":[
      "field_name",
      "field_value",
      "field_element_index",
      "long_array_parameter",
      "length_parameter"
    ],
    "fields":[
      "serial_number",
      "double",
      "bool",
      "long_array",
      "double_array",
      "bool_array",
      "string"
    ]
  }
}
```

"methods" is an array of user methods. To execute a method, simply
type it into the input field and press the 'Send' button or press the
'Enter' key.

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
    "string":"abcdef"
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
    "string":"asdXghjkl"
  }
}
```

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

###Python

Example Python session:

```python
from modular_device import ModularDevice
dev = ModularDevice() # Automatically finds device if one available
dev.get_methods()
['get_memory_free',
 'get_bool',
 'get_field_value',
 'get_field_element_value',
 'set_field_element_value',
 'get_long_array_fixed',
 'set_long_array_variable',
 'get_doubled',
 'set_long_array_fixed',
 'set_fields_to_defaults',
 'get_field_default_values',
 'set_long_array_parameter',
 'get_string_all',
 'get_long_array_variable',
 'set_field_value',
 'set_all_field_element_values',
 'get_field_values',
 'get_string_some',
 'set_field_to_default']
dev.get_field_values()
{'bool': False,
 'bool_array': [False, True],
 'double': 3.14159,
 'double_array': [-1.1, 2.2, 3.3],
 'long_array': [5, 4, 3, 2],
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
dev.get_field_values()
{'bool': False,
 'bool_array': [False, False],
 'double': 3.14159,
 'double_array': [-1.1, 2.2, 3.3],
 'long_array': [5, 4, 3, 2],
 'serial_number': 0,
 'string': 'asdXghjkl'}
dev.set_fields_to_defaults()
```

For more details on the Python interface:

<https://github.com/janelia-modular-devices/modular_device_python>

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
dev = ModularDevice(serial_port) % creates a device object
dev.open()                       % opens a serial connection to the device
dev.getMethods()                 % get device methods
Modular Device Methods
---------------------
getMemoryFree
getFieldDefaultValues
setFieldsToDefaults
getFieldValues
getFieldValue
getFieldElementValue
setFieldValue
setFieldElementValue
setAllFieldElementValues
getDoubled
getBool
getLongArrayFixed
getLongArrayVariable
setLongArrayFixed
setLongArrayVariable
setLongArrayParameter
getStringAll
getStringSome
dev.getFieldValues()
ans =
  serial_number: 0
  double: 3.1416
  bool: 0
  long_array: [5 4 3 2]
  double_array: [-1.1000 2.2000 3.3000]
  bool_array: [0 1]
  string: 'abcdef'
dev.getFieldValue('long_array')
ans =
  5   4   3   2
dev.getFieldElementValue('double_array',1)
ans =
  2.2000
dev.setFieldValue('bool_array',[false,false]);
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
dev.setFieldsToDefaults();
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-modular-devices/modular_device_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
