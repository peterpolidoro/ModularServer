# BoardLedController

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Host Computer Interface

### Arduino Serial Monitor

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
      "name":"board_led_controller",
      "form_factor":"3x2",
      "serial_number":0
    },
    "api":{
      "firmware":["BoardLedController"],
      "verbosity":"NAMES",
      "functions":[
        "setLedOn",
        "setLedOff",
        "getLedPin",
        "blinkLed"
      ],
      "parameters":[
        "duration_on",
        "duration_off",
        "count"
      ]
    }
  }
}
```

The form\_factor and serial\_number may be different on your board than the ones
shown above.

"functions" is an array of user functions. To execute a function, simply
type it into the input property and press the 'Send' button or press the
'Enter' key.

Request:

```shell
setLedOn
```

Response:

```json
{
  "id":"setLedOn",
  "result":null
}
```

Notice that the LED on the Arduino board has turned ON.

Request:

```shell
blinkLed
```

Response:

```json
{
  "id":"blinkLed",
  "error":{
    "message":"Invalid params",
    "data":"Incorrect number of parameters. 0 given. 3 needed.",
    "code":-32602
  }
}
```

When a function executes successfully, the response will contain a
"result" property. In some cases the result may be null, but the function
execution was still successful. When there is an error, there will not
exist a "result" property, but there will exist an "error" property.

To get more information about a function, enter the function followed by
a question mark ?

Request:

```shell
blinkLed ?
```

Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"blinkLed",
    "firmware":"BoardLedController",
    "parameters":[
      "duration_on",
      "duration_off",
      "count"
    ]
  }
}
```

The blinkLed function requires 3 parameters.

To get more information about a single function parameter, enter the
function followed by the parameter followed by a question mark ?

Request:

```shell
blinkLed duration_on ?
```

Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"duration_on",
    "firmware":"BoardLedController",
    "type":"double",
    "min":0.100000,
    "max":2.500000,
    "units":"seconds"
  }
}
```

To get more information about all of the parameters a function takes,
enter the function followed by two questions marks ??:

Request:

```shell
blinkLed ??
```

Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"blinkLed",
    "firmware":"BoardLedController",
    "parameters":[
      {
        "name":"duration_on",
        "type":"double",
        "min":0.100000,
        "max":2.500000,
        "units":"seconds"
      },
      {
        "name":"duration_off",
        "type":"double",
        "min":0.100000,
        "max":2.500000,
        "units":"seconds"
      },
      {
        "name":"count",
        "type":"long",
        "min":1,
        "max":100
      }
    ]
  }
}
```

It works the same if the question marks are before or after the
function.

Request:

```shell
?? blinkLed
```

Request:

```shell
blinkLed 3.0 0.2 20
```

Response:

```json
{
  "id":"blinkLed",
  "error":{
    "message":"Invalid params",
    "data":"Parameter value out of range: 0.100000 <= duration_on <= 2.500000",
    "code":-32602
  }
}
```

Request:

```shell
blinkLed 0.5 0.2 20
```

Response:

```json
{
  "id":"blinkLed",
  "result":null
}
```

Notice that the LED on the Arduino board has blinked 20 times, with an
on duration of 500ms and an off duration of 200ms.

Request:

```shell
getLedPin ?
```

Response:

```json
{
  "id":"getLedPin",
  "result":{
    "name":"getLedPin",
    "firmware":"BoardLedController",
    "result_info":{
      "type":"long"
    }
  }
}
```

Request:

```shell
getLedPin
```

Response:

```json
{
  "id":"getLedPin",
  "result":13
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
    "name":"board_led_controller",
    "form_factor":"3x2",
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
    "processor":"MK20DX256",
    "hardware":[
      {
        "name":"Teensy",
        "version":"3.2"
      }
    ],
    "firmware":[
      {
        "name":"ModularServer",
        "version":"3.0.0"
      },
      {
        "name":"BoardLedController",
        "version":"2.0.0"
      }
    ]
  }
}
```

