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

 /*!
  \file can-i2c.h
  \brief library to translate CAN messages to I²C ones.
 */

#ifndef _CAN_I2C_H_
#define _CAN_I2C_H_

typedef struct _i2c_node_t {
	//! I2C node address.
	unsigned char nodeAddr;
   //! I2C node last data.
   unsigned char lastData[8];
	//! I2C node data length.
   unsigned char dataLength;
   //! I2C node data retreiving function.
   int (*update_data_from_node)(struct _i2c_node_t*);
   //! I2C node data update from CAN message
   int (*update_data_from_can)(struct _i2c_node_t*, can_event_msg_t*);
   //! fills data array in order to build a CAN message.
   int (*fillBuffer)(struct _i2c_node_t*, unsigned char*);
} i2c_node_t;

/*!
 * CAN-I2C translation entry.
 */

typedef struct {
	//! The CAN id.
   unsigned short canId;
   //! The CAN least message data
   unsigned char leastData[8];
   //! The least data length
   unsigned char dataLength;
   //! Data has changed flag.
   unsigned char dataHasChanged;
	//! The I2C nodes array.
   i2c_node_t* i2cNodes;
   //! Number of I2C nodes.
   unsigned char nodesLength;
} can_family_t;

/*!
 * Check and send changes.
 * \brief This function checks if there is such modifications of values
 * for each I²C node linked to an unique CAN message and send the
 * corresponding CAN message if necessary.
 *
 * \param trans translation array between I2C and CAN
 * \param length the array length
 * \return 0 on success, -1 on errors
 */
/*int i2c_send_changes(i2c_can_trans_t trans[],
					  		unsigned short length);*/


/*!
 * Check and send changes of FPGA.
 * \brief This function checks if there is such modifications of values
 * for I²C node, especially FPGA, linked to an unique CAN message and send the
 * corresponding CAN message if necessary.
 *
 * \param trans translation array for FPGA between I2C and CAN
 * \param length the array length
 * \return 0 on success, -1 on errors
 */
/*int fpga_send_changes(i2c_can_trans_t trans[],
					  		unsigned short length);*/

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
/*int update_values(can_event_msg_t msg,
						i2c_can_trans_t trans[],
                  unsigned short length);*/


/*!
 * Decode of keyboard.
 * \brief This function checks if there is any press on the keyboard, decodes
 * and then returns the value of this touch.
 *
 * \param slave the slave node address
 * \param tab array for keyboard value
 * \return 0 no touch, -1 on errors, or value of the key touched
 */
/*int keyboard_decode(unsigned char slave,
						  unsigned int tab[8][9]);*/

#endif

