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


//--------comm------

#define ID_BROADCAST 0xFE

#define START 0xFF
#define ID_MASK 0xFF000000
#define POS_MSB_MASK 0xFF00
#define POS_LSB_MASK 0x00FF
#define TORQUE_MASK 0x000000FF

//---------data_length-----

#define WRITE_LENGTH 0x04
#define READ_LENGTH 0x02

//---------taille data---

#define size_dataAX12 12
#define size_dataCAN 12 //à définir

//---------ID AX12------
#define ID_AX1 0x01//GAZ1
#define ID_AX2 0x02//GAZ2
#define ID_AX3 0x03//TRIM
#define nbAX12 3 //nb d'ax12 peut varier

//---------ID CAN-COMMANDE GAZ ET TRIM---------

#define ID_GAZ1 0x0000//à définir et à vérifier
#define ID_GAZ2 0x0020
#define ID_TRIM 0x0040

//--------Structure de données---------

typedef struct 
{
	unsigned char ID;
	unsigned int angle;
	
}sDataAX12;

//-------FONCTIONS-------

void init_AX12();

int set_MSB_Angle(unsigned char ID, unsigned char MSB_Angle);

int set_LSB_Angle(unsigned char ID, unsigned char LSB_Angle);

int read_MSB_Angle(unsigned char ID);

int read_LSB_Angle(unsigned char ID);

int init_couple_max_MSB(unsigned char ID, unsigned char couple_MSB);

int init_couple_max_LSB(unsigned char ID, unsigned char couple_LSB);

int enable_couple(unsigned char ID);

int read_MSB_charge(unsigned char ID);

int read_LSB_charge(unsigned char ID);

unsigned char get_MSB_Angle(unsigned char ID);

unsigned char get_LSB_Angle(unsigned char ID);

unsigned int concat_Angle(unsigned char ID);

int remplir_tab(sDataAX12* data_AX12[], unsigned char ID);

int dern_remplie(sDataAX12* data[], int taille);

void manage_data(sDataAX12* data[], int taille );

int transfer_data(sDataAX12* dataAX12[], sDataAX12* dataCAN[]);

int decode_Msg_CAN(can_event_msg_t* ptr_msg);

int create_msg_CAN(sDataAX12* data_CAN[], can_event_msg_t* ptrmsg );







