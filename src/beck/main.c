#include <stdio.h>
#include <clib.h>

#include "../../lib/can/can.h"
#include "../../lib/i2c/i2c.h"
#include "../../lib/can-i2c.h"


void main() {
   unsigned int am=0x3FFF,ac=0x4000;
   can_event_msg_t msg;
   int test;
   unsigned char msg2=0x7F;
   unsigned char add;

   if(can_init(am, ac, CAN_BAUDRATE_1M) < 0) {
   	printf("Error while initializing\n");
      return;
   }

   i2c_init();

   BIOS_Set_Focus(FOCUS_BOTH);

   //while(1) {
      /*msg.id = 0x1000;
      msg.data[0] = i;
      msg.length = 1;

      if(can_recv(10, &msg) == 0) {
   		printf("Message received from 0x%04X\n", (msg.id >> 5));

	      msg.id = 0x1000;
	   	can_send(msg);
	      printf("Message sended with id = 0x%04X\n", (msg.id >> 5));
      }*/

      add=I2C_scan(0x00,0xff);

      test=i2c_write(0x70, &msg2 ,2) ;

      printf("test= %d\n",test);
   //}

   can_destroy();
   i2c_destroy();
}




//--------------------to be modified-------------------
/*# define acceptance_mask 0xFFFF
# define acceptance_code 0xFFFF

static can_i2c_trans_t translations[] = {
  {  }
};

static tab_length=0;
//_______________________________________________________




void huge test();

static unsigned int timerID;
static TimerProc_Structure timer={
	&timerID,
   test,
   NULL,
	('T','E','S','T'),
	10
};
static int flag_test=0;

int beck_init(){
	int ret=0,test;

   test=RTX_Install_Timer (&timer);
   if(test!=0){
    	ret=-1;
   }

   test=can_init(acceptance_mask, acceptance_code,CAN_BAUDRATE_1M);

   if(test!=0){
    	ret=-1;
   }

   i2c_init();

   return ret;

}

void huge test(){

	flag_test=1;

}

void beck_destroy(){

	can_destroy();
   i2c_destroy();

}





void main(){

	int test;
   can_event_msg_t msg_can, msg_can_old;


   if(beck_init() != 0) {
   	printf("Error while initializing\n");
      return;
   }

   test=RTX_Start_Timer ( timerID );

   if(test!=0){
   	printf("invalid timer ID\n");
   }

	while(1){
      printf("entering in while(1)\n");
      test=can_recv(1,&msg_can);

      if(test!=-2){
      test=update_values(msg_can,translations,tab_length);
      printf("update_values : test=%d\n",test);
      }

      if(flag_test==1){
      	printf("entering in flag_test == 1\n");
         flag_test=0;
         send_changes(translations,tab_length);
      }

   }



}


*/

