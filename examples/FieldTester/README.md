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
      "name":"string_controller",
      "model_number":1002,
      "serial_number":0,
      "firmware_version":{
        "major":0,
        "minor":1,
        "patch":0
      }
    },
    "methods":[
      "getMemoryFree",
      "resetDefaults",
      "setSerialNumber",
      "echo",
      "length",
      "startsWith",
      "repeat",
      "charsAt",
      "startingChars",
      "setStartingCharsCount",
      "getStartingCharsCount",
      "setStoredString",
      "getStoredString"
    ]
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
  "result":""
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
setStartingCharsCount 5
```

Example Response:

```json
{
  "id":"setStartingCharsCount",
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
from modular_device import ModularDevice
dev = ModularDevice() # Automatically finds device if one available
device_info = dev.get_device_info()
dev.convert_to_json(device_info)
'{"serial_number":0,"firmware_version":{"major":0,"minor":1,"patch":0},"name":"string_controller","model_number":1002}'
dev.get_methods()
['starts_with',
 'get_memory_free',
 'repeat',
 'reset_defaults',
 'starting_chars',
 'set_serial_number',
 'get_starting_chars_count',
 'set_starting_chars_count',
 'echo',
 'length',
 'chars_at']
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
dev.repeat('"I am a string to repeat."',4)
['I am a string to repeat.',
 'I am a string to repeat.',
 'I am a string to repeat.',
 'I am a string to repeat.']
dev.chars_at('"I am an input string!"',[0,6,8])
[{'char': 'I', 'index': 0},
 {'char': 'n', 'index': 6},
 {'char': 'i', 'index': 8}]
dev.get_starting_chars_count('?')
{'name': 'getStartingCharsCount', 'parameters': [], 'result_type': 'long'}
dev.set_starting_chars_count(3)
dev.call_server_method('set_starting_chars_count',7)
dev.send_json_request('["set_starting_chars_count",5]')
dev.get_starting_chars_count()
5
dev.starting_chars('Fantastic!')
'Fanta'
dev.call_server_method('starting_chars','Fantastic!')
'Fanta'
dev.send_json_request('["starting_chars","Fantastic!"]')
'Fanta'
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
device_info = dev.getDeviceInfo();
json = dev.convertToJson(device_info)
json =
  {"name":"string_controller","model_number":1002,"serial_number":0,"firmware_version":{"major":0,"minor":1,"patch":0}}
dev.getMethods()                 % get device methods
Modular Device Methods
---------------------
getMemoryFree
resetDefaults
setSerialNumber
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
dev.getStartingCharsCount()
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
dev.close()                      % close serial connection
delete(dev)                      % deletes the device
```

For more details on the Matlab interface:

<https://github.com/janelia-modular-devices/modular_device_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
