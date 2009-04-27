/*
 * Simulator project
 *
 * Institut de Formation des Ing�nieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : Beck main program
 *
 * Authors :
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



	//---d�claration des tab de donn�es AX12 et CAN---
    sDataAX12* data_AX12[sizeDataAX12];
    sDataAX12* data_CAN[sizeDataCAN];

    //----d�claration des buffers d'entr�es et de sorties---
    //static BYTE* request_Angle_Buff;
    static BYTE* read_Angle_Buff;
    static BYTE* set_Angle_Buff;
   static BYTE* answer_Angle_Buff;
    

    //-----d�cl des var msg-------
    static can_event_msg_t* ptrmsg;
    static can_event_msg_t msg;

    //-----angle value
    static int angle_Value;

    //------var d'�tats

    static int state;


    //------indicateur de Pilote auto----
    static int PA;
    //------var interne
    static BYTE ID;
    int i;

    //-------indicateurs

    //-------manuel
    /*int end_sendIstru; //fin de transmission d'instruction AX12
    int end_rcvAngle; //fin de reception de trames de statut AX12
    int end_traitement;//fin de traitement des donn�es AX12 (concat, remplir tab ..)
    int end_transfer;//fin de d�tection de mise � jour de la table, transfer des donn�es vers CAN*/

    //---------automatique

 /*   int no_msg;//pr�sence d'un msg sur le port can d�stin� aux AX12
    int end_rcv_msg;//fin de r�ception de toute la trame
    int end_decode_msg;//fin du d�codage du msg, cr�ation de trames instru AX12
    int end_sendAngle;//fin de l'envoi de trame AX12 pour lire l'angle
    int end_treat;//timer pour laisser � l'AX12 le temps de prendre en compte les valeurs*/

   //------initialisations--------
   state=1;
   ID=13;
   PA=0;
   running=1;


   //------init buffers---------
   /*read_Angle_Buff[0]=START;
	read_Angle_Buff[1]=START;
	read_Angle_Buff[2]=ID;
	read_Angle_Buff[3]=4;//2 param(msb &lsb angle) + ID + instructions
	read_Angle_Buff[4]=INST_READ;
	read_Angle_Buff[5]=P_GOAL_POSITION_L;
	read_Angle_Buff[6]=2;//2 bytes MSB & LSB
	read_Angle_Buff[7]=~ (ID+4+INST_READ+P_GOAL_POSITION_L+2);//On peut caster en unsigned char, a verifier la necessite
     */


    // ne contient pas encore la partie Beck-Can

    BIOS_Set_Focus (FOCUS_APPLICATION) ;

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
        /*case 0 :
        {
            init_AX12();
            state=1;

        }break;*/

        case 1 :
            {   printf("state 1 : get angle \n");
                //ID=0;
                if(PA==1)
                {
                    state=6;
                    printf("automatic pilot enabled \n");

                }
                else
                {
                    //read_Angle(ID,read_Angle_Buff);
                    //end_sendIstru=read_Angle(ID,read_Angle_Buff);
                    //fossil_purge_output( FOSSIL_COM);
                    if(read_Angle(FOSSIL_COM, ID,read_Angle_Buff, nByteToRead )==1)
                    {
                        state=2;
                        printf("sending instructions succed \n");
                        for(i=0;i<nByteToRead;i++)
                        	{
                           	printf("%d ",read_Angle_Buff[i]);
                           }
                        printf(" \n ");

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
            	printf("state 2 : wait for receiving position \n");
                //sleep(3);
                //fossil_purge_input( FOSSIL_COM);
                sleep(1);
                sleep(1);
                sleep(1);
                sleep(1);
                sleep(1);
                sleep(1);

                printf("nb data received % d \n",fossil_readblock( FOSSIL_COM, answer_Angle_Buff,nByteToRead ));
                printf("bytes received \n");
                for(i=0;i<nByteToRead;i++)
                		printf("%d ", answer_Angle_Buff[i]);
                printf("\n");

                state=3;
            } break;

        case 3:
            {
            	printf("state 3 : fill position in data AX12 \n");
                //fill_data_AX12(data_AX12, sizeDataAX12, answer_Angle_Buff);
                //end_rcvAngle=fill_data_AX12(data_AX12, sizeDataAX12, answer_Angle_Buff);
                if(fill_data_AX12(data_AX12, sizeDataAX12, answer_Angle_Buff)==1)
                {
                    state=4;
                    printf("receiving informations succed \n");
                    printf("last data  : %d %d \n",data_AX12[dern_remplie(data_AX12, sizeDataAX12)]->ID,data_AX12[dern_remplie(data_AX12, sizeDataAX12)]->angle);
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
                /*decode_Msg_CAN(ptrmsg,angle_Value, set_Angle_Buff);
                end_decode_msg=	decode_Msg_CAN(ptrmsg,angle_Value, set_Angle_Buff);*/

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
