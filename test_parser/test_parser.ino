/*
 * Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * Implementation for parsing the NMEA-0183 based data emitted by uartpilot.
 */

#include <UARTPilot.h>

UARTPilot::Parser parser;

/**
 * Standard arduino setup(), initialize things
 */
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

/**
 * Standard arduino main loop()
 */
void loop() {
  int msg_len = -1;

  while (Serial1.available()) {
    msg_len = parser.parse(Serial1.read());
  }

  if (msg_len > 0)
    parser.debugMessage();
}

