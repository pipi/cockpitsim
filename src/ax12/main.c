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
#include <ax12.h>
#include <can.h>

// Program constants
#define CAN_MASK	0xFFFF
#define CAN_ID		0xFFFF
#define RTS1 12

// ----------------------- CAN MESSAGE HANDLERS --------------------
void forward_to_ax12(can_event_msg_t msg) {
	// TODO
   // Implement the AX-12 action to the CAN message.
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



	//---déclaration des tab de données AX12 et CAN---
	sDataAX12* data_AX12[size_dataAX12];
	sDataAX12* data_CAN[size_dataCAN];
	
	//-----décl des var msg-------
	can_event_msg_t* ptrmsg;
	can_event_msg_t msg;
	
	//------var d'états
   int state;


	//------var interne
	unsigned char ID;
   int PA;

   //*********

   int i;
   unsigned char dMSB, dLSB;

   //******
   unsigned char* buff;
   unsigned char* ReadBuff;
   unsigned int nByteToWrite;
   unsigned int nByteToRead;

	//-------indicateurs
	
	//-------manuel
	int end_sendIstru; //fin de transmission d'instruction AX12
	int end_rcvAngle; //fin de reception de trames de statut AX12
	int end_traitement;//fin de traitement des données AX12 (concat, remplir tab ..)
	int end_transfer;//fin de détection de mise à jour de la table, transfer des données vers CAN
	
	//---------automatique
	
	int no_msg;//présence d'un msg sur le port can déstiné aux AX12
	int end_rcv_msg;//fin de réception de toute la trame
	int end_decode_msg;//fin du décodage du msg, création de trames instru AX12
	int end_sendAngle;//fin de l'envoi de trame AX12 pour lire l'angle
	int end_treat;//timer pour laisser à l'AX12 le temps de prendre en compte les valeurs


   state=0;
   ID=1; //static en C ??????????????
   PA=1;
   //i=0;
  	buff[0]=START;
   buff[1]=START;
   buff[2]=13;
   buff[3]=7;//long allume LED
   buff[4]=INST_WRITE;
   buff[5]=30;
   buff[6]=8;
   buff[7]=3;
   buff[8]=30;
   buff[9]=0;
   buff[10]=(unsigned char)(~(13+7+INST_WRITE+30+8+3+30+0));

	BIOS_Set_Focus (FOCUS_APPLICATION) ;  

	 /* buff[0]=START;
   buff[1]=START;
   buff[2]=13;
   buff[3]=4;//long allume LED
   buff[4]=INST_WRITE;
   buff[5]=25;
   buff[6]=1;
   buff[7]=(unsigned char)(~(13+4+INST_WRITE+25+1));
   */
   nByteToWrite=11;
   nByteToRead=6;



	//can_init(CAN_MASK, CAN_ID, CAN_BAUDRATE_1M);

   running = 1;

   if(fossil_init ( FOSSIL_COM )!=0x1954)
   	printf("failed opem COM1");
   else
   	printf("open COM1 succeed \n");

   if(fossil_setbaud(FOSSIL_COM,57600,0,8,1)==-1)
   	printf("illegal parameter\n");
   else
   	printf("bit field error = %d \n",fossil_setbaud(FOSSIL_COM,57600,0,8,1));

   fossil_set_flowcontrol(FOSSIL_COM, FOSSIL_FLOWCTRL_OFF);
   fossil_set_rs485 ( FOSSIL_COM, FOSSIL_RS485_LOWACTIVE);
   fossil_set_rs485_txenable ( FOSSIL_COM, -1 );

   fossil_purge_output( FOSSIL_COM);
   fossil_purge_input( FOSSIL_COM);

  //fossil_enable_transmitter ( FOSSIL_COM, FOS_ENABLE);

  //pfe_enable_pio ( RTS0, 4 );
  //fossil_force_rts ( FOSSIL_COM, RTS_LOW);
	//fossil_force_rts ( FOSSIL_COM, RTS_HIGH  );

  //fossil_send_break ( FOSSIL_COM, FOS_LONG_BREAK );

   //pfe_enable_external_dma ( 2 );
   //pfe_enable_pio ( RTS1, 5);
   // hal_write_pio ( RTS1, 0 );

   if(fossil_writeblock(FOSSIL_COM, buff,nByteToWrite)== nByteToWrite)
   	printf("writeblock succed \n ");
   else
   	printf(" writeblock failed");

   	RTX_Sleep_Time (3);
     	//hal_write_pio ( RTS1, 1);
      RTX_Sleep_Time ( 2 );



     //printf(" %d \n ",RTX_Sleep_Time ( 1 ));
     //fossil_force_rts ( FOSSIL_EXT, RTS_HIGH  );

   /*if*/
   printf("%d\n",fossil_readblock(FOSSIL_COM, ReadBuff,nByteToRead)) ;
   //printf("%d\n",fossil_status_request ( FOSSIL_COM )) ;


      	for(i=0;i<nByteToRead;i++)
      		printf(" %d ",ReadBuff[i]);
      //}
   /*else
   printf("waiting for receive\n");
     */




   /*while(running) {
      can_msg_lookup();

      // Other treatments...



	switch(state)
   {
		case 0 :
		{
			init_AX12();
			state=1;

		}break;

		case 1 :
			{
				//ID=0;
				if(PA==1)
					state=7;
				else
				{
					remplir_tab(data_AX12, ID);
					end_traitement=remplir_tab(data_AX12, ID);
					printf("last data AX 12 = %d  %d", data_AX12[dern_remplie(data_AX12,size_dataAX12)]->ID,data_AX12[dern_remplie(data_AX12,size_dataAX12)]->angle);
					if(end_traitement==1)
						{
                  	state=2;
                  }
					else
               {
						state=1;
               }
				}
			}break;

		case 2:
			{
				transfer_data(data_AX12, data_CAN);
				end_transfer=transfer_data(data_AX12, data_CAN);
            printf("last data CAN = %d  %d", data_CAN[dern_remplie(data_CAN,size_dataAX12)]->ID,data_AX12[dern_remplie(data_CAN,size_dataAX12)]->angle);
				if(end_transfer==1)
					state=3;
				else
					state=2;

			}break;

      case 3:
      	{
         	create_msg_CAN(data_CAN,ptrmsg);
            state=4;

         }break;

      case 4:
      	{
         	can_send(*ptrmsg);
            state=5;
         }

		case 5:
			{
				ID++;
				state=6;

			}break;

		case 6:
			{
				if(ID==3)
				{
					ID=0;
				}
				state=1;
			}break;

		case 7:
			{
         	if(PA=0)
            {
            	state=1;
            }
            else
            {
					can_recv(1,ptrmsg);
					state=8;
            }
			}break;

		case 8:
			{
				decode_Msg_CAN(ptrmsg);
				end_decode_msg = decode_Msg_CAN(ptrmsg);

				if(end_decode_msg==1)
					state=8;
				else
					state=7;

			}break;

		default :
      {
      	state=0;
      }


	}





	//---------------------tests-------------------



   can_destroy();
   } */

// ------------------------- END OF ENTRY POINT --------------------
}
