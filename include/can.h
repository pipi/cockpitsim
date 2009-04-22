/*
 * Simulator project
 *
 * Institut de Formation des Ingénieurs de Paris-Sud
 * 2008-2009
 *
 * Sub-project : CAN library
 *
 * Authors : Julien Peeters <julien.peeters@u-psud.fr>
 *			    Fabien Provost <fabien.provost@u-psud.fr>
 *			    Feng Xiong <feng.xiong@u-psud.fr>
 *           Yongchao Xu <yongchao.xu@u-psud.fr>
 *
 * Creation date : 25/03/2009
 */

#ifndef _CAN_H_
#define _CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <windows.h>
#else // BECK
typedef unsigned short WORD;
typedef unsigned char BYTE;
#endif


/*!
 \file can.h
 \brief CAN library header file
*/

/*!
 * CAN event message
 */
typedef struct {
  //! The event id
  WORD id;
  //! The lenght of the data
  BYTE length;
  //! The data
  BYTE data[8];
} can_event_msg_t;

/*!
 * CAN event handler
 */
typedef struct {
   //! The CAN event id
 	unsigned short canId;
   //! The CAN event handler
   void (*handler)(can_event_msg_t);
} can_handler_t;

#define CAN_BAUDRATE_1M     0
#define CAN_BAUDRATE_800K   1
#define CAN_BAUDRATE_500K   2
#define CAN_BAUDRATE_250K   3
#define CAN_BAUDRATE_125K   4
#define CAN_BAUDRATE_100K   5
#define CAN_BAUDRATE_50K    6
#define CAN_BAUDRATE_20K    7
#define CAN_BAUDRATE_10K    8

/*!
 * \brief Intialize the CAN node with the given parameters
 *
 * ATTENTION : be careful of on how much bits unsigned
 * integers are represented.
 *
 * \param am acceptance mask
 * \param ac acceptance code
 * \param baudrate the bus baudrate
 *
 * \return 0 on success, -1 on errors
 */
int can_init(WORD am,
			 WORD ac,
             WORD baudrate);

/*!
 * \brief Send a CAN event on the bus.
 *
 * \param msg the can event message
 *
 * \return 0 on success, -1 on errors
 */
int can_send(can_event_msg_t msg);

/*!
 * \brief Receive a message from the bus
 *
 * REMARK : if the timeout is set to zero the function is blocking.
 *
 * \param timeout max delay after which the function returns if no message is pending
 * \param msg the CAN event message to be filled
 *
 * \return 0 on success, -1 on errors, -2 Time period expired and still no message available
 */
int can_recv(unsigned short timeout,
			 can_event_msg_t* msg);

/*!
 * \brief Close the CAN port.
 *
 */
void can_destroy(void);

#ifdef __cplusplus
};
#endif

#endif
