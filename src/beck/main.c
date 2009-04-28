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
#include <math.h>

// Program constants
#define CAN_MASK	0xFFFF
#define CAN_ID		0xFFFF

// ---------------- I2C NODES DATA TRANSLATIONS TO CAN -------------
#define LENGTH_TAB_I2C 1
#define LENGTH_TAB_FPGA 1


//truct for classic I2C component (PCF)
i2c_can_trans_t translations_i2c[] = {
	{ 0x203f, { { 0x40, 2 } }, 1, { 0xff, 0xff }, 2 }
};

//truct for fpga
i2c_can_trans_t translations_fpga[] = {
   { 0x8020, { { 0xc8, 2 } }, 1, { 0x00, 0x00}, 2 }
};

int TAB[16][16];

// ---------------- END OF I2C NODES DATA TRANSLATIONS ------------


// -----------  FUNCTION FOR SENDING ALTITUDE CHANGES -------------
//default value of altitude
static long ALTITUDE=0;

int altitude_changes_lookup(i2c_can_trans_t* trans, long* nextAlt) {
	short i;
   char dataBuf[8];
   char plop=0x00;
   short test;
   unsigned char hasChanged;
   unsigned int oldPosition;
   unsigned int newPosition;
   int difference;

   hasChanged=0;

   i2c_write(trans->i2cNodes[0].nodeAddr, &plop, 1);
   test=i2c_read(trans->i2cNodes[0].nodeAddr, dataBuf, trans->i2cNodes[0].dataLength);
   if(test != 0) {
   // If there is no new value, copy the old one.
   	memcpy(dataBuf, trans->oldData,
      trans->i2cNodes[0].dataLength);
   }

   hasChanged = memcmp(dataBuf, trans->oldData, trans->dataLength);

   if(hasChanged!=0){
     //	printf("has changed = %d \nread new value=  0x%4x\n",hasChanged,*(unsigned int*)dataBuf);
      oldPosition=((unsigned int)(trans->oldData[1])&0x00FF)|(((unsigned int)(trans->oldData[0])<<8)&0xFF00);
      newPosition=((unsigned int)(dataBuf[1])&0x00FF)|(((unsigned int)(dataBuf[0])<<8)&0xFF00);

      difference=newPosition-oldPosition;
      *nextAlt=*nextAlt+((long)(30.4799*difference));
      if(*nextAlt<0) *nextAlt=0;
      printf("\naltitude=%lu\n", (*nextAlt)*3.28084);
   }

	memcpy(trans->oldData, dataBuf, trans->dataLength);
   return (hasChanged != 0 ? 0 : -1);
}


int altitude_send_changes(i2c_can_trans_t* trans, long* nextAlt) {
   can_event_msg_t msg;

	if(altitude_changes_lookup(trans, nextAlt) == 0) {
		memset(&msg, 0, sizeof(msg));
      msg.id = trans->canId;
      *((long *)msg.data)=*nextAlt<<16;
      msg.length = 4;
      return can_send(msg);
   }

   return 0;
}

// ----------- END OF FUNCTION ALTITUDE CHANGES SENDING ------------

// ----------------------- TIMER CONFIGURATION----------------------
static unsigned int timerID;
static int flag_timer=0;
static int flag_timer_50ms=0;

void huge timer_interrupt() {
	flag_timer++;
   if(flag_timer==50){
   flag_timer_50ms=1;
   flag_timer=0;
   }
   RTX_Start_Timer(timerID);
}

static TimerProc_Structure timer={
	&timerID,
   timer_interrupt,
   NULL,
	{'T','I','M','R'},
	1
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

   	case 0x801f: // update altitude value
             ALTITUDE = *((long*) msg.data)>>16;
/*			ALTITUDE=((long)msg.data[0]<<24)&0xFF000000;
         ALTITUDE=(((long)msg.data[1])<<16&0x00FF0000)|ALTITUDE;
         ALTITUDE=(((long)msg.data[2]<<8)&0x0000FF00)|ALTITUDE;
         ALTITUDE=(((long)msg.data[3])&0x000000FF)|ALTITUDE;*/
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
   { 0x801f, forward_to_i2c },
   //{ 0x0001, node_manager },
   { 0x0000, NULL }
};

