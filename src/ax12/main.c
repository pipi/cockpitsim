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
	/*can_event_msg* ptrmsg;
	can_event_msg msg;*/
	
	//------var d'états
   int state;


	//------var interne
	unsigned char ID;
	
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

	can_init(CAN_MASK, CAN_ID, CAN_BAUDRATE_1M);

   running = 1;


   while(running) {
      can_msg_lookup();

      // Other treatments...

      unsigned char ID;
	static ID=1; //static en C ??????????????
	switch(state)
		case 0 :
		{
			init_AX12();
			state=1;

		}break;

		case 1 :
			{
				//ID=0;
				if(PA==1)
					state=5;
				else
				{
					remplir_tab(data_AX12, ID);
					end_traitement=remplir_tab(data_AX12, ID);
					printf("dernier data AX 12 = %l",
					if(end_traitement==1)
						state=2;
					else
						state=1;

				}
			}break;

		case 2:
			{
				transfer_data(dataAX12, dataCAN);
				end_transfer=transfer_data(dataAX12, dataCAN);
				if(end_transfer==1)
					state=3;
				else
					state=2;

			}break;

		case 3:
			{
				ID++;
				state=4;

			}break;

		case 4:
			{
				if(ID==3)
				{
					ID=0;
				}
				state=1;
			}break;
			
		case 5:
			{
				can_recv(/*unsigned short timeout,*/ptrmsg);
				state=6;
			}break;
			
		case 6:
			{
				decode_Msg_CAN(ptrmsg);
				end_decode_msg=	decode_Msg_CAN(ptrmsg);
				
				if(end_decode_msg==1)
					state=5;
				else
					state=6;
				
			}break;
			
		default : state=0;
			
	}
	
	
	


	//---------------------tests-------------------

   }

   can_destroy();
}
// ------------------------- END OF ENTRY POINT --------------------
