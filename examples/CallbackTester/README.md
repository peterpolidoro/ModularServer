#CallbackTester

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
      "name":"callback_tester",
      "form_factor":"5x3",
      "serial_number":0
    },
    "API":{
      "firmware":["all"],
      "functions":[
        "getDeviceId",
        "getDeviceInfo",
        "getInterruptInfo",
        "detachAllInterrupts",
        "getApi",
        "getApiVerbose",
        "getPropertyDefaultValues",
        "setPropertiesToDefaults",
        "getPropertyValues",
        "getMemoryFree"
      ],
      "parameters":[
        "firmware"
      ],
      "properties":[
        "serialNumber",
        "durationOn",
        "durationOff",
        "count"
      ],
      "callbacks":[
        "setLedOn",
        "setLedOff",
        "blinkLed"
      ]
    }
  }
}
```

"functions" is an array of user functions. To execute a function, simply
type it into the input property and press the 'Send' button or press the
'Enter' key.

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
        "interrupts":[
          "bnc_a",
          "bnc_b",
          "bnc_c",
          "bnc_d",
          "bnc_e",
          "bnc_f"
        ]
      }
    ],
    "firmware":[
      {
        "name":"ModularServer",
        "version":"2.0.0"
      },
      {
        "name":"CallbackTester",
        "version":"1.0.0"
      }
    ]
  }
}
```

Each hardware layer may add zero or more interrupts.

Request:

```shell
getInterruptInfo
```

Response:

```json
{
  "id":"getInterruptInfo",
  "result":[
    {
      "name":"bnc_a",
      "hardware":"Mega2560",
      "number":0,
      "pin":2,
      "callback":"setLedOn",
      "mode":"FALLING"
    },
    {
      "name":"bnc_b",
      "hardware":"Mega2560",
      "number":1,
      "pin":3,
      "callback":"setLedOff",
      "mode":"FALLING"
    },
    {
      "name":"bnc_c",
      "hardware":"Mega2560",
      "number":5,
      "pin":18,
      "callback":"setLedOn",
      "mode":"FALLING"
    },
    {
      "name":"bnc_d",
      "hardware":"Mega2560",
      "number":4,
      "pin":19,
      "callback":"setLedOff",
      "mode":"FALLING"
    },
    {
      "name":"bnc_e",
      "hardware":"Mega2560",
      "number":3,
      "pin":20,
      "callback":"blinkLed",
      "mode":"FALLING"
    },
    {
      "name":"bnc_f",
      "hardware":"Mega2560",
      "number":2,
      "pin":21,
      "callback":null,
      "mode":"DETACHED"
    }
  ]
}
```

Each interrupt is permanently assigned a name, an interrupt number,
and a hardware digital input pin. Each interrupt may be dynamically
assigned to a callback with a trigger mode or is detached and
inactive.

Use callbacks to attach or detach callbacks to interrupts. Callbacks
may be attached to one or more interrupts, but each interrupt can only
have a single callback attached.

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
    "firmware":"CallbackTester",
    "properties":[
      "durationOn",
      "durationOff",
      "count"
    ],
    "interrupts":[
      "bnc_e"
    ],
    "functions":[
      "trigger",
      "attachTo",
      "detachFrom",
      "detachFromAll"
    ],
    "parameters":[
      "interrupt",
      "mode"
    ]
  }
}
```

Request:

```shell
blinkLed trigger
```

Response:

```json
{
  "id":"blinkLed",
  "result":null
}
```

Request:

```shell
blinkLed
```

Response:

```json
{
  "id":"blinkLed",
  "result":null
}
```

Request:

```shell
blinkLed attachTo ??
```

Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"attachTo",
    "firmware":"ModularServer",
    "parameters":[
      {
        "name":"interrupt",
        "firmware":"ModularServer",
        "type":"string",
        "subset":[
          "bnc_a",
          "bnc_b",
          "bnc_c",
          "bnc_d",
          "bnc_e",
          "bnc_f"
        ]
      },
      {
        "name":"mode",
        "firmware":"ModularServer",
        "type":"string",
        "subset":[
          "LOW",
          "CHANGE",
          "RISING",
          "FALLING"
        ]
      }
    ],
    "result_type":null
  }
}
```

Request:

```shell
blinkLed attachTo bnc_f FALLING
```

Response:

