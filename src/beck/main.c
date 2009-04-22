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
#include <stdlib.h>
#include <clib.h>
#include <conio.h>


#include <can.h>
#include <i2c.h>
#include <can-i2c.h>

// Program constants
#define CAN_MASK	0xFFFF
#define CAN_ID		0xFFFF

// ---------------- I2C NODES DATA TRANSLATIONS TO CAN -------------
#define LENGTH_TAB_I2C 1
#define LENGTH_TAB_FPGA 1

i2c_can_trans_t translations_i2c[] = {
	{ 0x203f, { { 0x40, 2 } }, 1, { 0xff, 0xff }, 2 }
};

i2c_can_trans_t translations_fpga[] = {
   { 0x8020, { { 0xc8, 4 } }, 1, { 0x00, 0x00, 0x00, 0x00 }, 4 }
};

static unsigned long ALTITUDE;
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

	//update_values(msg, translations_i2c, LENGTH_TAB_I2C);
   //write data to the classic i2c components
   switch(msg.id) {

   	case 0x8000: // TODO
			ALTITUDE=((unsigned long)msg.data[0]<<24)&0xFF000000;
         ALTITUDE=(((unsigned long)msg.data[1]<<16)&0x00FF0000)|ALTITUDE;
         ALTITUDE=(((unsigned long)msg.data[2]<<8)&0x0000FF00)|ALTITUDE;
         ALTITUDE=(((unsigned long)msg.data[3])&0x000000FF)|ALTITUDE;
      	break;

      default :
     		break;
   }
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
	// Examples
   { 0x8000, forward_to_i2c },
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
            //printf("Message received and node found\n");
            break;
         }
      }
   }
}

//-------------------- END OF CAN MESSAGE HANDLERS ----------------

// -------------------------- ENTRY POINT -------------------------
void main() {
   unsigned char scan, ret;
   char plop[8];
   char plop2[8];
   char plop3[8];
   char plop4[8];
   char pouet[2];
   char ffxx[2]={0xff,0xff};
   int data0,data1,data;
   can_event_msg_t msg;

	if(can_init(CAN_MASK, CAN_ID, CAN_BAUDRATE_1M) == 0) {
    	printf("CAN BUS init OK\n");
   } else {
   	printf("CAN BUS init FAILED\n");
      exit(1);
   }

   if(i2c_init() == 0) {
    	printf("I2C BUS init OK\n");
   } else {
    	printf("I2C BUS init FAILED\n");
      exit(1);
   }

   running = 1;

   while(running){





   }

}
// ------------------------- END OF ENTRY POINT --------------------
