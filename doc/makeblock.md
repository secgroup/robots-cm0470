MakeBlock Quick-Start Guide
==========================

## Installation Instructions
Follow the steps 1 and 2 on http://learn.makeblock.com/en/learning-arduino-programming/ to download and install the Arduino IDE and the MakeBlock library (<a href="https://github.com/Makeblock-official/Makeblock-Libraries/">github</a>, <a href="https://github.com/Makeblock-official/Makeblock-Libraries/archive/master.zip">zip</a>).

Restart then the Arduino IDE and try some examples (File->Examples) to get familiar with 
code style and IDE functions, as in step 3 of the tutorial.

Note that the MBot Ranger that you'll have to deal with in the course is the **Auriga** model:
on board selection on the Arduino IDE you have to select "**Arduino Mega 2560**".

To flash the code on the MBot first connect it with the provided USB cable to your pc,
then select the correct serial port and press the upload button.  
Note that on linux you have to either change the permission of the file that 
represents the serial port (e.g. `/dev/ttyUSB0`) or add your user to the correct 
group (that you can retrieve with `ls -l /dev/ttyUSB0`).

The first try you can install the <a href="https://github.com/Makeblock-official/Makeblock-Libraries/blob/master/examples/Firmware_for_Auriga/Firmware_for_Auriga.ino">firmware for the auriga</a>, that contains 
the standard code for the robot to be controlled by the <a href="https://play.google.com/store/apps/details?id=cc.makeblock.makeblock&hl=en_US">companion app</a>.

## Programming
As previously mentioned the <a href="https://github.com/Makeblock-official/Makeblock-Libraries/tree/master/examples">examples</a> of the MakeBlock library are the **most valuable
and only tools** at your disposal to learn the basics of MBot programming.

Make sure that you specify the correct peripherals ports on the code according to the physical configuration
of your robot.

Every arduino program has two mandatory toplevel functions: `setup()` and `loop()`. The first one is called on startup and is needed to setup existing objects and initialize memory, and the second is called endlessly in a loop.

### Devices and sample code

Here you can find links to sample code for the different devices you will need for the lab project.
Make sure to change the `#include "MeOrion.h"` to `#include <MeAuriga.h>` to compile for the correct model.

Device | Example Code
-------|-------------
Ultrasonic Sensor | [UltrasonicSensortest.ino](https://github.com/Makeblock-official/Makeblock-Libraries/blob/master/examples/Me_UltrasonicSensor/UltrasonicSensorTest/UltrasonicSensorTest.ino)
Line Sensor | [LineFollowerTest.ino](https://github.com/Makeblock-official/Makeblock-Libraries/blob/master/examples/Me_LineFollower/LineFollowerTest/LineFollowerTest.ino)
Gyro Sensor | [MeGyroTest.ino](https://github.com/Makeblock-official/Makeblock-Libraries/blob/master/examples/Me_Gyro/MeGyroTest/MeGyroTest.ino)
Compass | [MeCompassTest.ino](https://github.com/Makeblock-official/Makeblock-Libraries/blob/master/examples/Me_Compass/MeCompassTest/MeCompassTest.ino)

## Bluetooth connectivity
To establish a bluetooth connection the RFCOMM protocol is used. Exploiting the fact that on the MBot robot the **USB Serial port shares its tx and rx with the bluetooth module**, it is possible to setup a serial bluetooth connection that behaves exactly as the USB serial of the Arduino.

Using the standard linux tools for the bluetooth communications, contained in the `bluez` and `bluez-utils` packages it is possible to connect to a bluetooth serial and setup a device `/dev/rfcomm0` that behaves like a standard serial interface.

For More information look at the [Bluetooth guide](bluetooth.md).

## Power Level
The power level and the charge of the batteries affect the performance in unpredictable ways (sensors may provide wrong data).
Keeping the batteries at medium/high level of power is enough to have reproducible results.

## Examples

- [Simple Line Following Program](../src/line_follower.ino)

