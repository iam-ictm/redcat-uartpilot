/*
 * Copyright (C) 2016 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * A simple test program to check if all required interfaces behave correctly.
 */
#include <due_can.h>

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);

    Can0.begin(CAN_BPS_250K);
    Can1.begin(CAN_BPS_250K);
}

void loop() {
    CAN_FRAME frame;
    frame.id = 0x23;
    frame.length = 8;
    frame.data.low = 0xEFBEADDE;

    Serial.write("Testing Serial1 port...\n\r");
    if(Serial1.available() > 0) {
        Serial1.write("Received: ");
        Serial1.write(Serial1.read());
        Serial1.write("\n\r");
    } else {
        Serial1.write("Waiting...\n\r");
    }
    delay(1000);

    Serial.write("Testing Can0 port...\n\r");
    frame.data.high = 0x00000000;
    Can0.sendFrame(frame);
    delay(1000);

    Serial.write("Testing Can1 port...\n\r");
    frame.data.high = 0x11111111;
    Can1.sendFrame(frame);
    delay(1000);
}
