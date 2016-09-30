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
    "device_id":{
      "name":"minimal_device",
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
        "getMemoryFree"
      ],
      "parameters":[
        "field_name",
        "field_value",
        "field_element_index"
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

Example Method (This method only exists on AVR processors):

```shell
getMemoryFree
```

Example Response:

```json
{
  "id":"getMemoryFree",
  "result":5398
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
    "device_id":{
      "name":"minimal_device",
      "form_factor":"mega",
      "serial_number":0
    },
    "device_info":{
      "processor":"ATmega2560",
      "firmware":[
        {
          "name":"ModularServer",
          "version":"1.0.0"
        },
        {
          "name":"MinimalDevice",
          "version":"1.0.0"
        }
      ],
      "hardware":[
        {
          "name":"mega"
        }
      ]
    },
    "API":{
      "methods":[
        {
          "name":"getDeviceId",
          "parameters":[],
          "result_type":"object"
        },
        {
          "name":"getDeviceInfo",
          "parameters":[],
          "result_type":"object"
        },
        {
          "name":"getApi",
          "parameters":[],
          "result_type":"object"
        },
        {
          "name":"getFieldDefaultValues",
          "parameters":[],
          "result_type":"object"
        },
        {
          "name":"setFieldsToDefaults",
          "parameters":[],
          "result_type":null
        },
        {
          "name":"setFieldToDefault",
          "parameters":[
            "field_name"
          ],
          "result_type":null
        },
        {
          "name":"getFieldValues",
          "parameters":[],
          "result_type":"object"
        },
        {
          "name":"getFieldValue",
          "parameters":[
            "field_name"
          ],
          "result_type":"value"
        },
        {
          "name":"getFieldElementValue",
          "parameters":[
            "field_name",
            "field_element_index"
          ],
          "result_type":"value"
        },
        {
          "name":"setFieldValue",
          "parameters":[
            "field_name",
            "field_value"
          ],
          "result_type":null
        },
        {
          "name":"setFieldElementValue",
          "parameters":[
            "field_name",
            "field_element_index",
            "field_value"
          ],
          "result_type":null
        },
        {
          "name":"setAllFieldElementValues",
          "parameters":[
            "field_name",
            "field_value"
          ],
          "result_type":null
        },
        {
          "name":"getMemoryFree",
          "parameters":[],
          "result_type":"long"
        }
      ],
      "parameters":[
        {
          "name":"field_name",
          "type":"string"
        },
        {
          "name":"field_value",
          "type":"value"
        },
        {
          "name":"field_element_index",
          "type":"long"
        }
      ],
      "fields":[
        {
          "name":"serial_number",
          "type":"long",
          "min":0,
          "max":65535,
          "value":0,
          "default_value":0
        }
      ]
    }
  }
}
```

Example Method with Parameters:

```shell
getFieldValue
```

Example Response:

```json
{
  "id":"getFieldValue",
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
getFieldValue ?
```

Example Response:

```json
{
  "id":"getFieldValue",
  "result":{
    "name":"getFieldValue",
    "parameters":[
      "field_name"
    ],
    "result_type":"value"
  }
}
```

Or you can enter a question mark followed by the method.

Example Method Help:

```shell
? getFieldValue
```

Example Response:

```json
{
  "id":"?",
  "result":{
    "name":"getFieldValue",
    "parameters":[
      "field_name"
    ],
    "result_type":"value"
  }
}
```

To get more verbose information about all of the parameters a method
takes, enter the method followed by two questions marks.

```shell
getFieldValue ??
```

Example Response:

```json
{
  "id":"getFieldValue",
  "result":{
    "name":"getFieldValue",
    "parameters":[
      {
        "name":"field_name",
        "type":"string"
      }
    ],
    "result_type":"value"
  }
}
```

Example Method:

```shell
getFieldValue serial_number
```

Example Response:

```json
{
  "id":"getFieldValue",
  "result":0
}
```

The serial number setting persists even after the device is powered
off. The serial number is used to differentiate several identical
devices connected to a single host machine at one time.

Example Method:

```shell
setFieldValue serial_number 32
```

Example Response:

```json
{
  "id":"setFieldValue",
  "result":null
}
```

```shell
getFieldValue serial_number
```

Example Response:

```json
{
  "id":"getFieldValue",
  "result":32
}
```

To reset the serial number to the default value, use the setFieldsToDefaults
method.

Example Method:

```shell
setFieldsToDefaults
```

Example Response:

```json
{
  "id":"setFieldsToDefaults",
  "result":null
}
```

```shell
getFieldValue serial_number
```

Example Response:

```json
{
  "id":"getFieldValue",
  "result":0
}
```

###Python

Example Python session:

```python
from modular_device import ModularDevice
dev = ModularDevice() # Automatically finds device if one available
dev.get_device_id()
{'form_factor': '', 'name': 'minimal_device', 'serial_number': 0}
dev.get_methods()
['get_memory_free',
 'get_field_value',
 'get_field_element_value',
 'get_api',
 'set_field_element_value',
 'set_all_field_element_values',
 'set_fields_to_defaults',
 'get_device_id',
 'get_field_default_values',
 'set_field_value',
 'set_field_to_default',
 'get_field_values',
 'get_device_info']
dev.get_memory_free()
5398
dev.get_field_value()
IOError: (from server) message: Invalid params, data: Incorrect number of parameters. 0 given. 1 needed., code: -32602
dev.get_field_value('?')
{'name': 'getFieldValue', 'parameters': ['field_name'], 'result_type': 'value'}
dev.get_field_value('serial_number')
0
dev.set_field_value('serial_number',-1)
IOError: (from server) message: Invalid params, data: Parameter value out of range: 0 <= serial_number <= 65535, code: -32602
dev.set_field_value('serial_number',12)
result = dev.call_server_method('?')
result['device_info']['serial_number']
12
dev.convert_to_json(result['device_info']['firmware_version'])
'{"major":0,"minor":1,"patch":0}'
dev.send_json_request('["set_fields_to_defaults"]')
dev.get_field_value('serial_number')
0
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
  name: 'minimal_device'
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
dev.getMemoryFree()
ans =
  5398
dev.getFieldValue()
(from server) message: Invalid params, Incorrect number of parameters. 0 given. 1 needed., code: -32602
method_info = dev.getFieldValue('?')
method_info
ans =
  name: 'getFieldValue'
  parameters: {'field_name'}
  result_type: 'value'
dev.getFieldValue('serial_number')
ans =
  0
dev.setFieldValue('serial_number',-1)
(from server) message: Invalid params, Parameter value out of range: 0 <= serial_number <= 65535, code: -32602
dev.setFieldValue('serial_number',13);
result = dev.callServerMethod('?');
result.device_info.serial_number
ans =
  13
json = dev.convertToJson(result.device_info.firmware_version)
json =
  {"major": 0,"minor": 1,"patch": 0}
dev.sendJsonRequest('["setFieldsToDefaults"]')
dev.getFieldValue('serial_number')
ans =
  0
dev.close()
clear dev
```

For more details on the Matlab interface:

<https://github.com/janelia-matlab/modular_client_matlab>

##Installation

<https://github.com/janelia-arduino/arduino-libraries>
