#include <stdio.h>

#include "../can.h"

void main() {
  int ret, i;
  can_event_msg_t msg;
  
  ret = can_init(0xFFFF, 0x0000, CAN_BAUDRATE_1M);
  if(ret < 0) {
    printf("Error while opening the CAN port\n");
    return;
  }

  while(1) {
    msg.id = 0x4000;
    msg.data[0] = (unsigned char) 4;
    msg.length = 1;

    //can_send(msg);
    
    if(can_recv(0, &msg) == 0) {
      printf("Message received from 0x%04X (data length = %d)\n",
        (unsigned short)(msg.id >> 5), (unsigned char)msg.length);
      if(msg.length == 1) {
        printf("Data match, content = %d (expect %d)\n", msg.data[0], i);
      }
    }
  }

  can_destroy();
}
