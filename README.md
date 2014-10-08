arduino_device_interface
------------------------

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

##Example Usage

###Arduino Firmware

[See example firmware](./Examples)

###Host Computer Interface

####Python

<https://github.com/JaneliaSciComp/python_arduino_device>

####Matlab

<https://github.com/JaneliaSciComp/matlab_arduino_device>

####Arduino Serial Monitor

Open the Serial Monitor in the Arduino IDE.

Set the baud rate to match the value in your Arduino sketch.

Set the line ending to 'Newline'.

Type a single question mark ? into the input field and press the
'Send' button or press the 'Enter' key.

Example Response for '?' command in ./Examples/LedController.ino:

```json
{
  "commands": [
    "setLedOn",
    "setLedOff",
    "getLedPin"
  ],
  "name": "arduino_led_controller"
}
```

"commands" is an array of user commands. To execute a command, simply
type it into the input field and press the 'Send' button or press the
'Enter' key.

Example Command for ./Examples/simple.ino:

```json
setLedOn
```
Example Response for 'setLedOn' command in ./Examples/LedController.ino:

```json
{
  "command": "setLedOn",
  "status": "success"
}
```

Notice that the LED on the Arduino board has turned ON.

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

####Clone This Repository into the Directory 'DeviceInterface'

```shell
git clone https://github.com/JaneliaSciComp/arduino_device_interface.git DeviceInterface
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


