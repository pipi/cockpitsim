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
   { 0x2020, { { 0xc8, 4 } }, 1, { 0x00, 0x00, 0x00, 0x00 }, 4 }
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

	update_values(msg, translations_i2c, LENGTH_TAB_I2C);
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
	// Examples
	{ 0x4000, forward_to_ax12 },
   { 0x203f, forward_to_i2c },
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

   //while(running){






     /* plop[0]=0x00;

   	printf("Write FPGA = %d\n", i2c_write(0xC8, plop, 1));

      printf("Read Cnt 1 = %d\n", i2c_read(0xC9, plop, 4));
      data0 =  *(unsigned int*)(plop);
      data1 = *(unsigned int*)(plop+2);
      //data = ((data << 8) & 0xFF00) | ((data >> 8) & 0x00FF);
      printf(" 0x%04X    0x%04X\n", data0,data1);
      //printf(" 0x%04X    \n", data0);
      sleep(1);
      //plop[0]=0x06;


      plop2[0]=0x02;
      printf("adress: 0x%2x", I2C_scan(0x00,0xff));
      printf("Write FPGA = %d\n", i2c_write(0xC8, plop2, 1));
      printf("Read Cnt 2 = %d\n", i2c_read(0xC9, plop, 2));
      data = *(unsigned int*)plop;
      data = ((data << 8) & 0xFF00) | ((data >> 8) & 0x00FF);
      printf("Value 2= 0x%04X\n", data);
      sleep(2);    */
      /*
      plop3[0]=0x05;
   	printf("Write FPGA = %d\n", i2c_write(0xC8, plop3, 1));
      printf("Read Cnt 1 = %d\n", i2c_read(0xC9, plop, 2));
      data = *(unsigned int*)plop;
      data = ((data << 8) & 0xFF00) | ((data >> 8) & 0x00FF);
      printf("Value 1= 0x%04X\n", data);
      sleep(2);

      plop4[0]=0x06;
   	printf("Write FPGA = %d\n", i2c_write(0xC8, plop4, 1));
      printf("Read Cnt 1 = %d\n", i2c_read(0xC9, plop, 2));
      data = *(unsigned int*)plop;
      data = ((data << 8) & 0xFF00) | ((data >> 8) & 0x00FF);
      printf("Value 1= 0x%04X\n", data);
      sleep(2); */
     

  // }

	//if(RTX_Start_Timer(timerID) != 0){
   //	printf("Error whil initiazing the time : invalid timer ID\n");
   //}

   /*plop[0]=0x55;
   plop[1]=0x55;
   plop2[0]=0;
   plop2[1]=0;
   pouet[0]=0xff;
   pouet[1]=0xff;
   i2c_write(0x40, plop,2);
   i2c_read(0x40,plop2,2);
   printf("read value no 1 = 0x%4x\n",*(unsigned int*)plop2 );
   i2c_write(0x40,pouet,0);
   i2c_write(0x41,pouet,0);
   i2c_read(0x40,plop2,2);
   printf("read value no 2= 0x%4x\n",*(unsigned int*)plop2 );*/

      scan =0x40;//I2C_scan(0x40,0x40);
      translations_i2c[0].i2cNodes[0].nodeAddr=scan;
      printf("Scanned node = 0x%02X\n", scan);

      plop[0] = 0xFF;
      plop[1] = 0xFF;
      i2c_write(scan & 0xFE, plop, 2);


      i2c_read(scan | 0x01, plop, 2);


   while(running) {

      //test PC -> PCF LED and the function update_values
      //can_msg_lookup();

      //test PCF LEC -> PC and the function i2c_changes_lookup
      //                                    i2c_send_changes
     /*if(fpga_changes_lookup(&translations_fpga[0])==0)
      {
      	printf("has changed\n");
      }*/
      fpga_send_changes(translations_fpga, 1);
      i2c_send_changes(translations_i2c, 1);



    //  printf("*");
      /* switch (getch()){
          case 'q': 	running=0;
                    	break;

          case 'l':  printf("read: %d\n",i2c_read(scan | 0x01, plop, 2));
                     printf("%4x\n", *((unsigned int *)plop));
                     break;

          case 'w':	printf("write : %d\n",i2c_write(scan,ffxx,2));
                     break;

          default:	break;
       }*/

     // i2c_changes_lookup(& translations_i2c[0]);
      //printf("*\n");
     // sleep(1);

      //printf("%4x\n", *((unsigned int *)plop));








      // Timer interval of 10 µs.
      //if(flag_timer == 1) {
      	//if(i2c_send_changes(translations_i2c, LENGTH_TAB_I2C) == 0) {
         //	printf("New value event sended correcly\n");
      	//}
      	//fpga_send_changes(translations_fpga, LENGTH_TAB_FPGA);
         //flag_timer = 0;
      //}

      // Other treatments...


      //All ports programmed as input should be set to logic 1 if we want to
      //read thoses ports
  /*    scan = I2C_scan(0x00, 0xFF);
      printf("Scanned node = 0x%02X\n", scan);
      plop[0] = 0xff;
      plop[1] = 0x55;
      if((ret = i2c_write(scan & 0xFE, plop, 2)) != 0) {
       	printf("ERROR %d\n", (unsigned int)ret);
      }
      printf("Sent value = 0x%04X\n", *(unsigned int*)plop);
      sleep(1);
      if(i2c_read(scan | 0x01, plop, 2) == 0) {
       	printf("Read value = 0x%04X\n", *(unsigned int*)plop);
      }
      sleep(1);
  */

   }
   printf("BYE !!!\n");

   i2c_destroy();
   can_destroy();
}
// ------------------------- END OF ENTRY POINT --------------------
