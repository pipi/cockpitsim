/*
 * Simulator project
 *
 * Institut de Formation des Ing√©nieurs de Paris-Sud
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

 /*!
  \file can-i2c.h
  \brief library to translate CAN messages to I≤C ones.
 */

#ifndef _CAN_I2C_H_
#define _CAN_I2C_H_

/*!
 * CAN-I2C translation entry.
 */

typedef struct {
   //! The CAN event id
 	unsigned short idCan;
   //! The I2C node id
   unsigned char idI2C; //WARNING it is the read mode 
   //! the last value
   char data[8];
   //! The data length
   unsigned short length;
} can_i2c_trans_t;


/*!
 * Check and send changes.
 * \brief This function checks if there is such modifications of values
 * for each I≤C node and a CAN message if necessary.
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

