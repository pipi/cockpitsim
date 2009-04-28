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
#include <string.h>

#include <clib.h>

#include <can.h>
#include <i2c.h>
#include <can-i2c.h>

// Program constants
#define CAN_MASK	0xFFFF
#define CAN_ID		0xFFFF

#define DEBUG 		1

// ---------------- I2C NODES DATA TRANSLATIONS TO CAN -------------
#define CAN_FAMILY_LENGTH	1

int update_pcf_data(i2c_node_t* pNode) {
   static unsigned int lastData = 0, down = 0;
	unsigned int data;

   if(pNode == NULL) {
    	return -1;
   }

   data = 0xFFFF;
	if(i2c_write(pNode->nodeAddr, (char*) &data, 2) == 0) {
   	if(i2c_read(pNode->nodeAddr, (char*) &data, 2) == 0) {
         data = ~data;
         printf("[ DEBUG ] Data read from PCF = 0x%04X.\n", data);
         printf("[ DEBUG ] Last data of PCF = 0x%04X.\n",
         	lastData);
      	if(data != lastData) {
         	printf("[ DEBUG ] PCF data has changed !\n");
         	if(lastData == 0 && data == 1) { // rising edge
            	down = (down == 1 ? 0 : 1);
            }
            lastData = data;
         	memcpy(pNode->lastData, &down, sizeof(unsigned int));
            return 0;
         }
         printf("[ DEBUG ] Is down ? %s.\n", (down == 1 ? "true" : "false"));
      } else {
       	printf("[ DEBUG ] PCF read failed.\n");
      }
   } else {
    	printf("[ DEBUG ] PCF write failed.\n");
   }
   return -1;
}

int update_pcf_data_from_can(i2c_node_t* pNode, can_event_msg_t* pMsg) {
   if(pNode == NULL || pMsg == NULL) {
    	return -1;
   }

	memcpy(pNode->lastData, pMsg->data, pNode->dataLength);
   return 0;
}

int fill_buffer_from_pcf(i2c_node_t* pNode, unsigned char* buffer) {
   if(pNode == NULL || buffer == NULL) {
   	return -1;
   }

   memcpy(buffer, pNode->lastData, pNode->dataLength);
   return 0;
}

int update_enc_data(i2c_node_t* pNode) {
   static int lastPosition = 0;
   int position, hasChanged;

   if(pNode == NULL) {
    	return -1;
   }

   hasChanged = 0;
   if(i2c_write(pNode->nodeAddr, "\0" /* 0x00 */, 1) == 0) {
    	if(i2c_read(pNode->nodeAddr, (char*) &position, 2) == 0) {
       	if(position != lastPosition) {
          	*((int*) pNode->lastData) += (int) (3.4799 * (position - lastPosition));
            hasChanged = 1;
         }
         lastPosition = position;
         return (hasChanged ? 0 : -1);
      }
   }
   return -1;
}

int update_enc_data_from_can(i2c_node_t* pNode, can_event_msg_t* pMsg) {
 	long altitude;

   if(pNode == NULL || pMsg == NULL) {
    	return -1;
   }

   memcpy(&altitude, pMsg->data, sizeof(long));
   altitude = altitude >> 16;
   memcpy(pNode->lastData, &altitude, pNode->dataLength);
   return 0;
}

int fill_buffer_from_enc(i2c_node_t* pNode, unsigned char* buffer) {
	int length;
	long altitude;

 	if(pNode == NULL || buffer == NULL) {
    	return -1;
   }

   length = sizeof(long);
	memcpy(&altitude, pNode->lastData, pNode->dataLength);
   altitude = altitude << 16;
   memcpy(buffer, &altitude, length);
   return length;
}

static i2c_node_t altitudeNodes[] = {
	{
     0x40,
     { 0, 0, 0, 0 },
     4,
     update_pcf_data,
     update_pcf_data_from_can,
     fill_buffer_from_pcf
   },
   {
     0xC8,
     { 0, 0 },
     2,
     update_enc_data,
     update_enc_data_from_can,
     fill_buffer_from_enc
   }
};

static can_family_t canFamilies[] = {
	// two first bytes for the PCF and the four last for the altitude value.
	{ 0x8000, { 0, 0, 0, 0, 0, 0, 0, 0 }, 8, 0, altitudeNodes, 2 }
};

// ---------------- END OF I2C NODES DATA TRANSLATIONS ------------

// -----------  FUNCTION FOR SENDING ALTITUDE CHANGES -------------

