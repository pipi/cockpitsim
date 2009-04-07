#include <stdio.h>
#include <clib.h>

#include <can/can.h>
#include <i2c/i2c.h>
#include <can-i2c.h>

#define CAN_MASK	0xFFFF
#define CAN_ID		0xFFFF

//i2c_can_trans_t translations[] = {
//};

void main() {
   can_event_msg_t canMsg;

	can_init(CAN_MASK, CAN_ID, CAN_BAUDRATE_1M);
   i2c_init();

   while(1) {
		if(can_recv(1, &canMsg) == 0) {
      	// handle CAN msg
      }
   }

   i2c_init();
   can_destroy();
}




