#BoardLedController

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
      "name":"board_led_controller",
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
        "setLedOn",
        "setLedOff",
        "getLedPin",
        "blinkLed"
      ],
      "parameters":[
        "field_name",
        "field_value",
        "field_element_index",
        "duration_on",
        "duration_off",
        "count"
      ],
      "fields":[
        "serial_number"
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
setLedOn
```

Example Response:

```json
{
  "id":"setLedOn",
  "result":null
}
```

Notice that the LED on the Arduino board has turned ON.

Example Method with Parameters:

```shell
blinkLed
```

Example Response:

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

When a method executes successfully, the response will contain a
"result" field. In some cases the result may be null, but the method
execution was still successful. When there is an error, there will not
exist a "result" field, but there will exist an "error" field.

To get more information about a method, enter the method followed by
a question mark ?

Example Method Help:

```shell
blinkLed ?
```

Example Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"blinkLed",
    "parameters":[
      "duration_on",
      "duration_off",
      "count"
    ],
    "result_type":null
  }
}
```

The blinkLed method requires 3 parameters.

To get more information about a single method parameter, enter the
method followed by the parameter followed by a question mark ?

Example Parameter Help:

```shell
blinkLed duration_on ?
```

Example Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"duration_on",
    "units":"seconds",
    "type":"double",
    "min":0.100000,
    "max":2.500000
  }
}
```

To get more information about all of the parameters a method takes,
enter the method followed by two questions marks ??:

```shell
blinkLed ??
```

Example Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"blinkLed",
    "parameters":[
      {
        "name":"duration_on",
        "units":"seconds",
        "type":"double",
        "min":0.100000,
        "max":2.500000
      },
      {
        "name":"duration_off",
        "units":"seconds",
        "type":"double",
        "min":0.100000,
        "max":2.500000
      },
      {
        "name":"count",
        "type":"long",
        "min":1,
        "max":100
      }
    ],
    "result_type":null
  }
}
```

It works the same if the question marks are before or after the
method.

```shell
?? blinkLed
```

Example Method:

```shell
blinkLed 3.0 0.2 20
```

Example Response:

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

Example Method:

```shell
blinkLed 0.5 0.2 20
```

Example Response:

```json
{
  "id":"blinkLed",
  "result":null
}
```

Notice that the LED on the Arduino board has blinked 20 times, with an
on duration of 500ms and an off duration of 200ms.

Example Method:

```shell
getLedPin ?
```

Example Response:

```json
{
  "id":"getLedPin",
  "result":{
    "name":"getLedPin",
    "parameters":[],
    "result_type":"long"
  }
}
```

Example Method:

```shell
getLedPin
```

Example Response:

```json
{
  "id":"getLedPin",
  "result":13
}
```

###Python

Example Python session:

```python
from modular_device import ModularDevice
dev = ModularDevice() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '', 'name': 'board_led_controller', 'serial_number': 0}
dev.get_methods()
['get_memory_free',
 'get_field_value',
 'get_field_element_value',
 'set_led_on',
 'get_led_pin',
 'get_api',
 'set_field_element_value',
 'set_all_field_element_values',
 'set_fields_to_defaults',
 'get_device_id',
 'get_field_default_values',
 'blink_led',
 'set_led_off',
 'set_field_value',
 'set_field_to_default',
 'get_field_values',
 'get_device_info']
 dev.set_led_on()
dev.set_led_off()
dev.blink_led()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 3 needed., code: -32602
dev.blink_led('?')
{'name': 'blinkLed',
 'parameters': ['duration_on', 'duration_off', 'count'],
 'result_type': None}
dev.blink_led('duration_on','?')
{'max': 2.5,
 'min': 0.1,
 'name': 'duration_on',
 'type': 'double',
 'units': 'seconds'}
dev.blink_led('??')
{'name': 'blinkLed',
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
  {'max': 100, 'min': 1, 'name': 'count', 'type': 'long'}],
 'result_type': None}
dev.blink_led(3.0,0.2,20)
IOError: (from server) message: Invalid params, data: Parameter value out of range: 0.100000 <= duration_on <= 2.500000, code: -32602
dev.blink_led(0.5,0.2,20)
result = dev.get_led_pin('?')
dev.convert_to_json(result)
'{"name":"getLedPin","parameters":[],"result_type":"long"}'
dev.get_led_pin()
13
dev.call_server_method("get_led_pin")
13
dev.send_json_request('["get_led_pin"]')
13
dev.call_server_method("blink_led",0.5,0.2,20)
dev.send_json_request('["blink_led",0.5,0.2,20]')
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
dev = ModularDevice(serial_port) % creates a device object
dev.open()                       % opens a serial connection to the device
dev.getDeviceId()
ans =
  name: 'board_led_controller'
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
setLedOn
setLedOff
getLedPin
blinkLed
dev.setLedOn();
dev.setLedOff();
dev.blinkLed();
Error using ModularDevice/sendRequest (line 301)
(from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 3 needed., code: -32602
method_info = dev.blinkLed('?')
method_info =
  name: 'blinkLed'
  parameters: {'duration_on'    'duration_off'    'count'}
  result_type: []
parameter_info = dev.blinkLed('duration_on','?')
parameter_info =
  name: 'duration_on'
  units: 'seconds'
  type: 'double'
  min: 0.100000
  max: 2.500000
dev.blinkLed(3.0,0.2,20)
(from server) message: Invalid params, data: Parameter value out of range: 0.100000 <= duration_on <= 2.500000, code: -32602
dev.blinkLed(0.5,0.2,20);
dev.getLedPin()
ans =
  13
led_pin = dev.callServerMethod('getLedPin')
led_pin =
  13
led_pin = dev.sendJsonRequest('["getLedPin"]')
led_pin =
  13
dev.callServerMethod('blinkLed',0.5,0.2,20)
dev.sendJsonRequest('["blinkLed",0.5,0.2,20]')
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
