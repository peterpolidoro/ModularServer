LedController
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
  "methods": [
    "setLedOn",
    "setLedOff",
    "getLedPin",
    "blinkLed"
  ],
  "device_info": {
    "name": "led_controller",
    "model_number": 1234,
    "serial_number": 0,
    "firmware_number": 1
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
  "method": "setLedOn",
  "status": "success"
}
```

Notice that the LED on the Arduino board has turned ON.

Example Method:

```shell
blinkLed
```

Example Response:

```json
{
  "method": "blinkLed",
  "status": "error",
  "error_message": "Incorrect number of parameters. 0 given. 3 needed."
}
```

To get more information about a method, enter the method followed by
a question mark ?

Example Method Help:

```shell
blinkLed ?
```

Example Response:

```json
{
  "method": "blinkLed",
  "parameters": [
    "duration_on",
    "duration_off",
    "count"
  ],
  "status": "success"
}
```

The blinkLed method requires 3 parameters.

To get more information about a parameter, enter the method followed
by the parameter followed by a question mark ?

Example Parameter Help:

```shell
blinkLed duration_on ?
```

Example Response:

```json
{
  "method": "blinkLed",
  "parameter": {
    "name": "duration_on",
    "position": 0,
    "method": "blinkLed",
    "units": "ms"
  },
  "status": "success"
}
```

Example Method:

```shell
blinkLed 500 200 20
```

Example Response:

```json
{
  "method": "blinkLed",
  "status": "success"
}
```

Notice that the LED on the Arduino board has blinked 20 times, with an
on duration of 500ms and an off duration of 200ms.

####Python

Example Python session:

```python
from remote_device import RemoteDevice
dev = RemoteDevice() # Automatically finds device if one available
dev.get_methods()
['set_led_on', 'get_led_pin', 'blink_led', 'set_led_off']
dev.set_led_on()
dev.set_led_off()
dev.blink_led()
IOError: (from device) Incorrect number of parameters. 0 given. 3 needed.
dev.blink_led('?')
['duration_on', 'duration_off', 'count']
dev.blink_led('duration_on','?')
{'method': 'blinkLed', 'name': 'duration_on', 'position': 0, 'units': 'ms'}
dev.blink_led(500,200,20)
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
dev.getMethods()                 % get device methods
Remote Device Methods
---------------------
setLedOn
setLedOff
getLedPin
blinkLed
dev.setLedOn()
dev.setLedOff()
dev.blinkLed()
Error using RemoteDevice/sendCmd (line 308)
device responded with error, Incorrect number of parameters. 0 given. 3 needed.
dev.blinkLed('?')
ans =
'duration_on'    'duration_off'    'count'
dev.blinkLed(500,200,20)
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