```json
{
  "id":"blinkLed",
  "result":null
}
```

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
    "firmware":"CallbackTester",
    "properties":[
      "durationOn",
      "durationOff",
      "count"
    ],
    "interrupts":[
      "bnc_e",
      "bnc_f"
    ],
    "functions":[
      "trigger",
      "attachTo",
      "detachFrom",
      "detachFromAll"
    ],
    "parameters":[
      "interrupt",
      "mode"
    ]
  }
}
```

Request:

```shell
blinkLed detachFrom ?
```

Response:

```json
{
  "id":"blinkLed",
  "result":{
    "name":"detachFrom",
    "firmware":"ModularServer",
    "parameters":[
      "interrupt"
    ],
    "result_type":null
  }
}
```

Request:

```shell
count setValue 2
```

Response:

```json
{
  "id":"count",
  "result":2
}
```

Request:

```shell
blinkLed
```

Response:

```json
{
  "id":"blinkLed",
  "result":null
}
```

Request:

```shell
detachAllInterrupts
```

Response:

```json
{
  "id":"detachAllInterrupts",
  "result":null
}
```

Request:

```shell
getInterruptInfo
```

Response:

```json
{
  "id":"getInterruptInfo",
  "result":[
    {
      "name":"bnc_a",
      "hardware":"Mega2560",
      "number":0,
      "pin":2,
      "callback":null,
      "mode":"DETACHED"
    },
    {
      "name":"bnc_b",
      "hardware":"Mega2560",
      "number":1,
      "pin":3,
      "callback":null,
      "mode":"DETACHED"
    },
    {
      "name":"bnc_c",
      "hardware":"Mega2560",
      "number":5,
      "pin":18,
      "callback":null,
      "mode":"DETACHED"
    },
    {
      "name":"bnc_d",
      "hardware":"Mega2560",
      "number":4,
      "pin":19,
      "callback":null,
      "mode":"DETACHED"
    },
    {
      "name":"bnc_e",
      "hardware":"Mega2560",
      "number":3,
      "pin":20,
      "callback":null,
      "mode":"DETACHED"
    },
    {
      "name":"bnc_f",
      "hardware":"Mega2560",
      "number":2,
      "pin":21,
      "callback":null,
      "mode":"DETACHED"
    }
  ]
}
```

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

##Python

Example Python session:

```python
from modular_client import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '5x3', 'name': 'callback_tester', 'serial_number': 0}
dev.get_methods()
['count',
 'get_memory_free',
 'serial_number',
 'set_led_on',
 'get_interrupt_info',
 'get_api',
 'get_api_verbose',
 'duration_off',
 'get_property_values',
 'get_device_id',
 'blink_led',
 'set_led_off',
 'duration_on',
 'get_property_default_values',
 'set_properties_to_defaults',
 'detach_all_interrupts',
 'get_device_info']
dev.get_device_info()
{'firmware': [{'name': 'ModularServer', 'version': '2.0.0'},
  {'name': 'CallbackTester', 'version': '1.0.0'}],
 'hardware': [{'interrupts': ['bnc_a',
    'bnc_b',
    'bnc_c',
    'bnc_d',
    'bnc_e',
    'bnc_f'],
   'name': 'Mega2560'}],
 'processor': 'ATmega2560'}
dev.blink_led('?')
{'firmware': 'CallbackTester',
 'functions': ['trigger', 'attachTo', 'detachFrom', 'detachFromAll'],
 'interrupts': ['bnc_e'],
 'name': 'blinkLed',
 'parameters': ['interrupt', 'mode'],
 'properties': ['durationOn', 'durationOff', 'count']}
dev.set_properties_to_defaults()
dev.blink_led('trigger')
dev.blink_led()
dev.blink_led('attachTo','??')
{'firmware': 'ModularServer',
 'name': 'attachTo',
 'parameters': [{'firmware': 'ModularServer',
   'name': 'interrupt',
   'subset': ['bnc_a', 'bnc_b', 'bnc_c', 'bnc_d', 'bnc_e', 'bnc_f'],
   'type': 'string'},
  {'firmware': 'ModularServer',
   'name': 'mode',
   'subset': ['LOW', 'CHANGE', 'RISING', 'FALLING'],
   'type': 'string'}],
 'result_type': None}
dev.blink_led('attachTo','bnc_f','FALLING')
dev.count('setValue',2)
2
dev.blink_led()
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
  name: 'minimal_device'
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
  getMemoryFree
  serialNumber
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
