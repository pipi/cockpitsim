#include <stdio.h>
#include <windows.h>

#include "../../../../lib/can/can.h"

void main() {
  int ret, i;
  can_event_msg_t msg;
  
  ret = can_init(0xFFFF, 0xFFFF, CAN_BAUDRATE_1M);
  if(ret < 0) {
    printf("Error while opening the CAN port\n");
    return;
  }

  i = 0;
  while(1) {
    /* ID = 0100000000 (11 bits) */
    msg.id = 0x4000;
    msg.data[0] = (unsigned char) i;
    msg.length = 1;

    can_send(msg);
    printf("Message sended with id = 0x%04X\n", (msg.id >> 5));

    if(can_recv(0, &msg) == 0) {
      printf("Message received with id = 0x%04X\n", (msg.id >> 5));
    }
   
    Sleep(500);
  }

  can_destroy();
}
