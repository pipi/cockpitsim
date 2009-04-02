#include <stdio.h>
#include <clib.h>
#include "../can.h"

void main(){
   unsigned int am=0x3FFF,ac=0x4000;
   can_event_msg_t msg;

   if(can_init(am, ac, CAN_BAUDRATE_1M) < 0) {
   	printf("Error while initializing\n");
      return;
   }

   BIOS_Set_Focus(FOCUS_BOTH);

   while(1) {
      /*msg.id = 0x1000;
      msg.data[0] = i;
      msg.length = 1;*/

      if(can_recv(10, &msg) == 0) {
   		printf("Message received from 0x%04X\n", (msg.id >> 5));

	      msg.id = 0x1000;
	   	can_send(msg);
	      printf("Message sended with id = 0x%04X\n", (msg.id >> 5));
      }
   }

   can_destroy();
}