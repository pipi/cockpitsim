#include <can.h>

//------Control table-------
#define P_ID 3
#define P_BAUD_RATE 4

#define P_CW_ANGLE_LIMIT_L 6
#define P_CW_ANGLE_LIMIT_H 7
#define P_CCW_ANGLE_LIMIT_L 8
#define P_CCW_ANGLE_LIMIT_H 9
#define P_SYSTEM_DATA2 10

#define P_MAX_TORQUE_L 14
#define P_MAX_TORQUE_H 15

#define P_TORQUE_ENABLE (24)



#define P_GOAL_POSITION_L (30)
#define P_GOAL_POSITION_H (31)

#define P_TORQUE_LIMIT_L (34)
#define P_TORQUE_LIMIT_H (35)

#define P_PRESENT_POSITION_L (36)
#define P_PRESENT_POSITION_H (37)

#define P_PRESENT_LOAD_L (40)
#define P_PRESENT_LOAD_H (41)


//--- Instruction ---


#define INST_READ 0x02
#define INST_WRITE 0x03
#define INST_REG_WRITE 0x04
#define INST_ACTION 0x05
#define INST_RESET 0x06
#define INST_SINCWRITE 0x83

//-----default value
#define SPEED_VALUE_LSB 0;
#define SPEED_VALUE_MSB 2;



//--------comm------

#define ID_BROADCAST 0xFE

#define START 0xFF
//#define ID_MASK 0xFF000000
#define POS_MSB_MASK 0xFF00
#define POS_LSB_MASK 0x00FF
//#define TORQUE_MASK 0x000000FF

//---------data_length-----

#define WRITE_LENGTH 0x04
#define READ_LENGTH 0x02

//---------taille data---

#define sizeDataAX12 12
#define sizeDataCAN 12 //à définir

//---------ID AX12------
#define ID_AX1 0x01//GAZ1
#define ID_AX2 0x02//GAZ2
#define ID_AX3 0x03//TRIM
#define nbAX12 3 //nb d'ax12 peut varier

//---------ID CAN-COMMANDE GAZ ET TRIM---------

#define ID_GAZ1 0x0000//à définir et à vérifier
#define ID_GAZ2 0x0020
#define ID_TRIM 0x0040

//--------Data count--------
#define nByteToWrite_Set 11
//#define nByteToWrite_Read 8
#define nByteToRead 8

//-------fs_ax value---------

#define power_min -4096
#define power_max 16384
#define angle_Max 300
#define angle_max_bin 0x03FF
#define angle_min_bin 0x0000
#define MSB_MASK 0xFF00
#define LSB_MASK 0x00FF

//--------Structure de données---------
typedef unsigned char BYTE;

typedef struct 
{
	unsigned char ID;
	unsigned int angle;
	
}sDataAX12;

//-------FONCTIONS-------

//void initAX12();

int set_Angle(int port, BYTE ID, int angle, BYTE* set_Angle_Buff, int nb_byte); //angle from 0 to 1023, ID 1 to 3

int read_Angle(int port, BYTE ID, BYTE* read_Angle_Buff, int nb_byte); // ID 1 to 3

int fill_data_AX12(sDataAX12* dAX12[], int size_dAX12, BYTE* answer_Angle_Buff);

int dern_remplie(sDataAX12* data[], int taille);

void manage_data(sDataAX12* data[], int size );

int transfer_data(sDataAX12* dataAX12[], sDataAX12* dataCAN[], int size_dataAX12, int size_dataCAN);

int decode_Msg_CAN(can_event_msg_t* ptr_msg, int angleValue, BYTE* set_Angle_Buff);

int create_msg_CAN(sDataAX12* data_CAN[], int size_dataCAN, can_event_msg_t msg);