Every function, parameter, property, and callback belongs to one
firmware set.

To get the API limited to one or more firmware sets, use the getApi function.
List the firmware sets you want API information on in an array or use the
special name "ALL" to see the API info from all firmware sets.

There are three API verbosity levels, NAMES, GENERAL, and DETAILED. NAMES only
shows the names of available functions, parameters, properties. GENERAL gives
info that is true for all devices with the same name, but different form factors.
DETAILED shows information specific to that particular device.

Request:

```shell
getApi NAMES ["ALL"]
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["ALL"],
    "verbosity":"NAMES",
    "functions":[
      "getDeviceId",
      "getDeviceInfo",
      "getApi",
      "getPropertyDefaultValues",
      "setPropertiesToDefaults",
      "getPropertyValues",
      "getInterruptInfo",
      "detachAllInterrupts",
      "setLedOn",
      "setLedOff",
      "getLedPin",
      "blinkLed"
    ],
    "parameters":[
      "firmware",
      "verbosity",
      "duration_on",
      "duration_off",
      "count"
    ],
    "properties":[
      "serialNumber"
    ]
  }
}
```

Request:

```shell
getApi NAMES ["BoardLedController"]
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["BoardLedController"],
    "verbosity":"NAMES",
    "functions":[
      "setLedOn",
      "setLedOff",
      "getLedPin",
      "blinkLed"
    ],
    "parameters":[
      "duration_on",
      "duration_off",
      "count"
    ]
  }
}
```

Request:

```shell
getApi GENERAL ["BoardLedController"]
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["BoardLedController"],
    "verbosity":"GENERAL",
    "functions":[
      {
        "name":"setLedOn"
      },
      {
        "name":"setLedOff"
      },
      {
        "name":"getLedPin",
        "result_info":{
          "type":"long"
        }
      },
      {
        "name":"blinkLed",
        "parameters":[
          "duration_on",
          "duration_off",
          "count"
        ]
      }
    ],
    "parameters":[
      {
        "name":"duration_on",
        "type":"double"
      },
      {
        "name":"duration_off",
        "type":"double"
      },
      {
        "name":"count",
        "type":"long"
      }
    ]
  }
}
```

Request:

```shell
getApi DETAILED ["BoardLedController"]
```

Response:

```json
{
  "id":"getApi",
  "result":{
    "firmware":["BoardLedController"],
    "verbosity":"DETAILED",
    "functions":[
      {
        "name":"setLedOn"
      },
      {
        "name":"setLedOff"
      },
      {
        "name":"getLedPin",
        "result_info":{
          "type":"long"
        }
      },
      {
        "name":"blinkLed",
        "parameters":[
          "duration_on",
          "duration_off",
          "count"
        ]
      }
    ],
    "parameters":[
      {
        "name":"duration_on",
        "type":"double",
        "min":0.100000,
        "max":2.500000,
        "units":"seconds"
      },
      {
        "name":"duration_off",
        "type":"double",
        "min":0.100000,
        "max":2.500000,
        "units":"seconds"
      },
      {
        "name":"count",
        "type":"long",
        "min":1,
        "max":100
      }
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
{'form_factor': '3x2', 'name': 'board_led_controller', 'serial_number': 0}
dev.get_methods()
['set_led_on',
 'get_interrupt_info',
 'get_api',
 'serial_number',
 'get_property_values',
 'get_device_id',
 'blink_led',
 'set_led_off',
 'get_led_pin',
 'get_property_default_values',
 'set_properties_to_defaults',
 'detach_all_interrupts',
 'get_device_info']
dev.set_led_on()
dev.set_led_off()
dev.blink_led()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 3 needed., code: -32602
dev.blink_led('?')
{'firmware': 'BoardLedController',
 'name': 'blinkLed',
 'parameters': ['duration_on', 'duration_off', 'count']}
dev.blink_led('duration_on','?')
{'firmware': 'BoardLedController',
 'max': 2.5,
 'min': 0.1,
 'name': 'duration_on',
 'type': 'double',
 'units': 'seconds'}
dev.blink_led('??')
{'firmware': 'BoardLedController',
 'name': 'blinkLed',
 'parameters': [{'max': 2.5,
   'min': 0.1,
   'name': 'duration_on',
   'type': 'double',
   'units': 'seconds'},
  {'max': 2.5,
   'min': 0.1,
   'name': 'duration_off',
   'type': 'double',
   'units': 'seconds'},
  {'max': 100, 'min': 1, 'name': 'count', 'type': 'long'}]}
dev.blink_led(3.0,0.2,20)
IOError: (from server) message: Invalid params, data: Parameter value out of range: 0.100000 <= duration_on <= 2.500000, code: -32602
dev.blink_led(0.5,0.2,20)
result = dev.get_led_pin('?')
dev.convert_to_json(result)
'{"firmware":"BoardLedController","name":"getLedPin","result_info":{"type":"long"}}'
dev.get_led_pin()
13
dev.call_get_result("get_led_pin")
13
dev.send_json_request('["get_led_pin"]')
13
dev.call("blink_led",0.5,0.2,20)
dev.send_json_request('["blink_led",0.5,0.2,20]')
dev.get_api('NAMES',["BoardLedController"])
{'firmware': ['BoardLedController'],
 'functions': ['setLedOn', 'setLedOff', 'getLedPin', 'blinkLed'],
 'parameters': ['duration_on', 'duration_off', 'count'],
 'verbosity': 'NAMES'}
```

