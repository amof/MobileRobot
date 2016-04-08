MobileRobot
============

A personal project based on a 4WD MobileRobot.

##Goal

The first goal is to developp a remote control car over Bluetooth.

The second objective is to make the robot autonomous. Then, it will have the ability to transmit informations to the PC if there is a bluetooth connection available.

## Softwares
- QT 5.6 (latest version) + QT Creator
- Atmel Studio 7 with GCC 4.9.2

## Hardware
- 4WD Mobile Robot
- ATMega 1284P-PU
- Bluetooth module JY-MCU v1.06
- Adafruit Motor Shield V2.0 (PCA9685PW chip for DC motor control)

## Installation Procedure
There is not yet a .exe to execute directly to your PC.
So to have access and launch successfully the software, you must :
- Download the softwares
- Get the hardware
- Clone this repository to your desktop
- Load the .pro file located in /MobileRobot_Software and compile it (no extra libraries needed)
- Load the .atsln located in /MobileRobot_Embedded and upload in your µc

## Development roadmap
###V1.0 (2016)
- Bluetooth module tests(OK)
- AdafruitMotorShield (OK)
- Application Software on QT(WIP)
- Communication controller(WIP)

###V2.0 (2016 ?)
- Integration of accelerometer, compass and ultrasonic modules.
- Localise the robot precisely
- Obstacle avoidance
- "Target and go"
- ...

## Licence
This project and all its files are published under GPLv3.

