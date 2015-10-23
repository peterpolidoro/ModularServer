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

To get help information about the Arduino device, type a single
question mark ? into the input field and press the 'Send' button or
press the 'Enter' key.

```shell
?
```

Example Response:

```json
{
  "method":"?",
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
  ],
  "status":success
}
```

"methods" is an array of user methods. To execute a method, simply
type it into the input field and press the 'Send' button or press the
'Enter' key.

Example Method:

```shell
getMemoryFree
```

Example Response:

```json
{
  "method":"getMemoryFree",
  "memory_free":4514,
  "status":success
}
```

Example Method with Parameters:

```shell
repeat
```

Example Response:

```json
{
  "method":"repeat",
  "status":error,
  "error_message":"Incorrect number of parameters. 0 given. 2 needed."
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
  "method":"repeat",
  "parameters":[
    "string",
    "count"
  ],
  "status":success
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
  "method":"repeat",
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
  "status":success
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
  "method":"repeat",
  "parameter":{
    "name":"count",
    "type":"long",
    "min":1,
    "max":100
  },
  "status":success
}
```

Example Method:

```shell
repeat "I am a string to repeat." 4
```

Example Response:

```json
{
  "method":"repeat",
  "strings":[
    "I am a string to repeat.",
    "I am a string to repeat.",
    "I am a string to repeat.",
    "I am a string to repeat."
  ],
  "status":success
}
```

Example Method:

```shell
charsAt "I am an input string!" [0,6,8]
```

Example Response:

```json
{
  "method":"charsAt",
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
  ],
  "status":success
}
```

Example Method:

```shell
startingChars "Fantastic!"
```

Example Response:

```json
{
  "method":"startingChars",
  "starting_chars":  "Fa",
  "status":success
}
```

Example Method:

```shell
setStartingCharsCount 5
```

Example Response:

```json
{
  "method":"setStartingCharsCount",
  "status":success
}
```

Example Method:

```shell
startingChars "Fantastic!"
```

Example Response:

```json
{
  "method":"startingChars",
  "starting_chars":  "Fanta",
  "status":success
}
```

###Python

Example Python session:

```python
from modular_device import ModularDevice
dev = ModularDevice() # Automatically finds device if one available
dev.get_device_info()
{'firmware_version': {'major': 0, 'minor': 1, 'patch': 0},
 'model_number': 1002,
 'name': 'string_controller',
 'serial_number': 0}
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
dev.get_memory_free()
4514
dev.repeat()
IOError: (from device) Incorrect number of parameters. 0 given. 2 needed.
dev.repeat('?')
['string', 'count']
dev.repeat('??')
[{'name': 'string', 'type': 'string'},
 {'max': 100, 'min': 1, 'name': 'count', 'type': 'long'}]
dev.repeat('count','?')
{'max': 100, 'min': 1, 'name': 'count', 'type': 'long'}
dev.repeat('"I am a string to repeat."',-1)
IOError: (from device) Parameter value out of range: 1 <= count <= 100
dev.repeat('"I am a string to repeat."',4)
['I am a string to repeat.',
 'I am a string to repeat.',
 'I am a string to repeat.',
 'I am a string to repeat.']
dev.chars_at('"I am an input string!"',[0,6,8])
[{'char': 'I', 'index': 0},
 {'char': 'n', 'index': 6},
 {'char': 'i', 'index': 8}]
dev.get_starting_chars_count()
5
dev.starting_chars("Fantastic!")
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
device_info = dev.getDeviceInfo()
device_info =
  name: 'string_controller'
  model_number: 1002
  serial_number: 0
  firmware_version: [1x1 struct]
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
dev.getMemoryFree()
ans =
  4514
dev.repeat()
Error using ModularDevice/sendRequest (line 297)
device responded with error, Incorrect number of parameters. 0 given. 2 needed.
dev.repeat('?')
ans =
  'string'    'count'
dev.repeat('count','?')
ans =
  name: 'count'
  type: 'long'
  min: 1
  max: 100
dev.repeat('??')
ans =
  [1x1 struct] [1x1 struct]
dev.repeat('"I am a string to repeat."',-1)
Error using ModularDevice/sendRequest (line 297)
device responded with error, Parameter value out of range: 1 <= count <= 100
dev.repeat('"I am a string to repeat."',4)
ans =
I am a string to repeat.
I am a string to repeat.
I am a string to repeat.
I am a string to repeat.
dev.charsAt('"I am an input string!"','[0,6,8]')
ans =
1x3 struct array with fields:
    index
    char
dev.getStartingCharsCount()
ans =
     5
dev.startingChars('Fantastic!')
ans =
Fanta
dev.close()                      % close serial connection
delete(dev)                      % deletes the device
```

For more details on the Matlab interface:

<https://github.com/janelia-modular-devices/modular_device_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
