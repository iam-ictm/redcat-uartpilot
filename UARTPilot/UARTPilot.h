/*
 * Copyright (C) 2017 BFH - Bern University of Applied Sciences, http://bfh.ch
 * Author: Pascal Mainini <pascal.mainini@bfh.ch>
 *
 * Licensed under MIT license, see included file LICENSE or
 * http://opensource.org/licenses/MIT
 *
 * Implementation for parsing the NMEA-0183 based data emitted by uartpilot.
 */

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#define MSG_MAX_LEN 80

namespace UARTPilot {

  typedef struct Msg_COD_t {
    int speed;
    int steerangle;

    Msg_COD_t() :
    speed(0),
    steerangle(0) { }
  };

  class Parser {
  public:
    Parser();
    int parse(char c);
    void debugMessage();
    boolean getMsgCOD(Msg_COD_t &msg);

    inline boolean isChecksumCorrect() {
#ifdef DISABLE_CHECKSUM
      return true;
#else
      return checksum_correct;
#endif
    }

    inline String getMessage() {
      return message;
    }

  private:
    char data[MSG_MAX_LEN + 3]; // NMEA-0183 + checksum + NULL byte
    byte parity;
    int offset;
    int checksum_offset;
    boolean checksum_correct;
    String message;
  };
};

