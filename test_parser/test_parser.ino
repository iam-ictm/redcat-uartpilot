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
UARTPilot::Msg_COD_t p_msg;

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
  while (Serial1.available()) {
    if (parser.parse(Serial1.read()) > 0) {
      parser.debugMessage();

      if (parser.isChecksumCorrect()) {
        Serial.print("Message: " + parser.getMessage() + "\n");

        if (parser.getMsgCOD(&p_msg)) {
          Serial.print("COD: ");
          Serial.print(p_msg.speed);
          Serial.print(" ");
          Serial.println(p_msg.steerangle);
          Serial.println();
        }
      }
    }
  }
}
