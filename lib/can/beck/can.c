/*
 * Simulator project
 *
 * Institut de Formation des Ingénieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : CAN library
 *
 * Authors : Julien Peeters <julien.peeters@u-psud.fr>
 *			    Fabien Provost <fabien.provost@u-psud.fr>
 *			    Feng Xiong <feng.xiong@u-psud.fr>
 *           Yongchao Xu <yongchao.xu@u-psud.fr>
 *
 * Creation date : 25/03/2009
 */

#include <stdio.h>

#include <can_api.h>

#include <can/can.h>

CAN_PORT_INIT can0Init;
CAN_RX_FILT can0RxFilter;

unsigned int baudrate_tab[9] = {
	1000,
	800,
	500,
	250,
	125,
	100,
	50,
	20,
	10
};

int can_init(unsigned short am, unsigned short ac, unsigned short baudrate){

	int ret;

   can0Init.fDisable_Rx = 0; //FALSE /* We want to be able to receive data. */
   can0Init.Rx_Q_Size = 100;
   can0Init.Tx_Q_Size[0] = 100;
   can0Init.Tx_Q_Size[1] = 0;
   can0Init.Tx_Q_Size[2] = 0;

   can0Init.Config.Baud = baudrate_tab[baudrate];
   can0Init.Config.Mode = 0;
   can0Init.Config.Mask = 0;
   can0Init.Config.Value = 0;
   can0Init.Config.Bit_Rate_Div = 0;

   /* !!! ATTENTION !!! CAN_ID bits are left justified
    * (read from left to right) and are placed in B15 to B5
    * (do not forget to shift id of 5 positions).
    */
   can0RxFilter.Id_Mask.Normal = am;
   can0RxFilter.Id_Mask.Extended = 0xFFFF; /* Don't use 29 bits mode */
   can0RxFilter.Id_Value.Normal = ac;
   can0RxFilter.Id_Value.Extended = 0xFFFF;
   can0RxFilter.Data_Mask = 0xFFFF; /* No mask on data */
	can0RxFilter.Data_Value = 0xFFFF;

	ret = CAN_Open_Port(CAN0_PORT, &can0Init);
   if(ret != 0) {
   	printf("Initialization error of CAN port\n");
   	return -1;
   }

   ret = CAN_Rx_Filters(CAN0_PORT, CAN_FILT1, &can0RxFilter);
   if(ret != 0) {
	   printf("Initialization error of filter\n");
   	return -1;
   }

   return 0;
}

int can_send(can_event_msg_t msg){
	CAN_MSG message;
   int ret;
   int i;

   message.Id.Normal=msg.id;
   for(i=0;i<8;i++){
   	message.Data[i]=msg.data[i];
   }

   /* !!! ATTENTION !!! we want give the value of msg.length at the
   	*four LSB of message.Len_Ctrl
      *and we want to put 1 at the second byte of Len_Ctrl
   */
   message.Len_Ctrl=CAN_DLC_FIELD&msg.length;

   ret=CAN_Send(CAN0_PORT,CAN_TX1,&message);

   if(ret != 0) {
   	printf("Sending CAN message error \n");
      return -1;
   }
   return 0;
}

int can_recv(unsigned short timeout, can_event_msg_t* ptr_msg){
	CAN_MSG message;
   int i;
   int ret;

   if(ptr_msg==NULL){
   	printf("Null message pointer\n");
      return -1;
   }

   ret=CAN_Recv(CAN0_PORT,&message,timeout);

   if(ret != 0){
   	if(ret != CAN_EC_TIMEOUT) {
   		printf("Receiving error\n");
      }
      return -1;
      if(ret == CAN_EC_TIMEOUT){
      	return -2;
      }
   }

   ptr_msg->id = message.Id.Normal;
   for(i=0;i<8;i++){
   	ptr_msg->data[i]=message.Data[i];
   }

   ptr_msg->length=message.Len_Ctrl&CAN_DLC_FIELD;

   return 0;
}

void can_destroy(void){
	CAN_Close_Port(CAN0_PORT);
}

