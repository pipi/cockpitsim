/*
// Sample C code - customize as your Beck IPC application
*/
#include <stdio.h>
#include <clib.h>
#include <ax12.h>
#include <can.h>

can_event_msg_t* ptrmsg;


//------instruction function-----------

int set_Angle(int port, BYTE ID, int angle, BYTE* set_Angle_Buff, int nb_byte) //angle from 0 to 1023, ID 1 to 3
{
	set_Angle_Buff[0]=START;
	set_Angle_Buff[1]=START;
	set_Angle_Buff[2]=ID;
	set_Angle_Buff[3]=7;//4 param + ID + instructions + length
	set_Angle_Buff[4]=INST_WRITE;
	set_Angle_Buff[5]=P_GOAL_POSITION_L;
	set_Angle_Buff[6]=(BYTE)(angle && POS_LSB_MASK);
	set_Angle_Buff[7]=(BYTE)(angle && POS_MSB_MASK);
	set_Angle_Buff[8]=SPEED_VALUE_LSB;
   set_Angle_Buff[9]=SPEED_VALUE_MSB;
	set_Angle_Buff[10]=~(ID + 7 + INST_WRITE + P_GOAL_POSITION_L +set_Angle_Buff[6]+set_Angle_Buff[7]+ set_Angle_Buff[8] + set_Angle_Buff[9]);

	//fossil_writeblock ( FOSSIL_COM, set_Angle_Buff,nByteToWrite_Set );

	if(	fossil_writeblock ( port, set_Angle_Buff,nb_byte )==nb_byte)
		return 1;
	else
		return 0;	

}

int read_Angle(int port, BYTE ID, BYTE* read_Angle_Buff, int nb_byte) // ID 1 to 3
{
	read_Angle_Buff[0]=START;
	read_Angle_Buff[1]=START;
	read_Angle_Buff[2]=ID;
	read_Angle_Buff[3]=4;//2 param(msb &lsb angle) + ID + instructions
	read_Angle_Buff[4]=INST_READ;
	read_Angle_Buff[5]=P_PRESENT_POSITION_L;
	read_Angle_Buff[6]=2;//2 bytes MSB & LSB
	read_Angle_Buff[7]=~ (ID+4+INST_READ+P_PRESENT_POSITION_L+2);//On peut caster en unsigned char, a verifier la necessite

	if(fossil_writeblock(port, read_Angle_Buff,nb_byte)==nb_byte)
		return 1;
	else
		return 0;
}

//--------receive position function------------------------------------


int dern_remplie(sDataAX12* data[], int taille)
{
	int i;
	i=taille-1;
	while((i!=0)&&(data[i]==NULL))
	{
		i--;
		
	}
	return i;
}

void manage_data(sDataAX12* data[], int taille)
{
	int i;
	if(data[taille-1]!=0)//il faut initialiser ttes les cases du tab à une valeur non utilisée
	{
		for(i=taille-1; i>0; i--)
		{
			data[i-1]=data[i];
		}
		data[taille-1]=NULL;
	}
	

}

//---------fill informations from AX12 (positions) in dataAX12 table---------

int fill_data_AX12(sDataAX12* dAX12[], int size_dAX12, BYTE* answer_Angle_Buff)
{
	 	int dern_index, angle_received;
		dern_index=dern_remplie(dAX12,size_dAX12);

		angle_received=answer_Angle_Buff[6];// angle

		if(dern_index!=(size_dAX12-1))
			{
				dAX12[dern_remplie(dAX12,size_dAX12)+1]->ID=answer_Angle_Buff[2];
				dAX12[dern_remplie(dAX12,size_dAX12)+1]->angle=((angle_received<<8)|(answer_Angle_Buff[5]));
			}
		else
			{
				manage_data(dAX12,size_dAX12);//on peut s'en passer si on appelle à chaque remplissage manage_data systématiquement
				dAX12[dern_remplie(dAX12,size_dAX12)+1]->ID=answer_Angle_Buff[2];
				dAX12[dern_remplie(dAX12,size_dAX12)+1]->angle=((angle_received<<8)|(answer_Angle_Buff[5]));
		
			}

    	return 1;

}

//---------detect update in dataAX12 and send changes to dataCAN--------