void can_msg_lookup(void) {
	short i;
	can_event_msg_t canMsg;

   if(can_recv(1, &canMsg) == 0) {

   	// Dispatch the CAN message to the corresponding handler.
  	   // NOTE : Suppose there is only one handler per CAN id.
      printf("msgId= 0x%04x",canMsg.id);
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

char buffer[8] = {4,0,0,0,0,0,0,0};
void main() {
   unsigned char scan, ret;

   char plop[8];
   char plop2[8];
   char plop3[8];
   char plop4[8];
   char pouet[2];
   char ffxx[2]={0xff,0xff};
   int data0,data1,data,test;
   can_event_msg_t msg;
   int i,j,k;
   unsigned char buffer[5];

   k=1;
   for(i=0;i<16;i++){
		for(j=0;j<16;j++){
      	TAB[i][j]=k;
         k++;
      }
   }

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
   RTX_Install_Timer(&timer);
   RTX_Start_Timer(timerID);

   while(1){

  /* buffer[0]=0x00;
   buffer[1]=0xFF;
   buffer[2]=0xEF;
   buffer[3]=0xFF;
   buffer[4]=0xFF;
   i2c_write(0xC8,buffer,5);
   //buffer[0]=0x00;
   test=i2c_write(0xC8, buffer,1);
   getchar();
   test=i2c_read(0xc9,buffer,2);
   printf("  all 1---  ligne=  0x%04x",*(unsigned int*)buffer);

   buffer[0]=0x02;
   test=i2c_write(0xC8, buffer,1);
   getchar();
   test=i2c_read(0xc9,buffer,2);
   printf("  all 1---  collone=  0x%04x\n\n\n",*(unsigned int*)buffer);
       getchar();
    buffer[0]=0x00;
   buffer[1]=0x00;
   buffer[2]=0x00;
   buffer[3]=0x00;
   buffer[4]=0x00;
   i2c_write(0xC8,buffer,5);
   //buffer[0]=0x00;
   test=i2c_write(0xC8, buffer,1);
       getchar() ;
   test=i2c_read(0xc9,buffer,2);
   printf(" all 0 ---  ligne=  0x%04x",*(unsigned int*)buffer);

   buffer[0]=0x02;        
   test=i2c_write(0xC8, buffer,1);
   getchar();
   test=i2c_read(0xc9,buffer,2);
   getchar();
   printf("  all 0---  collone=  0x%04x\n",*(unsigned int*)buffer); */

       test=keyboard_decode(0xC9,TAB);
    //   if(test!=0){
        //	printf("\ntab=%d\n",test);
      // }
   }


    while(0){
        i2c_write(0xc8,plop,1);
    	  i2c_read(0xc9, plop2,2);
        //i2c_read(0xc9, plop3,2);

        printf("read new value= 0x%04x\n", *(unsigned int*) plop2);
    }

    while(1){

    	test=keyboard_decode(0xC8, TAB_Keyboard);
    	if(test!=0){
    		printf("touch detect = %d\n",test);
    	}


    }

     /*  I2C_receive_char(0xc9, &plop2[0], 1);
       I2C_receive_char(0xc9, &plop2[1], 1);
       I2C_receive_char(0xc9, &plop2[2], 1);
       I2C_receive_char(0xc9, &plop2[3], 0);
       printf("read new value= 0x%04x  0x%04x\n", *((unsigned int*)(plop2)), *((unsigned int*)(plop2+2)));
     */
      //sleep(1);
   //}


   i2c_destroy();
   can_destroy();

}
// ------------------------- END OF ENTRY POINT --------------------
