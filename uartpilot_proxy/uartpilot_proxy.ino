/*
 * Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * Arduino application which proxies/modifies CAN messages based on commands 
 * received on the serial interface.
 */

#include <UARTPilot.h>

UARTPilot::Parser parser;
UARTPilot::Msg_COD_t* p_msg;

/**
 * Standard arduino setup(), initialize things
 */
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  p_msg = (UARTPilot::Msg_COD_t*) malloc(sizeof(p_msg));
}

/**
 * Standard arduino main loop()
 */
void loop() {
  int msg_len = -1;

  while (Serial1.available()) {
    msg_len = parser.parse(Serial1.read());
  }

  if (msg_len > 0 && parser.isChecksumCorrect() && parser.getMsgCOD(p_msg)) {
    Serial.print(p_msg->speed);
    Serial.println(p_msg->steerangle);
  }
}