int transfer_data(sDataAX12* dataAX12[], sDataAX12* dataCAN[], int size_dataAX12, int size_dataCAN)
{
	int i;
   unsigned char ID_dernRemplie;
	i=dern_remplie(dataAX12,size_dataAX12);
	ID_dernRemplie=dataAX12[dern_remplie(dataAX12,size_dataAX12)]->ID;//(unsigned char)(dataAX12[dern_remplie(dataAX12,size_dataAX12)]&ID_MASK>>24);

	while(i!=0)
	{
		if(ID_dernRemplie==((dataAX12[dern_remplie(dataAX12,size_dataAX12)-i]->ID)))
		{
			if((dataAX12[dern_remplie(dataAX12,size_dataAX12)]->angle)!=(dataAX12[dern_remplie(dataAX12,size_dataAX12)-i]->angle))
			{
				dataCAN[dern_remplie(dataCAN,size_dataCAN)+1]->ID=dataAX12[dern_remplie(dataAX12,size_dataAX12)]->ID;
				dataCAN[dern_remplie(dataCAN,size_dataCAN)+1]->angle=dataAX12[dern_remplie(dataAX12,size_dataAX12)]->angle;

			}
			i=0;//finish boucle
			return 1;//transfer finished
		}
		i--;
	}
	return 0;
	
}


//--------CAN-BECK functions--------------

int decode_Msg_CAN(can_event_msg_t* ptr_msg, int angleValue, BYTE* set_Angle_Buff)
{
	angleValue=(int)((ptr_msg->data[0]<<8)|(ptr_msg->data[1]));
	switch(ptr_msg->id)
	{
	case ID_GAZ1:
		{
         set_Angle(FOSSIL_COM, ID_AX1, angleValue, set_Angle_Buff, 11);
			return 1;
			
		}break;
		
	case ID_GAZ2:
		{
         set_Angle(FOSSIL_COM, ID_AX2, angleValue, set_Angle_Buff, 11);
			return 1;
			
		}break;
		
	case ID_TRIM:
		{
         set_Angle(FOSSIL_COM, ID_AX3, angleValue, set_Angle_Buff, 11);
			return 1;
			
		}break;
		
	default : return 1;
	}
	return 0;//indique fin de décodage 
}




int create_msg_CAN(sDataAX12* data_CAN[], int size_dataCAN, can_event_msg_t msg )
{		
	switch ((unsigned char)(data_CAN[dern_remplie(data_CAN,size_dataCAN)-1]->ID))
	{
	case ID_AX1:
		{
			msg.id=ID_GAZ1;
			msg.length=2;
			msg.data[0]=(unsigned char)(((data_CAN[dern_remplie(data_CAN,size_dataCAN)-1]->angle)&POS_MSB_MASK)<<8);
			msg.data[1]=(unsigned char)((data_CAN[dern_remplie(data_CAN,size_dataCAN)-1]->angle)&POS_LSB_MASK);

			return 1;

		}break;

	case ID_AX2:
		{
			msg.id=ID_GAZ2;
			msg.length=2;
			msg.data[0]=(unsigned char)(((data_CAN[dern_remplie(data_CAN,size_dataCAN)-1]->angle)&POS_MSB_MASK)<<8);
			msg.data[1]=(unsigned char)((data_CAN[dern_remplie(data_CAN,size_dataCAN)-1]->angle)&POS_LSB_MASK);
			
			return 1;
		}break;
		
	case ID_AX3:
		{
			msg.id=ID_TRIM;
			msg.length=2;
			msg.data[0]=(unsigned char)(((data_CAN[dern_remplie(data_CAN,size_dataCAN)-1]->angle)&POS_MSB_MASK)<<8);
			msg.data[1]=(unsigned char)((data_CAN[dern_remplie(data_CAN,size_dataCAN)-1]->angle)&POS_LSB_MASK);
			
			return 1;
		}break;
		
	}
	
	return 0;
}

//------------power/angle conversion functions ---------

int angle_pow(int angle)
{
	return (int)(angle*((power_max-power_min)/(angle_max_bin))+power_min);
}

int pow_angle(int pow)
{
	return (int)((pow-power_min)/((power_max-power_min)/(angle_max_bin)));
}

unsigned char separateMSB(int value)
{
	return (unsigned char)((value & MSB_MASK)>>8);
}

unsigned char separateLSB(int value)
{
	return (unsigned char)(value & LSB_MASK);
}
