#include <stdio.h>

#include "../can.h"

void main() {
  int ret;
  can_event_msg_t msg;
  
  ret = can_init(0x3FFF, 0x4000, CAN_BAUDRATE_1M);
  if(ret < 0) {
    printf("Error while opening the CAN port\n");
  }

  while(1) {
    msg.id = 0x8000;
    msg.data[0] = 0x21;
    msg.length = 1;

    can_send(msg);
    
    if(can_recv(0, &msg) == 0) {
      printf("Message received from 0x%04X\n", msg.id);
    }
  }

  can_destroy();
}
