# StringController

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Host Computer Interface

### Arduino Serial Monitor

Open the Serial Monitor in the Arduino IDE.

Set the baud to match the value in the Arduino sketch (115200).

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
  "id": "?",
  "result": {
    "device_id": {
      "name": "string_controller",
      "form_factor": "3x2",
      "serial_number": 0
    },
    "api": {
      "firmware": [
        "StringController"
      ],
      "verbosity": "NAMES",
      "functions": [
        "echo",
        "length",
        "startsWith",
        "repeat",
        "charsAt",
        "startingChars",
        "setStoredString",
        "getStoredString"
      ],
      "parameters": [
        "string",
        "string2",
        "count",
        "index_array",
        "double_echo"
      ],
      "properties": [
        "startingCharsCount",
        "storedString"
      ]
    }
  }
}
```

The form\_factor and serial\_number may be different on your board than the ones
shown above.

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
  "id": "??",
  "result": {
    "device_id": {
      "name": "string_controller",
      "form_factor": "3x2",
      "serial_number": 0
    },
    "api": {
      "firmware": [
        "ALL"
      ],
      "verbosity": "NAMES",
      "functions": [
        "getDeviceId",
        "getDeviceInfo",
        "getApi",
        "getPropertyDefaultValues",
        "setPropertiesToDefaults",
        "getPropertyValues",
        "getInterruptInfo",
        "detachAllInterrupts",
        "echo",
        "length",
        "startsWith",
        "repeat",
        "charsAt",
        "startingChars",
        "setStoredString",
        "getStoredString"
      ],
      "parameters": [
        "firmware",
        "verbosity",
        "string",
        "string2",
        "count",
        "index_array",
        "double_echo"
      ],
      "properties": [
        "serialNumber",
        "startingCharsCount",
        "storedString"
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
  "id": "setPropertiesToDefaults",
  "result": null
}
```

Request:

```shell
getStoredString
```

Response:

```json
{
  "id": "getStoredString",
  "result": "I am a stored string."
}
```

Request:

```shell
repeat
```

Response:

```json
{
  "id": "repeat",
  "error": {
    "message": "Invalid params",
    "data": "Incorrect number of parameters. 0 given. 2 needed.",
    "code": -32602
  }
}
```

To get more information about a function, enter the function followed by
a question mark ?

Request:

```shell
repeat ?
```

Response:

```json
{
  "id": "repeat",
  "result": {
    "name": "repeat",
    "firmware": "StringController",
    "parameters": [
      "string",
      "count"
    ],
    "result_info": {
      "type": "array",
      "array_element_type": "string"
    }
  }
}
```

The repeat function requires 2 parameters.

To get more information about all of the parameters a function takes,
enter the function followed by two questions marks ??:

Request:

```shell
repeat ??
```

Response:

```json
{
  "id": "repeat",
  "result": {
    "name": "repeat",
    "firmware": "StringController",
    "parameters": [
      {
        "name": "string",
        "type": "string"
      },
      {
        "name": "count",
        "type": "long",
        "min": 1,
        "max": 100
      }
    ],
    "result_info": {
      "type": "array",
      "array_element_type": "string"
    }
  }
}
```

To get more information about just one of the parameters a function takes,
enter the function followed by the parameter followed by one question mark ?:

Request:

```shell
repeat count ?
```

Response:

```json
{
  "id": "repeat",
  "result": {
    "name": "count",
    "firmware": "StringController",
    "type": "long",
    "min": 1,
    "max": 100
  }
}
```

Request:

```shell
repeat "I am a string to repeat." 4
```

Response:

```json
{
  "id": "repeat",
  "result": [
    "I am a string to repeat.",
    "I am a string to repeat.",
    "I am a string to repeat.",
    "I am a string to repeat."
  ]
}
```

Request:

```shell
charsAt "I am an input string!" [0,6,8]
```

Response:

```json
{
  "id": "charsAt",
  "result": [
    {
      "index": 0,
      "char": "I"
    },
    {
      "index": 6,
      "char": "n"
    },
    {
      "index": 8,
      "char": "i"
    }
  ]
}
```

Request:

```shell
startingChars "Fantastic!"
```

Response:

```json
{
  "id": "startingChars",
  "result": "Fa"
}
```

Request:

```shell
startingCharsCount setValue 5
```

Response:

```json
{
  "id": "startingCharsCount",
  "result": 5
}
```

Request:

```shell
startingChars "Fantastic!"
```

Response:

```json
{
  "id": "startingChars",
  "result": "Fanta"
}
```

Request:

```shell
storedString
```

Response:

```json
{
  "id": "storedString",
  "result": "I am a stored string."
}
```

Request:

```shell
storedString setAllElementValues x
```

Response:

```json
{
  "id": "storedString",
  "result": "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
}
```

Request:

```shell
storedString setValueToDefault
```

Response:

