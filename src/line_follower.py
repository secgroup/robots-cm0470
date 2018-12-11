#!/usr/bin/env python3

import sys
import signal
from time import sleep
import ev3dev.ev3 as ev3


# misc stuff
brick_but = ev3.Button()

# sensors
sensor_col = ev3.ColorSensor()
sensor_ultrasonic = ev3.UltrasonicSensor()
sensor_touch = ev3.TouchSensor()

# motors
motor_left = ev3.LargeMotor('outD')
motor_right = ev3.LargeMotor('outA')

devices = [sensor_col, sensor_ultrasonic, sensor_touch, motor_left, motor_right]

# global vars that can be adjusted after performing calibration
min_ref = 10
max_ref = 87
mid_ref = (10 + 87) / 2
kp = 0.9
power = -80


def stop_motors():
    motor_left.stop(stop_action='brake')
    motor_right.stop(stop_action='brake')


def signal_handler(signal, frame):
    stop_motors()
    print('Ctrl+C pressed, terminating...')
    sys.exit(1)


def steering(course, power):
    if course &gt;= 0:
        return power, power - ((power * course) // 100)
    else:
        return power + ((power * course) // 100), power


def run():
    sensor_col.mode = 'COL-REFLECT'

    motor_left.run_direct()
    motor_right.run_direct()

    while not (brick_but.any() or sensor_touch.is_pressed):
        # stop and beep if there's an obstacle
        while sensor_ultrasonic.distance_centimeters &lt; 15:
            stop_motors()
            ev3.Sound.tone([(100, 200, 0), (150, 200, 0), (130, 200, 0), (300, 1000, 0)]).wait()
            sleep(1)
        else:
            motor_left.run_direct()
            motor_right.run_direct()

        ref = sensor_col.value()
        # error ranges between -50 (W) and 50 (B), when it's close to 0 ref == mid_ref
        error = mid_ref - (100 * (ref - min_ref) / (max_ref - min_ref))
        course = (kp * error)
        motor_left.duty_cycle_sp, motor_right.duty_cycle_sp = steering(course, power)

        sleep(0.01)

    stop_motors()


def main():
    # check that all devices are connected
    assert all(d.connected for d in devices) is True

    # capture ctrl-c and stop motors in that case
    signal.signal(signal.SIGINT, signal_handler)

    ev3.Sound.speak("3 2 1 go!").wait()
    sleep(3)
    run()


if __name__ == '__main__':
    main()
