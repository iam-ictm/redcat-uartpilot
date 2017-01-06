/*
 * Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * Implementation for parsing the NMEA-0183 based data emitted by uartpilot.
 */

#include "UARTPilot.h";

namespace UARTPilot {

  Parser::Parser() :
  parity(0),
  offset(-1),
  checksum_offset(-1) { }

  /**
   * Parses a message starting with $ and ending with *. The two characters
   * after * are interpreted as checksum (bitwise XOR of the message).
   * @param c the character to parse
   * @return length of the message between $ and * or -1 if not yet done
   */
  int Parser::parse(char c) {
    switch (c) {
      case '$':
        // start of message
        parity = 0;
        offset = 0;
        checksum_offset = -1;
        break;

      case '*':
        // start of checksum
        if (offset >= 0 && offset < MSG_MAX_LEN) checksum_offset = offset;
        break;

      case '\r': break;
      case '\n': break;

      default:
        // all data (including checksum)
        if (offset >= 0 && offset < MSG_MAX_LEN) {
          // append data and checksum (two bytes)
          if (checksum_offset < 0 || offset < checksum_offset + 2) data[offset++] = c;

          // only calculate parity if not part of checksum
          if (checksum_offset < 0) parity ^= c;
        }

        // message complete, calculate checksum and return
        if (checksum_offset > 0 && offset == checksum_offset + 2) {
          data[offset] = 0;
          offset = -1;
          message = String(data).substring(0, checksum_offset);
          checksum_correct = parity == strtol(&data[checksum_offset], NULL, 16) ? true : false;
          return checksum_offset;
        }

        break;
    }
    return -1;
  }

  /**
   * Serial debug output of a received message
   */
  void Parser::debugMessage() {
    Serial.print("Got message: '");
    Serial.print(message);
    Serial.print("' (len: ");
    Serial.print(checksum_offset);
    Serial.print("), ");
    Serial.print("checksum: '");
    Serial.print(&data[checksum_offset]);
    Serial.print("' (calculated: ");
    Serial.print(parity, HEX);
    if (checksum_correct) Serial.print(", CORRECT)\n");
    else Serial.print(", WRONG)\n");
  }
};