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
#include <due_can.h>

UARTPilot::Parser parser;
UARTPilot::Msg_COD_t* p_cod;

/**
 * Map last values received from UARTPilot to the CAN frame.
 */
inline void patchFrame(CAN_FRAME *frame) {
  switch (frame->id) {
    case 0x282:
      frame->data.bytes[1] = p_cod->steerangle != 0 ? 0xC0 : 0;
      break;
    case 0x382:
      frame->data.bytes[3] = p_cod->steerangle;
      break;
    case 0x384:
      frame->data.bytes[3] = p_cod->speed;
      break;
  }
}

/**
 * Standard arduino setup(), initialize things
 */
void setup() {
  Serial1.begin(115200);
  Can0.begin(CAN_BPS_250K);
  Can1.begin(CAN_BPS_250K);

  p_cod = (UARTPilot::Msg_COD_t*) malloc(sizeof(p_cod));
  memset(p_cod, 0, sizeof(p_cod));
}

/**
 * Standard arduino main loop()
 */
void loop() {
  int msg_len = -1;
  CAN_FRAME frame;

  while (Serial1.available()) {
    msg_len = parser.parse(Serial1.read());
  }

  if (msg_len > 0 && parser.isChecksumCorrect()) parser.getMsgCOD(p_cod);

  if (Can0.rx_avail()) {
    Can0.get_rx_buff(frame);
    patchFrame(&frame);
    Can1.sendFrame(frame);
  }

  if (Can1.rx_avail()) {
    Can1.get_rx_buff(frame);
    patchFrame(&frame);
    Can0.sendFrame(frame);
  }
}

