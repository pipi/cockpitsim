/*
 * Simulator project
 *
 * Institut de Formation des Ingénieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : Beck main program
 *
 * Authors : Salwa BAHJI
 				 Herilanja RAMANAHADRAY
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

     // Other treatments...



	//---déclaration des tab de données AX12 et CAN---
    sDataAX12* data_AX12[sizeDataAX12];
    sDataAX12* data_CAN[sizeDataCAN];

    //----déclaration des buffers d'entrées et de sorties---

    BYTE*  set_Angle_Buff;
    BYTE* input_Buff, *output_Buff;

    //-----décl des var msg-------
    static can_event_msg_t* ptrmsg;
    static can_event_msg_t msg;

    //-----angle value
    static int angle_Value;

    //------var d'états

   static int state;


    //------indicateur de Pilote auto----
    static int PA;
    //------var interne
    static BYTE ID;
    int i;


   //------initialisations--------
   state=1;
   ID=13;
   PA=0;
   running=1;

    if(fossil_init ( FOSSIL_COM )!=0x1954)
        printf(" open COM1 failed \n ");
    else
        printf("open COM1 succeed \n");

    if(fossil_setbaud(FOSSIL_COM,57600,0,8,1)==-1)
        printf("illegal parameter\n");
    else
        printf("bit field error = %d \n",fossil_setbaud(FOSSIL_COM,57600,0,8,1));

    fossil_set_flowcontrol(FOSSIL_COM, FOSSIL_FLOWCTRL_OFF);
    fossil_set_rs485 ( FOSSIL_COM, FOSSIL_RS485_LOWACTIVE);
    fossil_set_rs485_txenable ( FOSSIL_COM, -1 );//-1 to select RTS

    fossil_purge_output( FOSSIL_COM);
    fossil_purge_input( FOSSIL_COM);

   while(running) {
      //can_msg_lookup();

         switch(state){

        case 1 :
            {   printf("state 1 : get angle \n");
                //ID=0;
                if(PA==1)
                {
                    state=8;
                    printf("automatic pilot enabled \n");

                }
                else
                {
                    if(read_Angle(FOSSIL_COM, ID, output_Buff, nByteToRead )==1)
                    {
                        printf("byte sent : \n");
                        for(i=0;i<nByteToRead;i++)
                        	{
                           	printf("%d ",output_Buff[i]);
                           }
                        printf(" \n ");

                        state=2;

                    }
                    else
                    {
                        state=1;
                        printf("sending instructions failed \n");

                    }
                }
            }break;

        case 2:
            {
            	printf("state 2 : receiving \n");
            	//printf(" sleep in %d ms ",RTX_Sleep_Time(2));    
               printf("nb data received % d \n",fossil_readblock(FOSSIL_COM, input_Buff, nByteToRead));
               printf("bytes received \n");
               for(i=0;i<nByteToRead;i++)
               {
               	printf("%d ", input_Buff[i]);
               }
               printf("\n");
               state=3;

            } break;

        case 3:
            {
            	printf("state 3 : fill position in data AX12 \n");

                if(fill_data_AX12(data_AX12, sizeDataAX12, input_Buff)==1)
                {
                    state=4;
                    printf("receiving informations succed \n");
                    printf("last data  : %d %d \n",data_AX12[dern_remplie(data_AX12, sizeDataAX12)+1]->ID,data_AX12[dern_remplie(data_AX12, sizeDataAX12)+1]->angle);
                }
                else
                {
                    state=3;
                    printf("receiving informations failed \n");
                }




            }break;

        case 4:
            {
            	printf(" state 4 :  detect update and fill data_can \n ");
                transfer_data(data_AX12, data_CAN, sizeDataAX12, sizeDataCAN);
                printf("end transfer \n");
                state=5;
            }break;

        case 5:
            {
            	printf(" state 5 : create and send can msg \n");
                create_msg_CAN(data_CAN, sizeDataCAN, msg );
                can_send(msg);
                state=6;

            }break;

        case 6:
            {
            	printf("state 6 : ID ++ \n");
                //ID++;
                state=7;

            }break;

        case 7:
            {
            	printf("state 7 : test if ID = last ID_AX12 and initialize ID \n");
                if(ID==3)
                {
                    ID=1;
                }
                state=1;
            }break;

        case 8:
            {
            	printf(" state 8 : automatic pilot enabled  receiving can msg \n ");
                if(PA==0)
                {
                    state=1;
                }
                can_recv(1,ptrmsg);
                state=9;
            }break;

        case 9://waiting for can message
            {
             printf(" state 9 : waiting for can msg \n");
                //------
                state=10;
            } break;

        case 10://receiving can message
            {
                //--can receive message function
                printf(" state 10 : receiving can msg \n");
                can_recv(1, ptrmsg);
                state=11;
            } break;

        case 11:
            {
            	printf(" state 11 : decoding can msg and setting AX12 positions \n");

                if(decode_Msg_CAN(ptrmsg,angle_Value, set_Angle_Buff)==1)
                    state=11;
                else
                    state=12;

            }break;

        case 12:
            {
            	printf(" state 12 : tempo \n");
                sleep(3);
                sleep(2);
                //may not be sufficient
                state=8;
            }

        default : state=0;

        }




	//---------------------tests-------------------



   //can_destroy();
   }

// ------------------------- END OF ENTRY POINT --------------------
}
