#ModularDevice

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

##Example Usage

###Arduino Firmware

This firmware requires at least 3.2KB of SRAM and 25.6KB of flash
memory, so it needs Arduino Mega level resources. Known to work with
the Arduino Mega 2560, Teensy 3.2, and chipKIT Max32.

[Arduino Sketch Examples](./examples)

###Host Computer Interface

For more detailed host computer interface information (Arduino Serial
Monitor, Python, and Matlab) see example sketch README files:

[LedController](./examples/LedController)

[MinimalDevice](./examples/MinimalDevice)

[StringController](./examples/StringController)

####Arduino Serial Monitor

Open the Serial Monitor in the Arduino IDE.

Set the baud rate to 9600.

Set the line ending to 'Newline'.

To get help information about the modular device, type a single
question mark ? into the input field and press the 'Send' button or
press the 'Enter' key.

```shell
?
```

The response will contain a field named "methods", which is an array
of user methods. To execute a method, simply type it into the input
field and press the 'Send' button or press the 'Enter' key.

To get more verbose help, including information about the parameters
each method takes, type two questions marks:

```shell
??
```

To get more information about a single method, enter the method
followed by a question mark '?'.

Example Method Help:

```shell
setSerialNumber ?
```

To get more verbose help about the method, including more information
about the parameters each method takes, type the method name followed
by two questions marks '??'.

Example Method Versbose Help:

```shell
setSerialNumber ??
```

####Python

<https://github.com/janelia-modular-devices/modular_device_python>

####Matlab

<https://github.com/janelia-modular-devices/modular_device_matlab>

##Installation

###Install This Library and its Dependencies Together

<https://github.com/janelia-arduino/arduino-libraries>