void update_can_families() {
	int fam, nod, index;

   index = 0;
   for(fam = 0; fam < CAN_FAMILY_LENGTH; fam++) {
    	for(nod = 0; nod < canFamilies[fam].nodesLength; nod++) {
      	i2c_node_t* pNode = &canFamilies[fam].i2cNodes[nod];
         //printf("[ DEBUG ] Update I2C node with address 0x%04X.\n",
         //	pNode->nodeAddr);
	   	if(pNode->update_data_from_node(pNode) == 0) {
         	printf("[ DEBUG ] I2C node with address 0x%04X has changed.\n",
            	pNode->nodeAddr);	
          	pNode->fillBuffer(pNode, canFamilies[fam].leastData+index);
            canFamilies[fam].dataHasChanged = 1;
         }
         index += pNode->dataLength;
      }
      //printf("[ DEBUG ] CAN family with data 0x%04X 0x%04X.\n",
      //	*((unsigned int*) canFamilies[fam].leastData+2),
      // *((unsigned int*) canFamilies[fam].leastData));
   }
}

int send_data_to_can() {
   int fam, ret;
 	can_event_msg_t msg;

   ret = 0;
   for(fam = 0; fam < CAN_FAMILY_LENGTH; fam++) {
      if(canFamilies[fam].dataHasChanged) {
      	memset(&msg, 0, sizeof(can_event_msg_t));
  	      msg.id = canFamilies[fam].canId;
  	 	   msg.length = canFamilies[fam].dataLength;
      	memcpy(msg.data, canFamilies[fam].leastData, msg.length);

      	ret = can_send(msg) == 0 ? ret : -1;
      	//printf("[ DEBUG ] Message sent with id 0x%04X.\n", msg.id);

         canFamilies[fam].dataHasChanged = 0;
      }
   }

   return ret;
}

// ----------- END OF FUNCTION ALTITUDE CHANGES SENDING ------------

// ----------------------- TIMER CONFIGURATION----------------------

static unsigned int canMsgSendingTimerId;
static char flag_timer_50ms = 0;

void huge can_msg_sending_interrupt() {
   flag_timer_50ms = 1;
   RTX_Start_Timer(canMsgSendingTimerId);
}

static TimerProc_Structure canMsgSendingTimer = {
	&canMsgSendingTimerId,
   can_msg_sending_interrupt,
   NULL,
	{'S','E','N','D'},
	50 // 50 milliseconds
};

//--------------------- END OF TIMER CONFIGURATION -----------------

// ----------------------- CAN MESSAGE HANDLERS --------------------

void forward_to_i2c(can_event_msg_t msg) {

   switch(msg.id & 0xFFE0) {
   	case 0x8000: // update altitude value
			altitudeNodes[0].update_data_from_can(&altitudeNodes[0], &msg);
         altitudeNodes[1].update_data_from_can(&altitudeNodes[0], &msg);
    	  	break;
      default :
     		break;
   }
}

// Any other handler which match the following prototype
// void handler(can_event_msg_t);

// Handlers look up table
can_handler_t handlers[] = {
	// Examples
   { 0x8000, forward_to_i2c },
   { 0x0000, NULL }
};

#define CAN_ID_MATCH(id1, id2) (((id1)&0xFFE0) == ((id2)&0xFFE0))

void can_msg_lookup(void) {
	int i;
	can_event_msg_t canMsg;

   if(can_recv(1, &canMsg) == 0) {

   	// Dispatch the CAN message to the corresponding handler.
  	   // NOTE : Suppose there is only one handler per CAN id.
      printf("[ DEBUG ] Received CAN message (id = 0x%04X)\n", canMsg.id);
    	for(i = 0; handlers[i].handler != NULL; ++i) {
    		if(CAN_ID_MATCH(handlers[i].canId, canMsg.id)) {
          	handlers[i].handler(canMsg);
            break;
         }
      }
   }
}

//-------------------- END OF CAN MESSAGE HANDLERS ----------------

// -------------------------- ENTRY POINT -------------------------

void main() {

	// Initialization.
	if(can_init(CAN_MASK, CAN_ID, CAN_BAUDRATE_1M) == 0) {
    	printf("[ INFO ] CAN BUS init OK\n");
   } else {
   	printf("[ ERROR ] CAN BUS init FAILED\n");
      exit(1);
   }

   if(i2c_init() == 0) {
    	printf("[ INFO ] I2C BUS init OK\n");
   } else {
    	printf("[ ERROR ] I2C BUS init FAILED\n");
      exit(1);
   }

   // Timer installation and starting.
   if(RTX_Install_Timer(&canMsgSendingTimer) == 0) {
   	printf("[ INFO ] CAN message sending timer installed.\n");
   } else {
		printf("[ WARNING ] CAN message sending timer not installed.\n");
   }

   if(RTX_Start_Timer(canMsgSendingTimerId) == 0) {
   	printf("[ INFO ] CAN message sending timer started.\n");
   } else {
		printf("[ WARNING ] CAN message sending timer removed.\n");
      RTX_Remove_Timer(canMsgSendingTimerId);
   }

   while(1) {
     can_msg_lookup();
     update_can_families();
     if(flag_timer_50ms == 1) {
     		flag_timer_50ms = 0;
			send_data_to_can();
     }
   }

   i2c_destroy();
   can_destroy();

}
// ------------------------- END OF ENTRY POINT --------------------
