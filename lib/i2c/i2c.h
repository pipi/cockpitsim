/*
 * Simulator project
 *
 * Institut de Formation des Ing√©nieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : I2C library
 *
 * Authors : Julien Peeters <julien.peeters@u-psud.fr>
 *			    Fabien Provost <fabien.provost@u-psud.fr>
 *			    Feng Xiong <feng.xiong@u-psud.fr>
 *           Yongchao Xu <yongchao.xu@u-psud.fr>
 *
 * Creation date : 02/04/2009
 */

 /*!
  \file i2c.h
  \brief library to translate CAN messages to I≤C ones.
 */


#ifndef _I2C_H_
#define _I2C_H_

#include <I2C_API.h>

/*!
 * \brief I2C bus initialization.
 *
 * \return 0 on success, -1 on failure.
 */
int i2c_init(void);

/*!
 * \brief Write data on the I2C bus.
 *
 * \param slave the slave node address.
 * \param buffer the data buffer to send.
 * \param length the length of data to send.
 *
 * \return 0 on success, -1 on failure.
 */
int i2c_write(unsigned char slave,
				  const char far *buffer,
              int length);

/*!
 * \brief Read data from the I2C bus.
 *
 * \param slave the slave node address.
 * \param buffer the data buffer to fill.
 * \param length the length of data to read.
 *
 * \return 0 on success, -1 on failure.
 */
int i2c_read(unsigned char slave,
				 char far *buffer,
             int length);

/*!
 * \brief Close the I2C bus.
 *
 */
void i2c_destroy(void);

#endif

