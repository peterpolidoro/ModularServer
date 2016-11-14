#StringController

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
      "name":"string_controller",
      "form_factor":"5x3",
      "serial_number":77
    },
    "API":{
      "firmware":["All"],
      "methods":[
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
        "echo",
        "length",
        "startsWith",
        "repeat",
        "charsAt",
        "startingChars",
        "setStoredString",
        "getStoredString"
      ],
      "parameters":[
        "firmware",
        "property_name",
        "property_value",
        "property_element_index",
        "string",
        "string2",
        "count",
        "index_array",
        "double_echo"
      ],
      "properties":[
        "serial_number",
        "starting_chars_count",
        "stored_string"
      ],
      "callbacks":[]
    }
  }
}
```

"methods" is an array of user methods. To execute a method, simply
type it into the input property and press the 'Send' button or press the
'Enter' key.

Example Method:

```shell
getStoredString
```

Example Response:

```json
{
  "id":"getStoredString",
  "result":"I am a stored string."
}
```

Example Method with Parameters:

```shell
repeat
```

Example Response:

```json
{
  "id":"repeat",
  "error":{
    "message":"Invalid params",
    "data":"Incorrect number of parameters. 0 given. 2 needed.",
    "code":-32602
  }
}
```

To get more information about a method, enter the method followed by
a question mark ?

Example Method Help:

```shell
repeat ?
```

Example Response:

```json
{
  "id":"repeat",
  "result":{
    "name":"repeat",
    "firmware":"StringController",
    "parameters":[
      "string",
      "count"
    ],
    "result_type":"array"
  }
}
```

The repeat method requires 2 parameters.

To get more information about all of the parameters a method takes,
enter the method followed by two questions marks ??:

Example Parameter Help:

```shell
repeat ??
```

Example Response:

```json
{
  "id":"repeat",
  "result":{
    "name":"repeat",
    "firmware":"StringController",
    "parameters":[
      {
        "name":"string",
        "firmware":"StringController",
        "type":"string"
      },
      {
        "name":"count",
        "firmware":"StringController",
        "type":"long",
        "min":1,
        "max":100
      }
    ],
    "result_type":"array"
  }
}
```

To get more information about just one of the parameters a method takes,
enter the method followed by the parameter followed by one question mark ?:

Example Parameter Help:

```shell
repeat count ?
```

Example Response:

```json
{
  "id":"repeat",
  "result":{
    "name":"count",
    "firmware":"StringController",
    "type":"long",
    "min":1,
    "max":100
  }
}
```

Example Method:

```shell
repeat "I am a string to repeat." 4
```

Example Response:

```json
{
  "id":"repeat",
  "result":[
    "I am a string to repeat.",
    "I am a string to repeat.",
    "I am a string to repeat.",
    "I am a string to repeat."
  ]
}
```

Example Method:

```shell
charsAt "I am an input string!" [0,6,8]
```

Example Response:

```json
{
  "id":"charsAt",
  "result":[
    {
      "index":0,
      "char":"I"
    },
    {
      "index":6,
      "char":"n"
    },
    {
      "index":8,
      "char":"i"
    }
  ]
}
```

Example Method:

```shell
startingChars "Fantastic!"
```

Example Response:

```json
{
  "id":"startingChars",
  "result":"Fa"
}
```

Example Method:

```shell
setPropertyValue starting_chars_count 5
```

Example Response:

```json
{
  "id":"setPropertyValue",
  "result":null
}
```

Example Method:

```shell
startingChars "Fantastic!"
```

Example Response:

```json
{
  "id":"startingChars",
  "result":"Fanta"
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
    "name":"string_controller",
    "form_factor":"5x3",
    "serial_number":77
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
        "version":"1.0.0"
      },
      {
        "name":"StringController",
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
      "echo",
      "length",
      "startsWith",
      "repeat",
      "charsAt",
      "startingChars",
      "setStoredString",
      "getStoredString"
    ],
    "parameters":[
      "firmware",
      "property_name",
      "property_value",
      "property_element_index",
      "string",
      "string2",
      "count",
      "index_array",
      "double_echo"
    ],
    "properties":[
      "serial_number",
      "starting_chars_count",
      "stored_string"
    ],
    "callbacks":[]
  }
}
```

Example:

```shell
getApi ["StringController"]
```

Example Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["StringController"],
    "methods":[
      "echo",
      "length",
      "startsWith",
      "repeat",
      "charsAt",
      "startingChars",
      "setStoredString",
      "getStoredString"
    ],
    "parameters":[
      "string",
      "string2",
      "count",
      "index_array",
      "double_echo"
    ],
    "properties":[
      "starting_chars_count",
      "stored_string"
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
{'form_factor': '5x3', 'name': 'string_controller', 'serial_number': 77}
dev.get_methods()
['echo',
 'get_property_default_values',
 'set_property_value',
 'get_device_info',
 'starts_with',
 'get_memory_free',
 'set_property_element_value',
 'set_properties_to_defaults',
 'get_device_id',
 'chars_at',
 'set_stored_string',
 'repeat',
 'get_stored_string',
 'set_all_property_element_values',
 'get_property_value',
 'starting_chars',
 'get_property_element_value',
 'get_api',
 'get_api_verbose',
 'length',
 'set_property_to_default',
 'get_property_values']
dev.repeat()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 2 needed., code: -32602
dev.repeat('?')
{'firmware': 'StringController',
 'name': 'repeat',
 'parameters': ['string', 'count'],
 'result_type': 'array'}
dev.repeat('??')
{'firmware': 'StringController',
 'name': 'repeat',
 'parameters': [{'firmware': 'StringController',
   'name': 'string',
   'type': 'string'},
  {'firmware': 'StringController',
   'max': 100,
   'min': 1,
   'name': 'count',
   'type': 'long'}],
 'result_type': 'array'}
dev.repeat('count','?')
{'firmware': 'StringController',
 'max': 100,
 'min': 1,
 'name': 'count',
 'type': 'long'}
dev.repeat('"I am a string to repeat."',-1)
IOError: (from server) message: Invalid params, data: Parameter value out of range: 1 <= count <= 100, code: -32602
dev.repeat('I am a string to repeat.',4)
['I am a string to repeat.',
 'I am a string to repeat.',
 'I am a string to repeat.',
 'I am a string to repeat.']
dev.chars_at('I am an input string!',[0,6,8])
[{'char': 'I', 'index': 0},
 {'char': 'n', 'index': 6},
 {'char': 'i', 'index': 8}]
dev.get_property_value('starting_chars_count')
5
dev.set_property_value('starting_chars_count',3)
dev.call_server_method('set_property_value','starting_chars_count',7)
dev.send_json_request('["set_property_value","starting_chars_count",5]')
dev.get_property_value('starting_chars_count')
3
dev.starting_chars('Fantastic!')
'Fan'
dev.call_server_method('starting_chars','Fantastic!')
'Fan'
dev.send_json_request('["starting_chars","Fantastic!"]')
'Fan'
dev.get_api(["StringController"])
{'properties': ['starting_chars_count', 'stored_string'],
 'firmware': ['StringController'],
 'callbacks': [],
 'methods': ['echo',
  'length',
  'startsWith',
  'repeat',
  'charsAt',
  'startingChars',
  'setStoredString',
  'getStoredString'],
 'parameters': ['string', 'string2', 'count', 'index_array', 'double_echo']}
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
  name: 'string_controller'
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
  echo
  length
  startsWith
  repeat
  charsAt
  startingChars
  setStartingCharsCount
  getStartingCharsCount
  setStoredString
  getStoredString
dev.repeat()
(from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 2 needed., code: -32602
dev.repeat('?')
ans =
  name: 'repeat'
  parameters: {'string'    'count'}
  result_type: 'array'
dev.repeat('count','?')
ans =
  name: 'count'
  type: 'long'
  min: 1
  max: 100
dev.repeat('I am a string to repeat.',-1)
device responded with error, Parameter value out of range: 1 <= count <= 100
dev.repeat('I am a string to repeat.',4)
ans =
  I am a string to repeat.
  I am a string to repeat.
  I am a string to repeat.
  I am a string to repeat.
chars_at = dev.charsAt('I am an input string!',[0,6,8]);
json = dev.convertToJson(chars_at)
json =
  [{"index":0,"char":"I"},{"index":6,"char":"n"},{"index":8,"char":"i"}]
dev.getPropertyValue('starting_chars_count')
ans =
     5
dev.startingChars('Fantastic!')
ans =
  Fanta
result = dev.callServerMethod('startingChars','Fantastic!')
result =
  Fanta
result = dev.sendJsonRequest('["startingChars","Fantastic!"]')
result =
  Fanta
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
