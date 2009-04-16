/*
 *  can_fs.h
 *  CAN_FS
 *
 *  Created by Alexandra Starynkevitch on 15/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
#define CAN_DATA_LENGTH 256
#define FS_DATA_LENGTH 256
#define U_INT8 0
#define INT8 1
#define U_INT16 2
#define INT16 3
#define U_INT32 4
#define INT32 5
#define FLOAT 6
 
//struct pour les donnees allant et provenant du can
typedef Struct{
	void (*from_can)(can_event_msg_t t);
	void (*to_can)(from_fs_t data[], unsigned short length);
}can_fs_trans_t;


//struct pour les donnees provenant de FS
typedef Struct{
	union {
		float float_value;
		unsigned char uint8_value;
		char int8_value;
		//finir
		//
		//
		int int_value;
	} value;
	short type;
	unsigned short canId;
} from_fs_t;

//note: mettre un flag dans les fonctions to_can
//


//tableau pour les donnees allant et provenant du can
can_fs_trans_t *dCAN[CAN_DATA_LENGTH];

//tableau pour les donnees provenant de FS
from_fs_t *dFS[FS_DATA_LENGTH];


//ouverture de fsuipc et initialisation des 2 structures
void init();

//traite les messages en attente sur le bus can
void lookup_can();

//traite les donnees provenant de fsuipc en cas de changement
void lookup_fsuipc();

//fermeture de fsuipc et destruction des 2 structures
void destroy();





