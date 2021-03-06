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
  Serial.print("INITIALIZED!\n");
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
    Serial.print("TEST START\n");

    CAN_FRAME frame;
    frame.id = 0x23;
    frame.length = 8;
    frame.data.low = 0xEFBEADDE;

    digitalWrite(pin_led, HIGH);
    delay(200);
    digitalWrite(pin_led, LOW);
    Serial.print("Testing Serial1 port...\n");
    if (Serial1.available() > 0) {
      Serial1.print("Received: ");
      Serial1.print(Serial1.readString());
      Serial1.print("\n");
    } else {
      Serial1.print("Waiting...\n");
    }
    delay(1000);

    digitalWrite(pin_led, HIGH);
    delay(200);
    digitalWrite(pin_led, LOW);
    Serial.print("Testing Can0 port...\n");
    frame.data.high = 0x00000000;
    Can0.sendFrame(frame);
    Can0.sendFrame(frame);
    Can0.sendFrame(frame);
    delay(1000);

    digitalWrite(pin_led, HIGH);
    delay(200);
    digitalWrite(pin_led, LOW);
    Serial.print("Testing Can1 port...\n");
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

    Serial.print("TEST END\n");    
  }
}
