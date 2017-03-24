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
      "name":"property_tester",
      "form_factor":"3x2",
      "serial_number":0
    },
    "API":{
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
        "incrementMode",
        "setNewDoubleRange",
        "setNewOddSubset",
        "setNewOddDefault"
      ],
      "parameters":[
        "long_array_parameter",
        "length_parameter",
        "count",
        "count_array",
        "direction",
        "direction_array",
        "subset_index"
      ],
      "properties":[
        "double",
        "bool",
        "longArray",
        "doubleArray",
        "boolArray",
        "string",
        "odd",
        "mode",
        "modeArray",
        "oddArray"
      ],
      "callbacks":[]
    }
  }
}
```

"functions" is an array of user functions. To execute a function, simply
type it into the input property and press the 'Send' button or press the
'Enter' key.

Request:

```shell
setPropertiesToDefaults
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
getPropertyValues
```

Response:

```json
{
  "id":"getPropertyValues",
  "result":{
    "serialNumber":0,
    "double":3.141590,
    "bool":false,
    "longArray":[
      5,
      4,
      3,
      2
    ],
    "doubleArray":[
      -1.100000,
      2.200000,
      3.300000
    ],
    "boolArray":[
      false,
      true
    ],
    "string":"abcdef",
    "odd":5,
    "mode":"RISING",
    "modeArray":[
      "RISING",
      "FALLING"
    ],
    "oddArray":[
      1,
      5
    ]
  }
}
```

Request:

```shell
double ?
```

Response:

```json
{
  "id":"double",
  "result":{
    "name":"double",
    "firmware":"PropertyTester",
    "type":"double",
    "min":1.000000,
    "max":5.000000,
    "value":3.141590,
    "default_value":3.141590,
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
longArray ?
```

Response:

```json

{
  "id":"longArray",
  "result":{
    "name":"longArray",
    "firmware":"PropertyTester",
    "type":"array",
    "array_element_type":"long",
    "array_element_min":-3,
    "array_element_max":10,
    "array_length_min":1,
    "array_length_max":4,
    "value":[
      5,
      4,
      3,
      2
    ],
    "default_value":[
      5,
      4,
      3,
      2
    ],
    "functions":[
      "getValue",
      "setValue",
      "getDefaultValue",
      "setValueToDefault",
      "getElementValue",
      "setElementValue",
      "getDefaultElementValue",
      "setElementValueToDefault",
      "setAllElementValues"
    ],
    "parameters":[
      "value",
      "element_index",
      "element_value"
    ]
  }
}
```

Request:

```shell
longArray getValue
```

Response:

```json
{
  "id":"longArray",
  "result":[
    5,
    4,
    3,
    2
  ]
}
```

Request:

```shell
longArray
```

Response:

```json
{
  "id":"longArray",
  "result":[
    5,
    4,
    3,
    2
  ]
}
```

Request:

```shell
doubleArray ?
```

Response:

```json
{
  "id":"doubleArray",
  "result":{
    "name":"doubleArray",
    "firmware":"PropertyTester",
    "type":"array",
    "array_element_type":"double",
    "array_element_min":-33.333000,
    "array_element_max":100.000000,
    "array_length_min":1,
    "array_length_max":3,
    "value":[
      -1.100000,
      2.200000,
      3.300000
    ],
    "default_value":[
      -1.100000,
      2.200000,
      3.300000
    ],
    "functions":[
      "getValue",
      "setValue",
      "getDefaultValue",
      "setValueToDefault",
      "getElementValue",
      "setElementValue",
      "getDefaultElementValue",
      "setElementValueToDefault",
      "setAllElementValues"
    ],
    "parameters":[
      "value",
      "element_index",
      "element_value"
    ]
  }
}
```

Request:

```shell
doubleArray getElementValue 1
```

Response:

```json
{
  "id":"doubleArray",
  "result":2.200000
}
```

Request:

```shell
doubleArray getElementValue 10
```

Response:

```json
{
  "id":"doubleArray",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in range: 0 <= element_index <= 2",
    "code":-32602
  }
}
```

Request:

```shell
doubleArray setAllElementValues 8.7654
```

Response:

```json
{
  "id":"doubleArray",
  "result":[
    8.765400,
    8.765400,
    8.765400
  ]
}
```

Request:

```shell
doubleArray setElementValue 1 5.678
```

Response:

```json
{
  "id":"doubleArray",
  "result":[
    8.765400,
    5.678000,
    8.765400
  ]
}
```

Request:

```shell
doubleArray getDefaultElementValue 0
```

Response:

```json
{
  "id":"doubleArray",
  "result":-1.100000
}
```

Request:

```shell
boolArray ?
```

Response:

```json
{
  "id":"boolArray",
  "result":{
    "name":"boolArray",
    "firmware":"PropertyTester",
    "type":"array",
    "array_element_type":"bool",
    "array_length_min":1,
    "array_length_max":2,
    "value":[
      false,
      true
    ],
    "default_value":[
      false,
      true
    ],
    "functions":[
      "getValue",
      "setValue",
      "getDefaultValue",
      "setValueToDefault",
      "getElementValue",
      "setElementValue",
      "getDefaultElementValue",
      "setElementValueToDefault",
      "setAllElementValues"
    ],
    "parameters":[
      "value",
      "element_index",
      "element_value"
    ]
  }
}
```

Request:

```shell
boolArray setValue [false,false]
```

Response:

```json
{
  "id":"boolArray",
  "result":[
    false,
    false
  ]
}
```

Request:

```shell
string setValue asdfghjkl
```

Response:

```json
{
  "id":"string",
  "result":"asdfghjkl"
}
```

Request:

```shell
string getElementValue 3
```

Response:

```json
{
  "id":"string",
  "result":"f"
}
```

Request:

```shell
string setElementValue 3 X
```

Response:

```json
{
  "id":"string",
  "result":"asdXghjkl"
}
```

Request:

```shell
odd setValue 2
```

Response:

```json
{
  "id":"odd",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in subset: [1,3,5,7,9]",
    "code":-32602
  }
}
```

Request:

```shell
odd setValue 7
```

Response:

```json
{
  "id":"odd",
  "result":7
}
```

Request:

```shell
mode setValue test
```

Response:

```json
{
  "id":"mode",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in subset: [RISING,FALLING,CHANGE]",
    "code":-32602
  }
}
```

Request:

```shell
mode setValue CHANGE
```

Response:

```json
{
  "id":"mode",
  "result":"CHANGE"
}
```

Request:

```shell
oddArray setAllElementValues 2
```

Response:

```json
{
  "id":"oddArray",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value not valid. Value not in subset: [1,3,5,7,9]",
    "code":-32602
  }
}
```

Request:

```shell
oddArray setAllElementValues 9
```

Response:

```json
{
  "id":"oddArray",
  "result":[
    9,
    9
  ]
}
```

Request:

```shell
getPropertyValues
```

Response:

```json
{
  "id":"getPropertyValues",
  "result":{
    "serialNumber":0,
    "double":3.141590,
    "bool":false,
    "longArray":[
      5,
      4,
      3,
      2
    ],
    "doubleArray":[
      8.765400,
      5.678000,
      8.765400
    ],
    "boolArray":[
      false,
      false
    ],
    "string":"asdXghjkl",
    "odd":7,
    "mode":"CHANGE",
    "modeArray":[
      "RISING",
      "FALLING"
    ],
    "oddArray":[
      9,
      9
    ]
  }
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

Request:

```shell
getDeviceInfo
```

Response:

```json
{
  "id":"getDeviceInfo",
  "result":{
    "processor":"ATmega2560",
    "hardware":[
      {
        "name":"Mega2560",
        "interrupts":[]
      }
    ],
    "firmware":[
      {
        "name":"ModularServer",
        "version":"2.0.0"
      },
      {
        "name":"PropertyTester",
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
getApi ["PropertyTester"]
```

Response:

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
      "setNewDoubleRange",
      "setNewOddSubset",
      "setNewOddDefault",
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
      "longArray",
      "doubleArray",
      "boolArray",
      "string",
      "odd",
      "mode",
      "modeArray",
      "oddArray"
    ],
    "callbacks":[]
  }
}
```

###Python

Example Python session:

```python
from modular_client import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '5x3', 'name': 'property_tester', 'serial_number': 0}
dev.get_methods()
['serial_number',
 'long_array',
 'get_count_array',
 'get_long_array_fixed',
 'get_property_values',
 'get_direction',
 'get_count',
 'odd',
 'get_string_some',
 'get_device_info',
 'get_memory_free',
 'bool_array',
 'set_long_array_variable',
 'get_device_id',
 'bool',
 'increment_mode',
 'string',
 'mode_array',
 'get_direction_array',
 'set_long_array_fixed',
 'odd_array',
 'detach_all_interrupts',
 'set_long_array_parameter',
 'get_string_all',
 'get_long_array_variable',
 'check_mode',
 'double_array',
 'get_interrupt_info',
 'get_api',
 'get_api_verbose',
 'get_doubled',
 'set_new_double_range',
 'set_new_odd_subset,
 'mode',
 'double',
 'get_bool',
 'get_property_default_values',
 'set_properties_to_defaults']
