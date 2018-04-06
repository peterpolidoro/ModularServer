# CallbackTester

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
      "name": "callback_tester",
      "form_factor": "3x2",
      "serial_number": 0
    },
    "api": {
      "firmware": [
        "CallbackTester"
      ],
      "verbosity": "NAMES",
      "properties": [
        "durationOn",
        "durationOff",
        "count"
      ],
      "callbacks": [
        "setLedOn",
        "setLedOff",
        "blinkLed"
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
      "name": "callback_tester",
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
        "getPinInfo",
        "detachAllPins"
      ],
      "parameters": [
        "firmware",
        "verbosity"
      ],
      "properties": [
        "serialNumber",
        "durationOn",
        "durationOff",
        "count"
      ],
      "callbacks": [
        "setLedOn",
        "setLedOff",
        "blinkLed"
      ]
    }
  }
}
```

"functions" is an array of user functions. To execute a function, simply type it
into the input property and press the 'Send' button or press the 'Enter' key.

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
        "version": "3.2",
        "pins": [
          "bnc_a",
          "bnc_b",
          "bnc_c",
          "bnc_d",
          "bnc_e",
          "bnc_f"
        ]
      }
    ],
    "firmware": [
      {
        "name": "ModularServer",
        "version": "3.0.0"
      },
      {
        "name": "CallbackTester",
        "version": "2.0.0"
      }
    ]
  }
}
```

Each hardware layer may add zero or more pins.

Request:

```shell
getPinInfo
```

Response:

```json
{
  "id": "getPinInfo",
  "result": [
    {
      "name": "bnc_a",
      "hardware": "Teensy",
      "callback": "setLedOn",
      "pin_mode": "INTERRUPT_FALLING"
    },
    {
      "name": "bnc_b",
      "hardware": "Teensy",
      "callback": "setLedOff",
      "pin_mode": "INTERRUPT_FALLING"
    },
    {
      "name": "bnc_c",
      "hardware": "Teensy",
      "callback": "setLedOn",
      "pin_mode": "INTERRUPT_FALLING"
    },
    {
      "name": "bnc_d",
      "hardware": "Teensy",
      "callback": "setLedOff",
      "pin_mode": "INTERRUPT_FALLING"
    },
    {
      "name": "bnc_e",
      "hardware": "Teensy",
      "callback": "blinkLed",
      "pin_mode": "INTERRUPT_FALLING"
    },
    {
      "name": "bnc_f",
      "hardware": "Teensy",
      "callback": "setLedOff",
      "pin_mode": "INTERRUPT_FALLING"
    }
  ]
}
```

Each pin may be dynamically assigned to a callback with a trigger mode or
the pin can be detached and inactive.

Use callbacks to attach or detach callbacks to pins. Callbacks may be
attached to one or more pins, but each pin can only have a single
callback attached.

Request:

```shell
blinkLed ?
```

Response:

```json
{
  "id": "blinkLed",
  "result": {
    "name": "blinkLed",
    "firmware": "CallbackTester",
    "properties": [
      "durationOn",
      "durationOff",
      "count"
    ],
    "pins": [
      "bnc_e"
    ],
    "functions": [
      "trigger",
      "attachTo",
      "detachFrom",
      "detachFromAll"
    ],
    "parameters": [
      "pin_name",
      "pin_mode"
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
  "id": "blinkLed",
  "result": null
}
```

Request:

```shell
blinkLed
```

Response:

```json
{
  "id": "blinkLed",
  "result": null
}
```

Request:

```shell
blinkLed attachTo ??
```

Response:

```json
{
  "id": "blinkLed",
  "result": {
    "name": "attachTo",
    "firmware": "ModularServer",
    "parameters": [
      {
        "name": "pin_name",
        "type": "string",
        "subset": [
          "bnc_a",
          "bnc_b",
          "bnc_c",
          "bnc_d",
          "bnc_e",
          "bnc_f"
        ]
      },
      {
        "name": "pin_mode",
        "type": "string",
        "subset": [
          "LOW",
          "CHANGE",
          "RISING",
          "INTERRUPT_FALLING"
        ]
      }
    ]
  }
}
```

Request:

```shell
blinkLed attachTo bnc_f INTERRUPT_FALLING
```

Response:

```json
{
  "id": "blinkLed",
  "result": null
}
```

Request:

```shell
blinkLed ?
```

Response:

```json
{
  "id": "blinkLed",
  "result": {
    "name": "blinkLed",
    "firmware": "CallbackTester",
    "properties": [
      "durationOn",
      "durationOff",
      "count"
    ],
    "pins": [
      "bnc_e",
      "bnc_f"
    ],
    "functions": [
      "trigger",
      "attachTo",
      "detachFrom",
      "detachFromAll"
    ],
    "parameters": [
      "pin_name",
      "pin_mode"
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
  "id": "blinkLed",
  "result": {
    "name": "detachFrom",
    "firmware": "ModularServer",
    "parameters": [
      "pin_name"
    ]
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
  "id": "count",
  "result": 2
}
```

