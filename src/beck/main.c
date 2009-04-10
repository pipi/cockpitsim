/*
 * Simulator project
 *
 * Institut de Formation des Ingénieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : Beck main program
 *
 * Authors : Julien Peeters <julien.peeters@u-psud.fr>
 *			    Fabien Provost <fabien.provost@u-psud.fr>
 *			    Feng Xiong <feng.xiong@u-psud.fr>
 *           Yongchao Xu <yongchao.xu@u-psud.fr>
 *
 * Creation date : 06/04/2009
 */

#include <stdio.h>
#include <clib.h>

#include <can.h>
#include <i2c.h>
#include <can-i2c.h>

// Program constants
#define CAN_MASK	0xFFFF
#define CAN_ID		0xFFFF
#define LENGTH_TAB_I2C 10
#define LENGTH_TAB_FPGA 10

// ---------------- I2C NODES DATA TRANSLATIONS TO CAN -------------
i2c_can_trans_t translations_i2c[] = {
	{ 0x00, { 0 }, 0, 0xFFFF }
};

i2c_can_trans_t translations_fpga[] = {
   { 0x00, { 0 }, 0, 0xFFFF }
};
// ---------------- END OF I2C NODES DATA TRANSLATIONS ------------

// ----------------------- TIMER CONFIGURATION----------------------
static unsigned int timerID;
static int flag_timer=0;

void huge timer_interrupt() {
	flag_timer = 1;
   RTX_Start_Timer(timerID);
}

static TimerProc_Structure timer={
	&timerID,
   timer_interrupt,
   NULL,
	{'T','I','M','R'},
	10
};

//--------------------- END OF TIMER CONFIGURATION -----------------

// ----------------------- CAN MESSAGE HANDLERS --------------------
void forward_to_ax12(can_event_msg_t msg) {
	// TODO
   // Implement the AX-12 action to the CAN message.
}

void forward_to_i2c(can_event_msg_t msg) {
 	// TODO
   // Implement the I2C action the CAN message

	update_values(msg, translations_i2c,LENGTH_TAB_I2C);
   //write data to the classic i2c components

}

// Any other handler which match the following prototype
// void handler(can_event_msg_t);

static unsigned char running;

void node_manager(can_event_msg_t msg) {
	switch(msg.id) {
   	case 0x0001: running = 0; break;
	}
}

// Handlers look up table
can_handler_t handlers[] = {
	{ 0x4000, forward_to_ax12 },
   { 0x5000, forward_to_i2c },
   { 0x0001, node_manager },
   { 0x0000, NULL }
};

void can_msg_lookup(void) {
	short i;
	can_event_msg_t canMsg;

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
}

//-------------------- END OF CAN MESSAGE HANDLERS ----------------

// -------------------------- ENTRY POINT -------------------------
void main() {
	can_init(CAN_MASK, CAN_ID, CAN_BAUDRATE_1M);
   i2c_init();

   running = 1;

   if(RTX_Start_Timer(timerID) != 0){
   	printf("Error whil initiazing the time : invalid timer ID\n");
   }

   while(running) {
      can_msg_lookup();

      /*if(flag_timer == 1){
      	i2c_send_changes(translations_i2c, LENGTH_TAB_I2C);
      	fpga_send_changes(translations_fpga, LENGTH_TAB_FPGA);
         flag_timer = 0;
      }*/

      // Other treatments...
   }

   i2c_destroy();
   can_destroy();
}
// ------------------------- END OF ENTRY POINT --------------------