```json
{
  "id": "storedString",
  "result": "I am a stored string."
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
  "id": "getDeviceId",
  "result": {
    "name": "string_controller",
    "form_factor": "3x2",
    "serial_number": 77
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
  "id": "getDeviceInfo",
  "result": {
    "processor": "MK20DX256",
    "hardware": [
      {
        "name": "Teensy",
        "version": "3.2"
      }
    ],
    "firmware": [
      {
        "name": "ModularServer",
        "version": "3.0.0"
      },
      {
        "name": "StringController",
        "version": "2.0.0"
      }
    ]
  }
}
```

Every function, parameter, property, and callback belongs to one
firmware set.

To get the API limited to one or more firmware sets, use the getApi
function.

Request:

```shell
getApi NAMES ["StringController"]
```

Response:

```json
{
  "id": "getApi",
  "result": {
    "firmware": [
      "StringController"
    ],
    "verbosity": "NAMES",
    "functions": [
      "echo",
      "length",
      "startsWith",
      "repeat",
      "charsAt",
      "startingChars",
      "setStoredString",
      "getStoredString"
    ],
    "parameters": [
      "string",
      "string2",
      "count",
      "index_array",
      "double_echo"
    ],
    "properties": [
      "startingCharsCount",
      "storedString"
    ]
  }
}
```

### Python

Example Python session:

```python
from modular_client import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '3x2', 'name': 'string_controller', 'serial_number': 77}
dev.get_methods()
['starts_with',
 'stored_string',
 'repeat',
 'get_stored_string',
 'starting_chars',
 'set_stored_string',
 'get_interrupt_info',
 'get_api',
 'serial_number',
 'length',
 'echo',
 'get_property_values',
 'get_device_id',
 'detach_all_interrupts',
 'chars_at',
 'starting_chars_count',
 'get_property_default_values',
 'set_properties_to_defaults',
 'get_device_info']
dev.set_properties_to_defaults(['ALL'])
dev.repeat()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 2 needed., code: -32602
dev.repeat('?')
{'firmware': 'StringController',
 'name': 'repeat',
 'parameters': ['string', 'count'],
 'result_info': {'array_element_type': 'string', 'type': 'array'}}
dev.repeat('??')
{'firmware': 'StringController',
 'name': 'repeat',
 'parameters': [{'name': 'string', 'type': 'string'},
  {'max': 100, 'min': 1, 'name': 'count', 'type': 'long'}],
 'result_info': {'array_element_type': 'string', 'type': 'array'}}
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
dev.starting_chars_count('getValue')
2
dev.starting_chars_count('setValue',3)
3
dev.call_get_result('starting_chars_count','setValue',7)
7
dev.send_json_request('["startingCharsCount","setValue",3]')
3
dev.starting_chars('Fantastic!')
'Fan'
dev.call_get_result('starting_chars','Fantastic!')
'Fan'
dev.send_json_request('["startingChars","Fantastic!"]')
'Fan'
dev.get_api('NAMES',["StringController"])
{'firmware': ['StringController'],
 'functions': ['echo',
  'length',
  'startsWith',
  'repeat',
  'charsAt',
  'startingChars',
  'setStoredString',
  'getStoredString'],
 'parameters': ['string', 'string2', 'count', 'index_array', 'double_echo'],
 'properties': ['startingCharsCount', 'storedString'],
 'verbosity': 'NAMES'}
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
  name: 'string_controller'
  form_factor: '3x2'
  serial_number: 77
dev.getMethods()
  Modular Device Methods
  ---------------------
  getDeviceId
  getDeviceInfo
  getApi
  getPropertyDefaultValues
  setPropertiesToDefaults
  getPropertyValues
  getInterruptInfo
  detachAllInterrupts
  echo
  length
  startsWith
  repeat
  charsAt
  startingChars
  setStoredString
  getStoredString
  serialNumber
  startingCharsCount
  storedString
dev.setPropertiesToDefaults({'ALL'});
dev.repeat()
(from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 2 needed., code: -32602
dev.repeat('?')
ans =
  name: 'repeat'
  firmware: 'StringController'
  parameters: {'string'  'count'}
  result_info: [1x1 struct]
dev.repeat('count','?')
ans =
  name: 'count'
  firmware: 'StringController'
  type: 'long'
  min: 1
  max: 100
dev.repeat('I am a string to repeat.',-1)
device responded with error, Parameter value out of range: 1 <= count <= 100
repeated = dev.repeat('I am a string to repeat.',4);
json = dev.convertToJson(repeated)
  ["I am a string to repeat.","I am a string to repeat.","I am a string to repeat.","I am a string to repeat."]
chars_at = dev.charsAt('I am an input string!',[0,6,8]);
json = dev.convertToJson(chars_at)
json =
  [{"index": 0,"char": "I"},{"index": 6,"char": "n"},{"index": 8,"char": "i"}]
dev.startingCharsCount('getValue')
ans =
  2
dev.startingCharsCount('setValue',5)
ans =
  5
dev.startingChars('Fantastic!')
ans =
  Fanta
result = dev.callGetResult('startingChars','Fantastic!')
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

## Installation

<https://github.com/janelia-arduino/arduino-libraries>
