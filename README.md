# MobileRobot
A personal project based on a 4WD MobileRobot.

##Goal
The goal of this project is to develop a remote control car in a first time.
After, the robot can be autonomous and transmit informations to the PC if there is a bluetooth connection available.

## Softwares
- QT 5.4.1 (latest version) + QT Creator 3.4.0
- Atmel Studio 6.1 SP2 (6.1.2730)

## Hardware
- 4WD Mobile Robot
- ATMega 1284P-PU
- Bluetooth module JY-MCU v1.06
- Adafruit Motor Shield V2.0 (PCA9685PW chip for DC motor control)

## Installation Procedure
There is not yet a .exe to execute directly to your PC.
So to have access and launch successfully the software, you must :
* Download the sofwtares
* Get the hardware
* Clone this repository to your desktop
* Load the .pro file located in /MobileRobot_Software and compile it (no extra libraries needed)
* Load the .atsln located in /MobileRobot_Embedded and upload in your µc

## Development roadmap
###V1.0 (31/07/15)
- Application Software on QT(WIP)
- Bluetooth module (OK)
- AdafruitMotorShield (OK)
- Reliability for transmission and ACK using regular expressions (WIP) 

###V2.0 (09/15 ?)
- Integration of accelerometer, compass and ultrasonic modules.
- Localise the robot precisely
- Obstacle avoidance
- "Target and go"
- ...

## Licence
This project and all its files are published under GPLv3.
This is true even if there is not yet informations in the .cpp or .h files.
