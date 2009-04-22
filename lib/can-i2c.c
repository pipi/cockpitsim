/*
 * Simulator project
 *
 * Institut de Formation des Ingénieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : CAN-I2C library
 *
 * Authors : Julien Peeters <julien.peeters@u-psud.fr>
 *			    Fabien Provost <fabien.provost@u-psud.fr>
 *			    Feng Xiong <feng.xiong@u-psud.fr>
 *           Yongchao Xu <yongchao.xu@u-psud.fr>
 *
 * Creation date : 02/04/2009
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <can.h>
#include <i2c.h>
#include <can-i2c.h>

#define mask_ID_CAN 0xFFE0

int i2c_changes_lookup(i2c_can_trans_t* trans) {
	short i, index;
   char dataBuf[8];
   unsigned char hasChanged;
   short ret;
   short test;

   hasChanged=0;
   index = 0;
   for(i = 0; i < trans->nodesLength; ++i) {

     	test=i2c_read(trans->i2cNodes[i].nodeAddr, dataBuf+index,
      		trans->i2cNodes[i].dataLength);
   	if(test != 0) {
      	// If there is no new value, copy the old one.
			memcpy(dataBuf+index, trans->oldData+index,
         	trans->i2cNodes[i].dataLength);
   	}
      index += trans->i2cNodes[i].dataLength;
   }


   hasChanged = memcmp(dataBuf, trans->oldData, trans->dataLength);

   if(hasChanged!=0){
   	printf("has changed = %d \nread new value=  0x%4x\n",hasChanged,*(unsigned int*)dataBuf);
   }
	memcpy(trans->oldData, dataBuf, trans->dataLength);
   //printf("oldData=  0x%4x\n",*(unsigned int*)trans->oldData);
   return (hasChanged != 0 ? 0 : -1);
}

int i2c_send_changes(i2c_can_trans_t trans[],
					  		unsigned short length) {
   short i;
   int ret;
   can_event_msg_t msg;

   ret = 0;
   for(i = 0; i < length; ++i) {
      if(i2c_changes_lookup(& trans[i]) == 0) {
      	memset(&msg, 0, sizeof(msg));
         msg.id = trans[i].canId;
         memcpy(msg.data, trans[i].oldData, trans[i].dataLength);
         msg.length = trans[i].dataLength;
      	ret = (can_send(msg) == -1) ? -1 : ret;
         printf("read new value=  0x%4x\n",*(unsigned int*)msg.data);
      } else {
        //	printf("No changes on I2C nodes\n");
      }
   }

   return ret;
}


int fpga_changes_lookup(i2c_can_trans_t* trans) {
	short i, index;
   char dataBuf[8];
   unsigned char hasChanged;
   char plop=0x00;
   short ret;
   short test;

   hasChanged=0;
   index = 0;
   for(i = 0; i < trans->nodesLength; ++i) {
      i2c_write(trans->i2cNodes[i].nodeAddr, &plop, 1);
     	test=i2c_read(trans->i2cNodes[i].nodeAddr, dataBuf+index,
      		trans->i2cNodes[i].dataLength);
   	if(test != 0) {
      	// If there is no new value, copy the old one.
			memcpy(dataBuf+index, trans->oldData+index,
         	trans->i2cNodes[i].dataLength);
   	}
      index += trans->i2cNodes[i].dataLength;
   }


   hasChanged = memcmp(dataBuf, trans->oldData, trans->dataLength);

   if(hasChanged!=0){
   	printf("has changed = %d \nread new value=  0x%4x\n",hasChanged,*(unsigned int*)dataBuf);
   }
	memcpy(trans->oldData, dataBuf, trans->dataLength);

   //printf("oldData=  0x%4x\n",*(unsigned int*)trans->oldData);
   //printf("msg send : 0x%4x0x%4x\n",*(unsigned int*)dataBuf,*(unsigned int*)(dataBuf+2));
   return (hasChanged != 0 ? 0 : -1);
}

int fpga_send_changes(i2c_can_trans_t trans[],
					  		unsigned short length) {
   short i;
   int ret;
   can_event_msg_t msg;

   ret = 0;
   for(i = 0; i < length; ++i) {
      if(fpga_changes_lookup(& trans[i]) == 0) {
      	memset(&msg, 0, sizeof(msg));
         msg.id = trans[i].canId;
         memcpy(msg.data, trans[i].oldData, trans[i].dataLength);
         msg.length = trans[i].dataLength;
      	ret = (can_send(msg) == -1) ? -1 : ret;
         printf("read new value=  0x%4x0x%4x\n",*(unsigned int*)msg.data,*(unsigned int*)(msg.data+2));
      } else {
        //	printf("No changes on I2C nodes\n");
      }
   }

   return ret;
}


int altitude_changes_lookup(i2c_can_trans_t* trans,unsigned long* nextAlt) {
	short i, index;
   char dataBuf[8];
   unsigned char hasChanged;
   char plop=0x00;
   short ret;
   short test;
   unsigned int oldPosition;
   unsigned int newPosition;


   hasChanged=0;
   index = 0;
   for(i = 0; i < trans->nodesLength; ++i) {
      i2c_write(trans->i2cNodes[i].nodeAddr, &plop, 1);
     	test=i2c_read(trans->i2cNodes[i].nodeAddr, dataBuf+index,
      		trans->i2cNodes[i].dataLength);
   	if(test != 0) {
      	// If there is no new value, copy the old one.
			memcpy(dataBuf+index, trans->oldData+index,
         	trans->i2cNodes[i].dataLength);
   	}
      index += trans->i2cNodes[i].dataLength;
   }


   hasChanged = memcmp(dataBuf, trans->oldData, trans->dataLength);

   if(hasChanged!=0){
   	printf("has changed = %d \nread new value=  0x%4x\n",hasChanged,*(unsigned int*)dataBuf);
      oldPosition=((unsigned int)(trans->oldData[1])&0x00FF)|(((unsigned int)(trans->oldData[0])<<8)&0xFF00);
      newPosition=((unsigned int)(dataBuf[1])&0x00FF)|(((unsigned int)(dataBuf[0])<<8)&0xFF00);
      *nextAlt=*nextAlt+(100*(newPosition-oldPosition));
   }
	memcpy(trans->oldData, dataBuf, trans->dataLength);

   //printf("oldData=  0x%4x\n",*(unsigned int*)trans->oldData);
   //printf("msg send : 0x%4x0x%4x\n",*(unsigned int*)dataBuf,*(unsigned int*)(dataBuf+2));
   return (hasChanged != 0 ? 0 : -1);
}


int altitude_send_changes(i2c_can_trans_t trans[],
					  		unsigned short length, unsigned long nextAlt) {
   short i;
   int ret;
   can_event_msg_t msg;

   ret = 0;
   for(i = 0; i < length; ++i) {
      if(altitude_changes_lookup(& trans[i],&nextAlt) == 0) {


      	memset(&msg, 0, sizeof(msg));
         msg.id = trans[i].canId;
         msg.data[0] =(nextAlt>>24)&0x000000FF;
         msg.data[1] =(nextAlt>>16)&0x000000FF;
         msg.data[2] =(nextAlt>>8)&0x000000FF;
         msg.data[3] =(nextAlt)&0x000000FF;
         msg.length = trans[i].dataLength;
      	ret = (can_send(msg) == -1) ? -1 : ret;
         printf("read new value=  0x%4x0x%4x\n",*(unsigned int*)msg.data,*(unsigned int*)(msg.data+2));
      } else {
        //	printf("No changes on I2C nodes\n");
      }
   }

   return ret;
}


int update_values(can_event_msg_t msg,
						i2c_can_trans_t trans[],
                  unsigned short length){
   short i, j, index;
   int ret;

   index = 0;
   ret = 0;
   for(i = 0; i < length; ++i) {
   	if(trans[i].canId == msg.id) {
      	for(j = 0; j < trans[i].nodesLength; ++j) {
      		if(i2c_write(trans[i].i2cNodes[j].nodeAddr & 0xFE, msg.data+index,
      				trans[i].i2cNodes[j].dataLength) == 0) {
	      		// If there is no new value, copy the old one.
					memcpy(trans[i].oldData+index, msg.data+index,
         			trans[i].i2cNodes[j].dataLength);
   			} else {
             	ret = -1;
            }
				index += trans[i].i2cNodes[j].dataLength;
         }
         break;
      }
   }

   return ret;
}


int keyboard_decode(unsigned char slave,unsigned short tab[8][9]){

   unsigned char replig[2];
   unsigned char repcol[2];
   unsigned int regX;
   unsigned int regY;
   unsigned short button;
   unsigned int X;
   unsigned int Y;
   unsigned int cmpt;
   int i;


   regX=0;
   regY=0x80FF;

   //column test XY=2 ==> 9bit
   keyboard_detection(slave,regX,regY,repcol,2);

   if (repcol[0]==0xFF && (repcol[1]&0x80)==0x80){
      return 0;
   }

   else{
      printf("Touch detection");
      regX=0xFFFF;
      regY=((unsigned int)repcol[1]<<8)&0xFF00;
      regY=(unsigned int)repcol[0]|regY;

      //lines test XY=1 ==> 8bit
      keyboard_detection(slave,regX,regY,replig,1);
   }

   cmpt=0;
   //for matrice 9x8
   for(i=0;i<8;i++){
      if(((repcol[0]>>i)&0x01==0)){
         Y=i;
         cmpt=cmpt+1;
      }
   }
   if((repcol[1]>>7)&0x01==0){
      Y=8;
      cmpt=cmpt+1;
   }
   if(cmpt>1){
      printf("Matrice detection error\n");
      return -1;
   }


   cmpt=0;
   //for matrice 9x8
   for(i=0;i<8;i++){
      if(((replig[0]>>i)&0x01==0)){
         X=i;
         cmpt=cmpt+1;
      }
   }
   if(cmpt>1){
      printf("Matrice detection error\n");
      return -1;
   }

	return tab[X][Y];
}



int keyboard_detection(unsigned char slave,unsigned int regx,unsigned int regy,unsigned char rep[2],
									unsigned int XY){

unsigned char buffer[5];
int test;

buffer[0]=0x00;
buffer[1]= regx&0x0F;
buffer[2]=(regx>>8)&0x0F;
buffer[3]=regy&0x0F;
buffer[4]=(regy>>8)&0x0F;



test=i2c_write(slave & 0xFE,buffer,5);
if(XY==1){
   test=i2c_write(slave & 0xFE, buffer,1);
}
else{
buffer[0]=0x02;
  	test=i2c_write(slave & 0xFE, buffer,1);
}
test=i2c_read(slave | 0x01,rep,2);

return 0;

}




