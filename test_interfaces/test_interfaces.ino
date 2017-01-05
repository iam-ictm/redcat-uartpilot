/*
 * Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * A simple test program to check if all required interfaces behave correctly.
 */
#include <due_can.h>

const int pin_btn = 2;
const int pin_led = 3;

boolean start = false;

void setup() {
  pinMode(pin_btn, INPUT_PULLUP);
  pinMode(pin_led, OUTPUT);

  Serial.begin(115200);
  Serial1.begin(115200);

  Can0.begin(CAN_BPS_250K);
  Can1.begin(CAN_BPS_250K);

  digitalWrite(pin_led, HIGH);
  delay(2000);
  digitalWrite(pin_led, LOW);
  Serial.write("Initialized!\n\r");
}

void loop() {
  if (digitalRead(pin_btn) == LOW) {
    start = true;
    digitalWrite(pin_led, HIGH);
    delay(500);
    digitalWrite(pin_led, LOW);
    delay(200);
    digitalWrite(pin_led, HIGH);
    delay(500);
    digitalWrite(pin_led, LOW);
    delay(1000);
  }

  if (start) {
    CAN_FRAME frame;
    frame.id = 0x23;
    frame.length = 8;
    frame.data.low = 0xEFBEADDE;

    digitalWrite(pin_led, HIGH);
    delay(200);
    digitalWrite(pin_led, LOW);
    Serial.write("Testing Serial1 port...\n\r");
    if (Serial1.available() > 0) {
      Serial1.write("Received: ");
      Serial1.print(Serial1.readString());
      Serial1.write("\n\r");
    } else {
      Serial1.write("Waiting...\n\r");
    }
    delay(1000);

    digitalWrite(pin_led, HIGH);
    delay(200);
    digitalWrite(pin_led, LOW);
    Serial.write("Testing Can0 port...\n\r");
    frame.data.high = 0x00000000;
    Can0.sendFrame(frame);
    Can0.sendFrame(frame);
    Can0.sendFrame(frame);
    delay(1000);

    digitalWrite(pin_led, HIGH);
    delay(200);
    digitalWrite(pin_led, LOW);
    Serial.write("Testing Can1 port...\n\r");
    frame.data.high = 0x11111111;
    Can1.sendFrame(frame);
    Can1.sendFrame(frame);
    Can1.sendFrame(frame);
    delay(1000);

    digitalWrite(pin_led, HIGH);
    delay(500);
    digitalWrite(pin_led, LOW);
    delay(200);
    digitalWrite(pin_led, HIGH);
    delay(500);
    digitalWrite(pin_led, LOW);
    start = false;
  }
}
