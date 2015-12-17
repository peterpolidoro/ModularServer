#MinimalDevice

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
      "name":"minimal_device",
      "model_number":1000,
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
      "setSerialNumber"
    ]
  }
}
```

"methods" is an array of user methods. To execute a method, simply
type it into the input field and press the 'Send' button or press the
'Enter' key.

Example Method (This method only exists on AVR processors):

```shell
getMemoryFree
```

Example Response:

```json
{
  "id":"getMemoryFree",
  "result":5154
}
```

To get more verbose help about the modular device, including
information about the parameters each method takes, type two question
marks ?? into the input field and press the 'Send' button or press the
'Enter' key.

```shell
??
```

Example Response:

```json
{
  "id":"??",
  "result":{
    "device_info":{
      "name":"minimal_device",
      "model_number":1000,
      "serial_number":0,
      "firmware_version":{
        "major":0,
        "minor":1,
        "patch":0
      }
    },
    "methods":[
      {
        "name":"getMemoryFree",
        "parameters":[],
        "result_type":"long"
      },
      {
        "name":"resetDefaults",
        "parameters":[],
        "result_type":null
      },
      {
        "name":"setSerialNumber",
        "parameters":[
          "serial_number"
        ],
        "result_type":null
      }
    ],
    "parameters":[
      {
        "name":"serial_number",
        "type":"long",
        "min":0,
        "max":65535
      }
    ]
  }
}
```

Example Method with Parameters:

```shell
setSerialNumber
```

Example Response:

```json
{
  "id":"setSerialNumber",
  "error":{
    "message":"Invalid params",
    "data":"Incorrect number of parameters. 0 given. 1 needed.",
    "code":-32602
  }
}
```

When a method executes successfully, the response will contain a
"result" field. In some cases the result may be null, but the method
execution was still successful. When there is an error, there will not
exist a "result" field, but there will exist an "error" field.

To get more information about a method, enter the method followed by
a question mark.

Example Method Help:

```shell
setSerialNumber ?
```

Example Response:

```json
{
  "id":"setSerialNumber",
  "result":{
    "name":"setSerialNumber",
    "parameters":[
      "serial_number"
    ],
    "result_type":null
  }
}
```

Or you can enter a question mark followed by the method.

Example Method Help:

```shell
? setSerialNumber
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "name":"setSerialNumber",
    "parameters":[
      "serial_number"
    ],
    "result_type":null
  }
}
```

To get more verbose information about all of the parameters a method
takes, enter the method followed by two questions marks.

```shell
setSerialNumber ??
```

Example Response:

```json
{
  "id":"setSerialNumber",
  "result":{
    "name":"setSerialNumber",
    "parameters":[
      {
        "name":"serial_number",
        "type":"long",
        "min":0,
        "max":65535
      }
    ],
    "result_type":null
  }
}
```

Example Method:

```shell
setSerialNumber 32
```

Example Response:

```json
{
  "id":"setSerialNumber",
  "result":null
}
```

The serial number setting persists even after the device is powered
off. The serial number is used to differentiate several identical
devices connected to a single host machine at one time.

```shell
?
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "device_info":{
      "name":"minimal_device",
      "model_number":1000,
      "serial_number":32,
      "firmware_version":{
        "major":0,
        "minor":1,
        "patch":0
      }
    },
    "methods":[
      "getMemoryFree",
      "resetDefaults",
      "setSerialNumber"
    ]
  }
}
```

To reset the serial number to the default value, use the resetDefaults
method.

Example Method:

```shell
resetDefaults
```

Example Response:

```json
{
  "id":"resetDefaults",
  "result":null
}
```

```shell
?
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "device_info":{
      "name":"minimal_device",
      "model_number":1000,
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
      "setSerialNumber"
    ]
  }
}
```

###Python

Example Python session:

```python
from modular_device import ModularDevice
dev = ModularDevice() # Automatically finds device if one available
dev.get_device_info()
{'firmware_version': {'major': 0, 'minor': 1, 'patch': 0},
 'model_number': 1000,
 'name': 'minimal_device',
 'serial_number': 0}
dev.get_methods()
['set_serial_number', 'get_memory_free', 'reset_defaults']
dev.get_memory_free()
5154
dev.set_serial_number()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 1 needed., code: -32602
dev.set_serial_number('?')
{'name': 'setSerialNumber',
 'parameters': ['serial_number'],
 'result_type': None}
dev.set_serial_number('serial_number','?')
{'max': 65535, 'min': 0, 'name': 'serial_number', 'type': 'long'}
dev.set_serial_number('??')
{'name': 'setSerialNumber',
 'parameters': [{'max': 65535,
   'min': 0,
   'name': 'serial_number',
   'type': 'long'}],
 'result_type': None}
dev.set_serial_number(-1)
IOError: (from server) message: Invalid params, data: Parameter value out of range: 0 <= serial_number <= 65535, code: -32602
dev.set_serial_number(12)
result = dev.call_server_method('?')
dev.convert_to_json(result)
'{"device_info":{"serial_number":12,"firmware_version":{"major":0,"minor":1,"patch":0},"name":"minimal_device","model_number":1000},"methods":["getMemoryFree","resetDefaults","setSerialNumber"]}'
dev.send_json_request('["reset_defaults"]')
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
  name: 'minimal_device'
  model_number: 1000
  serial_number: 0
  firmware_version: [1x1 struct]
device_info.firmware_version
ans =
  major: 0
  minor: 1
  patch: 0
dev.getMethods()                 % get device methods
Modular Device Methods
---------------------
getMemoryFree
resetDefaults
setSerialNumber
dev.getMemoryFree()
ans =
  5154
dev.setSerialNumber()
Error using ModularDevice/sendRequest (line 301)
(from server) message: Invalid params, Incorrect number of parameters. 0 given. 1 needed., code: -32602
method_info = dev.setSerialNumber('?')
method_info
ans =
  name: 'setSerialNumber'
  parameters: 'serial_number'
  result_type: []
method_info = dev.setSerialNumber('??')
method_info =
  name: 'setSerialNumber'
  parameters: [1x1 struct]
  result_type: []
method_info.parameters
  name: 'serial_number'
  type: 'long'
  min: 0
  max: 65535
parameter_info = dev.setSerialNumber('serial_number','?')
parameter_info =
  name: 'serial_number'
  type: 'long'
  min: 0
  max: 65535
dev.setSerialNumber(-1)
Error using ModularDevice/sendRequest (line 301)
(from server) message: Invalid params, Parameter value out of range: 0 <= serial_number <= 65535, code: -32602
dev.setSerialNumber(13)
ans =
  []
dev.close()                      % close serial connection
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-modular-devices/modular_device_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
