/*
 * Simulator project
 *
 * Institut de Formation des Ingénieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : CAN library
 *
 * Authors : Julien Peeters <julien.peeters@u-psud.fr>
 *			     Fabien Provost <fabien.provost@u-psud.fr>
 *			     Feng Xiong <feng.xiong@u-psud.fr>
 *           Yongchao Xu <yongchao.xu@u-psud.fr>
 *
 * Creation date : 02/04/2009
 */

 /*!
  \file can-i2c.c
  \brief library to translate can messages to i2c messages
 */

#include <stdio.h>

#include "../../lib/can/can.h"
#include "../../lib/i2c/i2c.h"
#include "../../lib/can-i2c.h"


int send_changes(can_i2c_trans_t trans[], unsigned short length){
   char buffer[8];
   can_event_msg_t msg;
	int i, j, flag, test, ret;

   ret = 0;
   for(i=0;i<length;i++){
   	i2c_read(trans[i].idI2C, buffer, trans[i].length);
      flag=0;
      for(j=0;j<trans[i].length;j++){
      	if(trans[i].data[j]!=buffer[j]){
         	flag=1;
         }
      }
      if(flag==1){
      	for(j=0;j<trans[i].length;j++){
         	trans[i].data[j]=buffer[j];
         }
      	msg.id = trans[i].idCan;
      	for(j=0;j<trans[i].length;j++){
         	msg.data[j]=buffer[j];
         }
      	msg.length = trans[i].length;
         test=can_send(msg);
         if(test==-1){
         	ret=-1;
         }
      }
   }
   return ret;
}



int update_values(can_event_msg_t msg, can_i2c_trans_t trans[], unsigned short length){

}












