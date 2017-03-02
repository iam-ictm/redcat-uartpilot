/*
 * Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * Test for verifying transmissions, count the number of correctly received messages.
 */

#include <UARTPilot.h>

UARTPilot::Parser parser;
UARTPilot::Msg_COD_t p_msg;

const int pin_btn = 2;

int count = 0;

/**
 * Print the counter to Serial and reset it.
 */
void reset() {
  Serial.print("Count: ");
  Serial.print(count, DEC);
  Serial.println();
  count = 0;
}

/**
 * Standard arduino setup(), initialize things.
 */
void setup() {
  pinMode(pin_btn, INPUT_PULLUP);

  Serial.begin(115200);
  Serial1.begin(115200);

  reset();
}

/**
 * Standard arduino main loop()
 */
void loop() {
  if (digitalRead(pin_btn) == LOW) {
    reset();
    delay(1000);
  }

  while (Serial1.available()) {
    if (parser.parse(Serial1.read()) > 0 && parser.isChecksumCorrect()) {
      parser.getMsgCOD(p_msg);
      count++;
    }
  }
}
