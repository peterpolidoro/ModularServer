MinimalDevice
-------------

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

###Host Computer Interface

####Arduino Serial Monitor

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
    "name":"minimal_device",
    "model_number":1000,
    "serial_number":0,
    "firmware_number":1
  },
  "methods":[
    "getMemoryFree",
    "resetDefaults",
    "setSerialNumber"
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
  "memory_free":4874,
  "status":success
}
```

To get more verbose help about the Arduino device, including
information about the parameters each method takes, type two question
marks ?? into the input field and press the 'Send' button or press the
'Enter' key.

```shell
??
```

Example Response:

```json
{
  "method":"??",
  "device_info":{
    "name":"minimal_device",
    "model_number":1000,
    "serial_number":0,
    "firmware_number":1
  },
  "methods":[
    {
      "getMemoryFree":{
        "parameters":[]
      }
    },
    {
      "resetDefaults":{
        "parameters":[]
      }
    },
    {
      "setSerialNumber":{
        "parameters":[
          "serial_number"
        ]
      }
    }
  ],
  "status":success
}
```

Example Method with Parameters:

```shell
setSerialNumber
```

Example Response:

```json
{
  "method":"setSerialNumber",
  "status":error,
  "error_message":"Incorrect number of parameters. 0 given. 1 needed."
}
```

To get more information about a method, enter the method followed by
a question mark ?

Example Method Help:

```shell
setSerialNumber ?
```

Example Response:

```json
{
  "method":"setSerialNumber",
  "parameters":[
    "serial_number"
  ],
  "status":success
}
```

To get more verbose information about all of the parameters a method
takes, enter the method followed by two questions marks ??:

```shell
setSerialNumber ??
```

Example Response:

```json
{
  "method":"setSerialNumber",
  "parameters":[
    {
      "serial_number":{
        "type":"long",
        "min":0,
        "max":65535
      }
    }
  ],
  "status":success
}
```

Example Method:

```shell
setSerialNumber 32
```

Example Response:

```json
{
  "method":"setSerialNumber",
  "status":success
}
```

The serial number setting persists even after the device is powered
off. The serial number is used to differentiate several identical
devices connected to a single host machine at one time.

To reset the serial number to the default value, use the resetDefaults
method.

Example Method:

```shell
resetDefaults
```

Example Response:

```json
{
  "method":"resetDefaults",
  "status":success
}
```

####Python

Example Python session:

```python
from remote_device import RemoteDevice
dev = RemoteDevice() # Automatically finds device if one available
dev.get_device_info()
{'firmware_number': 1,
 'model_number': 1000,
 'name': 'minimal_device',
 'serial_number': 0}
dev.get_methods()
['set_serial_number', 'get_memory_free', 'reset_defaults']
dev.get_memory_free()
4874
dev.set_serial_number()
IOError: (from device) Incorrect number of parameters. 0 given. 1 needed.
dev.set_serial_number('?')
['serial_number']
dev.set_serial_number('??')
[{'serial_number': {'max': 65535, 'min': 0, 'type': 'long'}}]
dev.set_serial_number(-1)
IOError: (from device) Parameter value out of range: 0 <= serial_number <= 65535
dev.set_serial_number(12)
```

For more details on the Python interface:

<https://github.com/JaneliaSciComp/python_remote_device>

####Matlab

Example Matlab session:

```matlab
% Linux and Mac OS X
ls /dev/tty*
serial_port = '/dev/ttyACM0'     % example Linux serial port
serial_port = '/dev/tty.usbmodem262471' % example Mac OS X serial port
% Windows
getAvailableComPorts()
serial_port = 'COM4'             % example Windows serial port
dev = RemoteDevice(serial_port)  % creates a device object
dev.open()                       % opens a serial connection to the device
device_info = dev.getDeviceInfo()
device_info = 
               name: 'minimal_device'
       model_number: 1000
      serial_number: 0
    firmware_number: 1
dev.getMethods()                 % get device methods
Remote Device Methods
---------------------
getMemoryFree
resetDefaults
setSerialNumber
dev.getMemoryFree()
ans =
        4874
dev.setSerialNumber()
Error using RemoteDevice/sendRequest (line 309)
device responded with error, Incorrect number of parameters. 0 given. 1 needed.
dev.setSerialNumber('?')
ans =
serial_number
dev.setSerialNumber('serial_number','?')
ans =
    type: 'long'
     min: 0
     max: 65535
dev.setSerialNumber(-1)
Error using RemoteDevice/sendRequest (line 309)
device responded with error, Parameter value out of range: 0 <= serial_number <= 65535
dev.setSerialNumber(13)
dev.close()                      % close serial connection
delete(dev)                      % deletes the device
```

For more details on the Matlab interface:

<https://github.com/JaneliaSciComp/matlab_remote_device>

##Installation

This library and its dependencies can be installed together or
separately.

###Install This Library and its Dependencies Together

Clone the arduino-libraries repository:

```shell
git clone https://github.com/JaneliaSciComp/arduino-libraries.git
```

Install symbolic library links into the default sketchbook directory
using Python when running Linux or Mac OS X:

```shell
python symlinks.py --install
```
Or for more information on installing libraries, see:

<http://arduino.cc/en/Guide/Libraries>

###Install This Library and its Dependencies Separately

####Clone This Repository into the Directory 'RemoteDevice'

```shell
git clone https://github.com/JaneliaSciComp/arduino_remote_device.git RemoteDevice
```

Install symbolic library link into the default sketchbook directory
using Python when running Linux or Mac OS X:

```shell
python symlink.py --install
```
Or for more information on installing libraries, see:

<http://arduino.cc/en/Guide/Libraries>

####Download and Install the Arduino Library Dependencies

Streaming:

<http://arduiniana.org/libraries/streaming/>

ArduinoJson:

<https://github.com/bblanchon/ArduinoJson>

StandardCplusplus:

<https://github.com/maniacbug/StandardCplusplus>


