# Description

This repository contains Arduino code for the UARTPilot functionality added to the PX4 firmware in [redcat-px4fw](https://github.com/iam-ictm/redcat-px4fw),
as well as some tests and helpers.

# Contents

* *UARTPilot* - Arduino library for interfacing with UARTPilot in PX4
* *uartpilot_proxy* - Arduino application which proxies/modifies CAN messages based on commands received on the serial interface

* *test_parser* - Arduino application for testing the parser provided with UARTPilot
* *test_hardware* - Arduino application for testing the custom, Arduino Due based board
* *count_messages* - Arduino application for testing, which counts the number of received and valid messages

* *uart-shell.py* - Simple commandline interface for sending UARTPilot commands over a serial connection

# Revision History

## v1.0.0 (2017-02-23)

* First successfull integration of the whole chan from QGroundcontrol to Hymog
