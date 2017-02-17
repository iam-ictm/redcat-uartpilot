# Description

This repository contains Arduino code for the UARTPilot functionality added to the PX4 firmware in [redcat-px4fw](https://github.com/iam-ictm/redcat-px4fw),
as well as some tests and helpers.

# Contents

* UARTPilot - Arduino library for interfacing with UARTPilot in PX4
* test_parser - Application for testing the parser provided with UARTPilot
* test_hardware - Application for testing the custom, Arduino Due based board
* count_messages - Application for testing, which counts the number of received and valid messages
* uart-shell.py - Simple commandline interface for sending UARTPilot commands over a serial connection