For more details on the Python interface:

<https://github.com/janelia-pypi/modular_client_python>

### Matlab

Example Matlab session:

```matlab
% Linux and Mac OS X
ls /dev/tty*
serial_port = '/dev/ttyACM0';    % example Linux serial port
serial_port = '/dev/tty.usbmodem262471';% example Mac OS X serial port
% Windows
getAvailableComPorts()
ans =
  'COM1'
  'COM4'
serial_port = 'COM4';            % example Windows serial port
dev = ModularClient(serial_port);% creates a device object
dev.open()                       % opens a serial connection to the device
dev.getDeviceId()
ans =
  name: 'board_led_controller'
  form_factor: '3x2'
  serial_number: 0
dev.getMethods()                 % get device methods
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
  setLedOn
  setLedOff
  getLedPin
  blinkLed
  serialNumber
dev.blinkLed()
Error using ModularClient/sendRequest (line 301)
(from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 3 needed., code: -32602
method_info = dev.blinkLed('?')
method_info =
  name: 'blinkLed'
  firmware: 'BoardLedController'
  parameters: {'duration_on'  'duration_off'  'count'}
parameter_info = dev.blinkLed('duration_on','?')
parameter_info =
  name: 'duration_on'
  firmware: 'BoardLedController'
  type: 'double'
  min: 0.1000
  max: 2.5000
  units: 'seconds'
dev.blinkLed(3.0,0.2,20)
(from server) message: Invalid params, data: Parameter value out of range: 0.100000 <= duration_on <= 2.500000, code: -32602
dev.blinkLed(0.5,0.2,20);
led_pin = dev.getLedPin()
led_pin =
  13
led_pin = dev.callGetResult('getLedPin')
led_pin =
  13
led_pin = dev.sendJsonRequest('["getLedPin"]')
led_pin =
  13
dev.call('blinkLed',0.5,0.2,20)
dev.sendJsonRequest('["blinkLed",0.5,0.2,20]')
dev.getApi('NAMES',{'BoardLedController'})
ans =
  firmware: {'BoardLedController'}
  verbosity: 'NAMES'
  functions: {'setLedOn'  'setLedOff'  'getLedPin'  'blinkLed'}
  parameters: {'duration_on'  'duration_off'  'count'}
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

## Installation

<https://github.com/janelia-arduino/arduino-libraries>
