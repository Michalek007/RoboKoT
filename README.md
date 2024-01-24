# RoboKoT
Software for RoboKoT project.

## Project structure

### CORE
Embedded code for microcontroller FRDM-KL05Z (C).

Core of the project, contains all RoboKoT logic.
Functionalities:
- 4x motors control via H-Bridge
- 3x touch sensors handling (GPIO)
- displaying RoboKoT emotions on LCD display (I2C)
- petting RoboKoT via capacitive touch sensor (TSI)
- wagging RoboKoT tail and paw with 2x servos (PWM)
- playing cat alike sounds with speaker (PCM)
- reading data from esp (UART)

### API
Rest-api written in Python as Flask app.

Allows user to give commands to RoboKoT via basic website.

Functionalities:
- user authorisation
- handling esp requests
- handling user requests

### ESP

Embedded code for microcontroller ESP32 (C++).

Functionalities:
- connects to WiFi
- makes requests to api
- sends data to core microcontroller via UART

## Block diagram

![RoboKoT_block_diagram](https://github.com/Michalek007/RoboKoT/assets/101892382/d5fc1608-7e4d-43dd-bb56-9e43c4086868)

## Installation

First, clone this repository.

    $ git clone https://github.com/Michalek007/RoboKoT.git

Then read `README.md` of each component, which contains information
how to develop and deploy the project.

![being_a_cat](https://github.com/Michalek007/RoboKoT/assets/101892382/a9ee07dc-042d-4321-8ae7-671da54ff155)
