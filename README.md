# Smart Touch

A glove that acts as a wireless mouse based on hand gestures.

## Gestures

Left Click | Right Click | Move | Drag | Scroll

## Libraries Required

### For Python:

* [pySerial](https://pyserial.readthedocs.io/en/latest/pyserial.html) - for serial connection using XBee

* [pyAutoGUI](https://pypi.org/project/PyAutoGUI/) - to control the mouse

### For Arduino:

* [TimerOne](https://www.arduinolibraries.info/libraries/timer-one) - for PWM control

* [Wire](https://www.arduino.cc/en/reference/wire) - preinstalled in Arduino IDE

## Hardware used

* Arduino LilyPad 328 Main Board

* Arduino UNO

* SparkFun Xbee Explorer Regulated

* SparkFun FTDI Basic Breakout Board (3.3V)

* SparkFun IMU Breakout - MPU-9250

* LilyPad XBee (This a breakout board, different from the main board above)

* Flex Sensor 2.2" x 2

* A pair of XBee modules compatible with your parts

* SparkFun XBee Explorer USB - used for the XBee module's setup

## Setup

The detailed setup for the project is present in the "images" folder.

## Instructions to Run

* Make sure your XBee modules are configured

### With Breadboard :

* Run LilyPad.ino on the Arduino UNO

* Run python.py on your computer/IDE after connecting the breakout board to the computer using the FTDI

## Credits

* Readin data from IMU:  [Philippe Lucidarme](https://lucidar.me/en/inertial-measurement-unit/mpu-9250-and-arduino-9-axis-imu/)
