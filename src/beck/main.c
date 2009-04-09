#include <stdio.h>
#include <clib.h>

#include <can.h>
#include <i2c.h>
#include <can-i2c.h>

#define CAN_MASK	0xFFFF
#define CAN_ID		0xFFFF

//i2c_can_trans_t translations[] = {
//};

void forward_to_ax12(can_event_msg_t msg) {
	// TODO
   // Implement the AX-12 action to the CAN message.
}

void forward_to_i2c(can_event_msg_t msg) {
 	// TODO
   // Implement the I2C action the CAN message
}

// Any other handler which match the following prototype
// void handler(can_event_msg_t);

static unsigned char running = 1;

void node_manager(can_event_msg_t msg) {
	switch(msg.id) {
   	case 0x0001: running = 0; break;
	}
}

can_handler_t handlers[] = {
	{ 0x4000, forward_to_ax12 },
   { 0x5000, forward_to_i2c },
   { 0x0001, node_manager },
   { 0x0000, NULL }
};

void main() {
	short i;
   can_event_msg_t canMsg;

	can_init(CAN_MASK, CAN_ID, CAN_BAUDRATE_1M);
   i2c_init();

   while(1) {
		if(can_recv(1, &canMsg) == 0) {
      	// Dispatch the CAN message to the corresponding handler.
         // NOTE : Suppose there is only one handler per CAN id.
      	for(i = 0; handlers[i].handler != NULL; ++i) {
      		if(handlers[i].canId == canMsg.id) {
            	handlers[i].handler(canMsg);
               break;
            }
         }
      }

      // Other treatments...
   }

   i2c_destroy();
   can_destroy();
}




