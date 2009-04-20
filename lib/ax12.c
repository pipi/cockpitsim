/*
// Sample C code - customize as your Beck IPC application
*/

#include <clib.h>
#include <ax12.h>
#include <can.h>

void init_AX12() //à faire------------
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID_BROADCAST);
	fossil_putbyte_wait(FOSSIL_COM, 0x0A);//L à définir
	fossil_putbyte_wait(FOSSIL_COM, INST_SINCWRITE);
	fossil_putbyte_wait(FOSSIL_COM, P_ID );

	fossil_putbyte_wait(FOSSIL_COM, 0x01);
	fossil_putbyte_wait(FOSSIL_COM, ID_AX1);

	fossil_putbyte_wait(FOSSIL_COM, 0x01);
	fossil_putbyte_wait(FOSSIL_COM, ID_AX2);

	fossil_putbyte_wait(FOSSIL_COM, 0x01);
	fossil_putbyte_wait(FOSSIL_COM, ID_AX3);


	//fossil_putbyte_wait(FOSSIL_COM, (ID_BROADCAST+0x04+INST_SINCWRITE+P_ID+ID));

}

int set_ID(short ID)
{
fossil_putbyte_wait(FOSSIL_COM, START);
fossil_putbyte_wait(FOSSIL_COM, START);
fossil_putbyte_wait(FOSSIL_COM, ID_BROADCAST);
fossil_putbyte_wait(FOSSIL_COM, 0x04);
fossil_putbyte_wait(FOSSIL_COM, INST_WRITE);
fossil_putbyte_wait(FOSSIL_COM, P_ID );
fossil_putbyte_wait(FOSSIL_COM, ID) ;
fossil_putbyte_wait(FOSSIL_COM, (ID_BROADCAST+0x04+INST_WRITE+P_ID+ID));

  return 1;

}

//---------Trames d'instructions AX12---------


int set_MSB_Angle(unsigned char ID, unsigned char MSB_Angle)
{

	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, WRITE_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_WRITE);
	fossil_putbyte_wait(FOSSIL_COM, P_GOAL_POSITION_H );
	fossil_putbyte_wait(FOSSIL_COM, MSB_Angle);
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_WRITE+P_GOAL_POSITION_H+MSB_Angle));

	return 1;

}

int set_LSB_Angle(unsigned char ID, unsigned char LSB_Angle)
{
	//unsigned char MSB_Angle =.........;
	//unsigned char LSB_Angle =.........;
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, WRITE_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_WRITE);
	fossil_putbyte_wait(FOSSIL_COM, P_GOAL_POSITION_L );
	fossil_putbyte_wait(FOSSIL_COM, LSB_Angle) ;
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_WRITE+P_GOAL_POSITION_L+LSB_Angle));

	return 1;

}

int read_MSB_Angle(unsigned char ID)
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, READ_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_READ);
	fossil_putbyte_wait(FOSSIL_COM, P_PRESENT_POSITION_H);
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_READ+P_PRESENT_POSITION_H));

	return 1;


}

int read_LSB_Angle(unsigned char ID)
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, READ_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_READ);
	fossil_putbyte_wait(FOSSIL_COM, P_PRESENT_POSITION_L) ;
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_READ+P_PRESENT_POSITION_L));

	return 1;

}


int init_couple_max_MSB(unsigned char ID, unsigned char couple_MSB)
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, WRITE_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_WRITE);
	fossil_putbyte_wait(FOSSIL_COM, P_MAX_TORQUE_H);
	fossil_putbyte_wait(FOSSIL_COM, couple_MSB);
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_READ+P_MAX_TORQUE_H+couple_MSB));

	return 1;

}

int init_couple_max_LSB(unsigned char ID, unsigned char couple_LSB)
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, WRITE_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_WRITE);
	fossil_putbyte_wait(FOSSIL_COM, P_MAX_TORQUE_L);
	fossil_putbyte_wait(FOSSIL_COM, couple_LSB);
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_READ+P_MAX_TORQUE_L+couple_LSB));

	return 1;

}

int enable_couple(unsigned char ID)
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, WRITE_LENGTH-1);
	fossil_putbyte_wait(FOSSIL_COM, INST_WRITE);
	fossil_putbyte_wait(FOSSIL_COM, P_TORQUE_ENABLE);
	fossil_putbyte_wait(FOSSIL_COM, 0x01);
	fossil_putbyte_wait(FOSSIL_COM, ID+WRITE_LENGTH-1+INST_WRITE+P_TORQUE_ENABLE+0x01);

	return 1;
}

int read_MSB_charge(unsigned char ID)
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, READ_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_READ);
	fossil_putbyte_wait(FOSSIL_COM, P_PRESENT_LOAD_H);
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_READ+P_PRESENT_LOAD_H));

	return 1;

}


int read_LSB_charge(unsigned char ID)
{
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, START);
	fossil_putbyte_wait(FOSSIL_COM, ID);
	fossil_putbyte_wait(FOSSIL_COM, READ_LENGTH);
	fossil_putbyte_wait(FOSSIL_COM, INST_READ);
	fossil_putbyte_wait(FOSSIL_COM, P_PRESENT_LOAD_L);
	fossil_putbyte_wait(FOSSIL_COM, (ID+INST_READ+P_PRESENT_LOAD_L));

	return 1;


}


//----------reception----------

