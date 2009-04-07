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

#include "../../lib/can/can.h"
#include "../../lib/i2c/i2c.h"
#include "../../lib/can-i2c.h"

#define mask_ID_CAN 0xFFE0

int send_changes(can_i2c_trans_t trans[],
					  unsigned short length){
   char buffer[8];
   can_event_msg_t msg;
	int i, j, ret;

   ret = 0;
   for(i = 0; i < length; i++){
   	i2c_read(trans[i].idI2C, buffer, trans[i].length);
      for(j = 0; j < trans[i].length; j++){
      	if(trans[i].data[j] != buffer[j]){
         	memcpy(trans[i].data, buffer, trans[i].length);
         	memcpy(msg.data, buffer, trans[i].length);
         	msg.id = trans[i].idCan;
      		msg.length = trans[i].length;
         	if(can_send(msg) == -1) {;
            	ret = -1;
         	}
         }
      }
   }
   
   return ret;
}



int update_values(can_event_msg_t msg,
						can_i2c_trans_t trans[],
                  unsigned short length_tab){

   int i;

   for(i = 0; i < length_tab; i++){
   	if((trans[i].idCan & mask_ID_CAN) == (msg.id & mask_ID_CAN)){
      	if(i2c_write(trans[i].idI2C, msg.data, msg.length) != 0) {
         	return -1;
         }
         trans[i].length = msg.length;
         memcpy(trans[i].data, msg.data, msg.length);
      }
   }

   return 0;
}