Request:

```shell
blinkLed
```

Response:

```json
{
  "id": "blinkLed",
  "result": null
}
```

Request:

```shell
detachAllPins
```

Response:

```json
{
  "id": "detachAllPins",
  "result": null
}
```

Request:

```shell
getPinInfo
```

Response:

```json
{
  "id": "getPinInfo",
  "result": [
    {
      "name": "bnc_a",
      "hardware": "Teensy",
      "pin_mode": "DETACHED"
    },
    {
      "name": "bnc_b",
      "hardware": "Teensy",
      "pin_mode": "DETACHED"
    },
    {
      "name": "bnc_c",
      "hardware": "Teensy",
      "pin_mode": "DETACHED"
    },
    {
      "name": "bnc_d",
      "hardware": "Teensy",
      "pin_mode": "DETACHED"
    },
    {
      "name": "bnc_e",
      "hardware": "Teensy",
      "pin_mode": "DETACHED"
    },
    {
      "name": "bnc_f",
      "hardware": "Teensy",
      "pin_mode": "DETACHED"
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
  "id": "setLedOn",
  "result": null
}
```

### Python

Example Python session:

```python
from modular_client import ModularClient
dev = ModularClient() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '3x2', 'name': 'callback_tester', 'serial_number': 0}
dev.get_methods()
['count',
 'set_led_on',
 'get_pin_info',
 'get_api',
 'serial_number',
 'duration_off',
 'get_property_values',
 'get_device_id',
 'blink_led',
 'set_led_off',
 'duration_on',
 'get_property_default_values',
 'set_properties_to_defaults',
 'detach_all_pins',
 'get_device_info']
dev.set_properties_to_defaults(['ALL'])
dev.get_device_info()
{'firmware': [{'name': 'ModularServer', 'version': '3.0.0'},
  {'name': 'CallbackTester', 'version': '2.0.0'}],
 'hardware': [{'pins': ['bnc_a',
    'bnc_b',
    'bnc_c',
    'bnc_d',
    'bnc_e',
    'bnc_f'],
   'name': 'Teensy',
   'version': '3.2'}],
 'processor': 'MK20DX256'}
dev.blink_led('?')
{'firmware': 'CallbackTester',
 'functions': ['trigger', 'attachTo', 'detachFrom', 'detachFromAll'],
 'pins': [],
 'name': 'blinkLed',
 'parameters': ['pin_name', 'pin_mode'],
 'properties': ['durationOn', 'durationOff', 'count']}
dev.blink_led('trigger')
dev.blink_led()
dev.blink_led('attachTo','??')
{'firmware': 'ModularServer',
 'name': 'attachTo',
 'parameters': [{'name': 'pin_name',
   'subset': ['bnc_a', 'bnc_b', 'bnc_c', 'bnc_d', 'bnc_e', 'bnc_f'],
   'type': 'string'},
  {'name': 'pin_mode',
   'subset': ['LOW', 'CHANGE', 'RISING', 'INTERRUPT_FALLING'],
   'type': 'string'}]}
dev.blink_led('attachTo','bnc_f','INTERRUPT_FALLING')
dev.count('setValue',2)
2
dev.blink_led()
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
  name: 'callback_tester'
  form_factor: '3x2'
  serial_number: 0
dev.getMethods()                 % get device methods
ans =
  Modular Device Methods
  ---------------------
  getDeviceId
  getDeviceInfo
  getApi
  getPropertyDefaultValues
  setPropertiesToDefaults
  getPropertyValues
  getPinInfo
  detachAllPins
  setLedOn
  setLedOff
  blinkLed
  serialNumber
  durationOn
  durationOff
  count
dev.setPropertiesToDefaults({'ALL'});
dev.blinkLed('?')
ans =
  name: 'blinkLed'
  firmware: 'CallbackTester'
  properties: {'durationOn'  'durationOff'  'count'}
  pins: {'bnc_e'}
  functions: {'trigger'  'attachTo'  'detachFrom'  'detachFromAll'}
  parameters: {'pin_name'  'pin_mode'}
dev.blinkLed('trigger');
dev.blinkLed();
dev.blinkLed('attachTo','??')
ans =
  name: 'attachTo'
  firmware: 'ModularServer'
  parameters: {[1x1 struct]  [1x1 struct]}
dev.blinkLed('attachTo','bnc_f','INTERRUPT_FALLING');
dev.count('setValue',2)
ans =
  2
dev.blinkLed();
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

## Installation

<https://github.com/janelia-arduino/arduino-libraries>