unsigned char get_MSB_Angle(unsigned char ID)
{//d signifie DATA

	int i;
	unsigned char dMSB;
	read_MSB_Angle(ID);
	for(i=0;i<6;i++)
	{
		fossil_getbyte_wait(FOSSIL_COM);
		if(i==4)
			dMSB=fossil_getbyte_wait(FOSSIL_COM);
	}
	return dMSB;

}

unsigned char get_LSB_Angle(unsigned char ID)
{//d signifie DATA
	int i;
	unsigned char dLSB;
	read_LSB_Angle(ID);
	for(i=0;i<6;i++)
	{
		fossil_getbyte_wait(FOSSIL_COM);
		if(i==4)
			dLSB=fossil_getbyte_wait(FOSSIL_COM);
	}
	return dLSB;

}

unsigned int concat_Angle(unsigned char ID)
{
	unsigned int Angle, AngleL;
	//Angle=ID;
	Angle=get_MSB_Angle(ID);
	AngleL=get_LSB_Angle(ID);
	Angle=((Angle<<8)||(AngleL));
	return Angle;
}

int remplir_tab(sDataAX12* data_AX12[], unsigned char ID)
{
	int dern_index;
	dern_index=dern_remplie(data_AX12,size_dataAX12);
	if(dern_index!=(size_dataAX12-1))
	{
		data_AX12[dern_remplie(data_AX12,size_dataAX12)+1]->ID=ID;
		data_AX12[dern_remplie(data_AX12,size_dataAX12)+1]->angle=concat_Angle(ID);
	}
	else
	{
		manage_data(data_AX12,size_dataAX12);//on peut s'en passer si on appelle à chaque remplissage manage_data systématiquement
		data_AX12[size_dataAX12-1]->ID=ID;
		data_AX12[size_dataAX12-1]->angle=concat_Angle(ID);

	}
	return 1;

}

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

void manage_data(sDataAX12* data[], int taille )
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

//-----------------------------------------------------------------

int transfer_data(sDataAX12* dataAX12[], sDataAX12* dataCAN[])
{

/*if(dataAX12[dern_remplie(dataAX12,size_dataAX12)]!=dataAX12[dern_remplie(dataAX12,size_dataAX12)-1])
	dataCAN[dern_remplie(dataCAN,size_dataCAN)+1]=dataAX12[dern_remplie(dataAX12,size_dataAX12)];*/
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
			i=0;//la boucle s'arrête
			return 1;//fin transfer
		}
		i--;
	}
	return 0;

}


//--------CAN-BECK--------------

int decode_Msg_CAN(can_event_msg_t* ptr_msg)
{
	switch(ptr_msg->id)
	{
	case ID_GAZ1:
		{
			set_MSB_Angle(ID_AX1, ptr_msg->data[0]);//en supposant que data[0] est le MSB de l'angle déjà converti
			set_LSB_Angle(ID_AX1, ptr_msg->data[1]);//en supposant que data[1] est le LSB de l'angle déjà converti
			return 1;

		}break;

	case ID_GAZ2:
		{
			set_MSB_Angle(ID_AX2, ptr_msg->data[0]);//en supposant que data[0] est le MSB de l'angle déjà converti
			set_LSB_Angle(ID_AX2, ptr_msg->data[1]);//en supposant que data[1] est le LSB de l'angle déjà converti
			return 1;

		}break;

	case ID_TRIM:
		{
			set_MSB_Angle(ID_AX3, ptr_msg->data[0]);//en supposant que data[0] est le MSB de l'angle déjà converti
			set_LSB_Angle(ID_AX3, ptr_msg->data[1]);//en supposant que data[1] est le LSB de l'angle déjà converti
			return 1;

		}break;

	default : return 1;
	}
	return 0;//indique fin de décodage
}




int create_msg_CAN(sDataAX12* data_CAN[],can_event_msg* ptrmsg )
{

	switch ((data_CAN[dern_remplie(dataAX12,size_dataAX12)-i]->ID))
	{
	case ID_AX1:
		{
			prtmsg->id=ID_GAZ1;
			prtmsg->length=2;
			prtmsg->data[0]=(unsigned char)(((data_CAN[dern_remplie(dataAX12,size_dataAX12)-i]->angle)& POS_MSB_MASK)>>8);//après conversion
			prtmsg->data[1]=(unsigned char)(((data_CAN[dern_remplie(dataAX12,size_dataAX12)-i]->angle) & POS_LSB_MASK));//après conversion

			return 1;

		}break;

	case ID_AX2:
		{
			prtmsg->id=ID_GAZ2;
			prtmsg->length=2;
			prtmsg->data[0]=(unsigned char)(((data_CAN[dern_remplie(dataAX12,size_dataAX12)-i]->angle)& POS_MSB_MASK)>>8);//après conversion
			prtmsg->data[1]=(unsigned char)(((data_CAN[dern_remplie(dataAX12,size_dataAX12)-i]->angle) & POS_LSB_MASK));//après conversion

			return 1;
		}break;

	case ID_AX3:
		{
			prtmsg->id=ID_TRIM;
			prtmsg->length=2;
			prtmsg->data[0]=(unsigned char)(((data_CAN[dern_remplie(dataAX12,size_dataAX12)-i]->angle)& POS_MSB_MASK)>>8);//après conversion
			prtmsg->data[1]=(unsigned char)(((data_CAN[dern_remplie(dataAX12,size_dataAX12)-i]->angle) & POS_LSB_MASK));//après conversion

			return 1;
		}break;

	}

	return 0;
}