dev.set_properties_to_defaults()
dev.get_property_values()
{'bool': False,
 'boolArray': [False, True],
 'double': 3.14159,
 'doubleArray': [-1.1, 2.2, 3.3],
 'longArray': [5, 4, 3, 2],
 'mode': 'RISING',
 'modeArray': ['RISING', 'FALLING'],
 'odd': 5,
 'oddArray': [1, 5],
 'serialNumber': 0,
 'string': 'abcdef'}
dev.long_array('getValue')
[5, 4, 3, 2]
dev.long_array()
[5, 4, 3, 2]
dev.double_array('getElementValue',1)
2.2
dev.double_array('getElementValue',10)
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in range: 0 <= element_index <= 2, code: -32602
dev.double_array('setAllElementValues',8.7654)
[8.7654, 8.7654, 8.7654]
dev.double_array('setElementValue',1,5.678)
[8.7654, 5.678, 8.7654]
dev.double_array('getDefaultElementValue',0)
-1.1
dev.bool_array('setValue',[False,False])
[False, False]
dev.string('setValue','asdfghjkl')
'asdfghjkl'
dev.string('getElementValue',3)
'f'
dev.string('setElementValue',3,'X')
'asdXghjkl'
dev.odd('setValue',2)
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in subset: [1,3,5,7,9], code: -32602
dev.odd('setValue',7)
7
dev.mode('setValue','test')
IOError: (from server) message: Invalid params, data: Parameter value not valid. Value not in subset: [RISING,FALLING,CHANGE], code: -32602
dev.mode('setValue','CHANGE')
'CHANGE'
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
dev.getMethods()                 % get device functions
  Modular Device Methods
  ---------------------
  getDeviceId
  getDeviceInfo
  getInterruptInfo
  detachAllInterrupts
  getApi
  getApiVerbose
  getPropertyDefaultValues
  setPropertiesToDefaults
  getPropertyValues
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
  setNewDoubleRange
  setNewOddSubset
  setNewOddDefault
  serialNumber
  double
  bool
  longArray
  doubleArray
  boolArray
  string
  odd
  mode
  modeArray
  oddArray
