EV3DEV TL;DR Bootstrap Guide
============================

The contents in this section refer to a Linux device, if you’re using a different OS please refer to the <a href="http://www.ev3dev.org/">ev3dev</a> website.

Select an image to download from http://www.ev3dev.org/downloads/, I tested the development branch based on the stable Debian version. Extract the image and flash your SD card:

```sh
$ unxz snapshot-ev3dev-stretch-ev3-generic-2017-11-01.img.xz
$ sudo dd if=snapshot-ev3dev-stretch-ev3-generic-2017-11-01.img of=/dev/mmcblk0 bs=4M
```

Follow the <a href="http://www.ev3dev.org/docs/tutorials/connecting-to-the-internet-via-bluetooth/">Bluetooth guide</a> to setup the network between the brick and your host. Bluetooth is recommended over USB to remotely control the robot with Once it’s done, connect to it using SSH:

```sh
$ ssh robot@10.42.0.164
```

The default password is `maker`.

Update the software index to install `tmux`, a useful program to detach your session from the Unix shell that started the connection to the robot. Also update the python library while you’re there:

```sh
$ sudo -i
# apt update
# apt install tmux
# apt install --only-upgrade python3-ev3dev
# exit
$ tmux
```

In case the connection to the brick is temporarily lost while working inside a `tmux` session, you can recover it by executing `tmux a`.

## Programming

Python library documentation can be found on the <a href="https://github.com/ev3dev/ev3dev-lang-python">ev3dev-lang-python</a> Github page. This API allows to easily control motors, sensors, hardware buttons, LCD displays and more from Python code. Full API documentation is available <a href="http://python-ev3dev.readthedocs.io/en/stable/spec.html">here</a>.

The following examples have been tested with Python3, assuming that the `ev3` library has been imported with `import ev3dev.ev3 as ev3`

### Motors

#### Large Motor

Motors support different modes. In `run-direct` mode, the motor will run at the duty cycle specified. Unlike other run commands, changing `duty_cycle_sp` while running will take effect immediately.

```python
>>> motor = ev3.LargeMotor('outA')
>>> motor.commands
['run-forever', 'run-to-abs-pos', 'run-to-rel-pos', 'run-timed', 'run-direct', 'stop', 'reset']
>>> motor.run_direct()
>>> motor.duty_cycle_sp = 10
>>> motor.duty_cycle_sp = 60
>>> motor.stop()
```

Depending on how you built your robot, it could be handy to invert the polarity of the motor to avoid specifying negative power values to get it moving forward.

```python
>>> motor = ev3.LargeMotor('outA')
>>> motor.reset()
>>> motor.polarity
'normal'
>>> motor.run_direct()
>>> motor.duty_cycle_sp = 30
>>> motor.stop()
>>>
>>> motor.reset()
>>> motor.polarity = 'inversed'
>>> motor.run_direct()
>>> motor.duty_cycle_sp = 30
>>> motor.stop()
```

### Sensors

#### Touch Sensor

This is the most trivial sensor available in the set. It only detects whether the button has been pushed or not, without providing pressure levels.

```python
>>> sensor_touch = ev3.TouchSensor()
>>> sensor_touch.is_pressed
0
>>> sensor_touch.is_pressed
1
```

#### Ultrasonic Sensor

A simple sensor that allows to measure distance in centimeters. It is pretty accurate and the view cone is quite narrow.

```python
>>> sensor_ultrasonic = ev3.UltrasonicSensor()
>>> sensor_ultrasonic.distance_centimeters
14.5
>>> sensor_ultrasonic.distance_centimeters
54.2
```

Notice that when the eyes are fully covered by an obstacle, the reported distance is `255`.

#### Infrared Sensor

The infrared sensor supports different modes which allows, respectively, to measure the distance to an object in front of it, the position of the infrared beacon and receive commands from the beacon acting as a remote controller. It is not very accurate

```python
>>> sensor_ir = ev3.InfraredSensor()
>>> sensor_ir.modes
['IR-PROX', 'IR-SEEK', 'IR-REMOTE', 'IR-REM-A', 'IR-S-ALT', 'IR-CAL']
```

Proximity is returned in percentage, 100% is approximately 70cm. When the sensor is covered the result could be 100, 1 or a random value. The view cone is large and it’s significantly affected by interferences with other IR sensors, even when they’re place perpendicularly.

```python
>>> sensor_ir.mode
'IR-PROX'
>>> sensor_ir.proximity
83
>>> sensor_ir.proximity
8
```

IR Seeker mode is supported by the `BeaconSeeker` class. Heading is the angle (either positive or negative) where the beacon is located and distance reports the gap between the sensor and the beacon.

```python
>>> sensor_ir = ev3.BeaconSeeker()
>>> sensor_ir.heading_and_distance
(25, 26)
```

Both the IR beacon and sensor support 4 different channels that could be used to locate the position of up to 4 different beacons. The channel on the beacon can be set using the red switch, while on the sensor it can be modified using the attribute `sensor_ir._channel`.

Remote control can be enabled using the `ev3.RemoteControl` class:

```python
>>> sensor_ir = ev3.RemoteControl()
>>> sensor_ir.blue_up
False
>>> sensor_ir.blue_up
True
```

#### Color Sensor

This rather powerful sensor supports different modes, such as measuring ambient light, identify a color among a set of 8 standard colors, return the amount of reflected light and work in raw mode, theoretically supporting a wider set of colors.

Ambient light mode (blue led on):

```python
>>> sensor_col = ev3.ColorSensor()
>>> sensor_col.ambient_light_intensity
38
```

Reflected light mode (red led on). Notice that it’s not needed to manually switch mode, it’s set automatically once using reading the attribute:

```python
>>> sensor_col.reflected_light_intensity
38
```

Color mode (all leds rapidly cycling). The reported value is determined by several factors, such as the material, the distance between the sensor and the colored surface, the ambient light. The colors detected by the sensor are no color (0), black (1), blue (2), green (3), yellow (4), red (5), white (6), brown (7):

```python
>>> sensor_col.color
5
```

Raw color mode, returned as a RGB triple:

```python
>>> sensor_col.raw
(22, 19, 18)
```

In case the task is to follow a line and identify a colored marker at the same time, it is advisable to use the raw mode and transform RGB values into HSV and use the saturation value for the line tracking part, while other values to detect the marker.


## Examples

- [Simple Line Following Program](../src/line_follower.py)
