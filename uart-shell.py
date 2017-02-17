#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# uart-shell.py
# Minimalistic shell for interacting with UARTPilot
#
# Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
# Author: Pascal Mainini <pascal.mainini@bfh.ch>
#
# Licensed under MIT license, see included file LICENSE or
# http://opensource.org/licenses/MIT
#

import argparse, cmd, serial
from random import randrange
from timeit import default_timer as timer

ENCODING = 'ascii'

class UARTShell(cmd.Cmd):
    intro = 'Type help or ? to list commands.\n'

    def __init__(self, serialparams):
        self.prompt = 'C>'
        self.calculate_checksum = True
        self.checksum = ''
        self.raw = False

        self.serial = serial.Serial(*serialparams)
        print('Serial port opened: ' + self.serial.name + ' ' + str(serialparams))

        super().__init__()
        
    def __send(self, data):
        if self.raw:
            message = bytes(data, ENCODING)
            self.serial.write(message)
        else:
            checksum = self.__checksum(data)
            message = bytes('$' + data + '*' + checksum, ENCODING)
            self.serial.write(message)
        print('Sent ' + str(message))

    def __checksum(self, data):
        if self.calculate_checksum:
            checksum = 0
            for b in bytes(data, ENCODING):
                checksum ^= b
            return format(checksum, '02x')
        else:
            return format(self.checksum, '02x')

    def do_checksum(self, arg):
        'If no argument is given, automatically calculate checksums; otherwise, use given argument as checksum.'
        if len(arg) == 0:
            self.calculate_checksum = True
            self.checksum = ''
            print('Automatically calculating checksums')
        else:
            self.calculate_checksum = False
            self.checksum = int(arg, 16)
            print('Checksum set to ' + format(self.checksum, '02x'))
    
    def do_raw(self, arg):
        'Switches RAW mode. If enabled, any input is sent without processing to UARTPilot'
        if self.raw:
            self.raw = False
            self.prompt = 'C>'
            print('RAW mode disabled')
        else:
            self.raw = True
            self.prompt = 'R>'
            print('RAW mode enabled')

    def do_exit(self, arg):
        'Exits the shell.'
        self.serial.close()
        return True

    def do_cod(self, arg):
        'Send controller output data. Requires speed and angle as parameters with values from -128 to 127.'
        speed, angle = arg.split()
        self.__send('AGCOD,' + hex(int(speed) & 0xff)[2:] + ',' + hex(int(angle) & 0xff)[2:])

    def do_rcod(self, arg):
        'Send random controller output data. Optionally specify the amount of messages to send.'
        if len(arg) == 0:
            count = 1
        else:
            count = int(arg)

        start = timer()
        for i in range(count):
            self.do_cod(str(randrange(-128, 127, 1)) + ' ' + str(randrange(-128, 127, 1)))

        print('Sending took ' + str(1000*(timer() - start)) + ' ms.')

    def default(self, arg):
        if arg == 'EOF':
            return self.do_exit(self)
        else:
            self.__send(arg)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Shell for UARTPilot')
    parser.add_argument('-b','--baudrate', default=115200, type=int, help='Baudrate to configure the serial interface')
    parser.add_argument('-d','--device', default='/dev/ttyUSB0', help='Serial interface device')
    args = parser.parse_args()

    UARTShell((args.device, args.baudrate)).cmdloop()