dev.setPropertiesToDefaults()
dev.getPropertyValues()
ans =
  serialNumber: 0
  double: 3.1416
  bool: 0
  longArray: [5 4 3 2]
  doubleArray: [-1.1000 2.2000 3.3000]
  boolArray: {[0]  [1]}
  string: 'abcdef'
  odd: 5
  mode: 'RISING'
  modeArray: ['RISING' 'FALLING']
  oddArray: [1 5]
dev.longArray('getValue')
ans =
  5     4     3     2
dev.longArray()
ans =
  5     4     3     2
dev.doubleArray('getElementValue',1)
ans =
  2.2000
dev.doubleArray('getElementValue',10)
(from server) message: Invalid params, data: Parameter value not valid. Value not in range: 0 <=
element_index <= 2, code: -32602
dev.doubleArray('setAllElementValues',8.7654)
ans =
  8.7654    8.7654    8.7654
dev.doubleArray('setElementValue',1,5.678)
ans =
  8.7654    5.6780    8.7654
dev.doubleArray('getDefaultElementValue',0)
ans =
  -1.1000
dev.boolArray('setValue',[false,false])
ans =
  [0]    [0]
dev.string('setValue','asdfghjkl')
ans =
  asdfghjkl
dev.string('getElementValue',3)
ans =
  f
dev.string('setElementValue',3,'X')
ans =
  asdXghjkl
dev.odd('setValue',2)
(from server) message: Invalid params, data: Parameter value not valid. Value not in subset:
[1,3,5,7,9], code: -32602
dev.odd('setValue',7)
ans =
  7
dev.mode('setValue','test')
(from server) message: Invalid params, data: Parameter value not valid. Value not in subset:
[RISING,FALLING,CHANGE], code: -32602
dev.mode('setValue','CHANGE')
ans =
  CHANGE
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
