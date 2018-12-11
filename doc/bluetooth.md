Bluetooth Connection
====================

Exploiting the fact that on the MBot robot the USB Serial port **shares its tx and rx with the bluetooth module**, it is possible to setup a serial bluetooth connection that behaves exactly as the USB serial of the Arduino.

Using the standard linux tools for the bluetooth communications it is possible to connect to a bluetooth serial and setup a device `/dev/rfcomm0 `that behaves like a regular serial interface.

The two robots can be connected using this technique, since the lego robot runs a linux os.

## Requirements
- `bluez` library
- `rfcomm` binary from `bluez-utils` or `bluez-utils-compat`
- `python3-pyserial` for serial communication and the `miniterm` utility

To install all the requirements on the ev3dev linux (and every Debian-based distros):

```sh
$ sudo -i
# apt install bluez bluez-utils bluez-utils-compat python3-pyserial
```

## Setup

Use the `bluetoothctl` utility to scan for nearby bluetooth devices

```sh
$ bluetoothctl
[bluetooth]# scan on
```

Find the Makeblock MAC address in the discovered devices and pair it

``` sh
[bluetooth]# scan off
[bluetooth]# trust 00:1B:10:30:4B:17
[bluetooth]# pair 00:1B:10:30:4B:17
```

If you are setting up the connection directly on the lego robot, a new pop-up window should
appear on the LCD display asking to accept the pairing. Once accepted the two devices are successfully paired.

The next step is to setup `/dev/rfcomm0` device

```
# rfcomm connect /dev/rfcomm0 00:1B:10:30:4B:17
# chmod 777 /dev/rfcomm0
```

And test the connection

```
python3 -m serial.tools.miniterm /dev/rfcomm0 115200
```

You should have now a fully working serial console connected to the MBot robot.

To use this new device in your python scripts you can use the `pyserial` python module (https://pythonhosted.org/pyserial/shortintro.html#opening-serial-ports)
