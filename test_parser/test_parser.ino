/*
 * Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * Implementation for parsing the NMEA-0183 based data emitted by uartpilot.
 */

#include <stdlib.h>

// variables used for parsing
char data[83];                         // NMEA-0183 + NULL byte
int msg_max = sizeof(data) - 2;        // need two bytes for checksum
byte parity = 0;
int offset = -1;
int checksum_offset = -1;

// results of parsing
boolean checksum_correct = false;
String msg;

/**
 * Parses a message starting with $ and ending with *. The two characters
 * after * are interpreted as checksum (bitwise XOR of the message).
 * @param c the character to parse
 * @return length of the message between $ and * or -1 if not yet done
 */
int parse(char c) {
  switch (c)
  {
    // start of message
    case '$':
      parity = 0;
      offset = 0;
      checksum_offset = -1;
      break;

    // start of checksum
    case '*':
      if (offset >= 0 && offset < msg_max) checksum_offset = offset;
      break;

    // EOL
    case '\r': break;
    case '\n': break;

    // all data (including checksum)
    default:
      if (offset >= 0 && offset < msg_max) {
        // append data and checksum (two bytes)
        if (checksum_offset < 0 || offset < checksum_offset + 2) data[offset++] = c;

        // only calculate parity if not part of checksum
        if (checksum_offset < 0) parity ^= c;
      }

      // message complete, calculate checksum and return
      if (checksum_offset > 0 && offset >= checksum_offset + 2) {
        data[offset] = 0;
        offset = -1;
        msg = String(data).substring(0, checksum_offset);
        checksum_correct = parity == strtol(&data[checksum_offset], NULL, 16) ? true : false;
        return checksum_offset;
      }

      break;
  }
  return -1;
}

/**
 * Serial debug output of a received message
 * @param msg_len length of the message
 */
void debugMsg(int msg_len) {
  Serial.print("Got msg: '");
  Serial.print(msg);
  Serial.print("' (len: ");
  Serial.print(msg_len);
  Serial.print("), ");
  Serial.print("checksum: '");
  Serial.print(&data[msg_len]);
  Serial.print("' (calculated: ");
  Serial.print(parity, HEX);
  if (checksum_correct) Serial.print(", CORRECT)\n");
  else Serial.print(", WRONG)\n");
}

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
    msg_len = parse(Serial1.read());
  }

  if (msg_len > 0)
    debugMsg(msg_len);
}

