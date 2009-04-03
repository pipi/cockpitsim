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
  \file can-i2c.h
  \brief library to translate can messages to i2c messages
 */
/*!
 translation table
 */

#ifndef _CAN_I2C_H_
#define _CAN_I2C_H_

typedef struct{
   //! The CAN event id
 	unsigned short idCan;
   //! The I2C node id
   unsigned char idI2C;//WARNING it is the read mode 
   //! the last value
   char data[8];
   //! The data length
   unsigned short length;

} can_i2c_trans_t;


 /*!
 * Check changes.
 * \brief this function check if there is a modification of value
 * for each i2c node then if there is a modification it sends a can message
 *
 * \param trans translation array between I2C and CAN
 * \param length the array length
 * \return 0 on success, -1 on errors
 */
int send_changes(can_i2c_trans_t trans[],
					  unsigned short length);


/*!
 * Interpretation of CAN message.
 * \brief this function send a I2C message after it
 * receives a CAN message
 *
 * \param msg CAN message received
 * \param transk translation array between I2C and CAN
 * \param length the array length
 * \return 0 on success, -1 on errors
 */
int update_values(can_event_msg_t msg,
						can_i2c_trans_t trans[],
                  unsigned short length);

#endif
