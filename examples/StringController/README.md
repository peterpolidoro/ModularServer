#StringController

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
      "name":"string_controller",
      "form_factor":"mega",
      "serial_number":0
    },
    "API":{
      "methods":[
        "getDeviceId",
        "getDeviceInfo",
        "getApi",
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
        "field_name",
        "field_value",
        "field_element_index",
        "string",
        "string2",
        "count",
        "index_array",
        "double_echo"
      ],
      "fields":[
        "serial_number",
        "starting_chars_count",
        "stored_string"
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
    "parameters":[
      {
        "name":"string",
        "type":"string"
      },
      {
        "name":"count",
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
setFieldValue starting_chars_count 5
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
startingChars "Fantastic!"
```

Example Response:

```json
{
  "id":"startingChars",
  "result":"Fanta"
}
```

###Python

Example Python session:

```python
from modular_device import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': 'mega', 'name': 'string_controller', 'serial_number': 0}
dev.get_methods()
['starts_with',
 'get_memory_free',
 'repeat',
 'set_all_field_element_values',
 'starting_chars',
 'get_field_value',
 'get_field_element_value',
 'get_api',
 'set_field_element_value',
 'length',
 'echo',
 'set_fields_to_defaults',
 'get_device_id',
 'get_field_default_values',
 'get_stored_string',
 'chars_at',
 'set_stored_string',
 'set_field_value',
 'set_field_to_default',
 'get_field_values',
 'get_device_info']
dev.repeat()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 2 needed., code: -32602
dev.repeat('?')
{'name': 'repeat', 'parameters': ['string', 'count'], 'result_type': 'array'}
dev.repeat('??')
{'name': 'repeat',
 'parameters': [{'name': 'string', 'type': 'string'},
  {'max': 100, 'min': 1, 'name': 'count', 'type': 'long'}],
 'result_type': 'array'}
dev.repeat('count','?')
{'max': 100, 'min': 1, 'name': 'count', 'type': 'long'}
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
dev.get_field_value('starting_chars_count')
2
dev.set_field_value('starting_chars_count',3)
dev.call_server_method('set_field_value','starting_chars_count',7)
dev.send_json_request('["set_field_value","starting_chars_count",5]')
dev.get_field_value('starting_chars_count')
5
dev.starting_chars('Fantastic!')
'Fanta'
dev.call_server_method('starting_chars','Fantastic!')
'Fanta'
dev.send_json_request('["starting_chars","Fantastic!"]')
'Fanta'
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
  name: 'string_controller'
  form_factor: 'mega'
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
dev.getFieldValue('starting_chars_count')
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
