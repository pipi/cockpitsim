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
 * Creation date : 25/03/2009
 */
 
 /*! 
  \file can.h
  \brief CAN library header file
 */
 
 /*!
 CAN event message
 */
typedef struct {
  //! The event id
  unsigned int id;
  //! The lenght of the data
  unsigned char length;
  //! The data
  unsigned char data[8];
} can_event_msg_t;

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
 * CAN initialization.
 * \brief Intialize the CAN node with the given parameters.
 *
 * \param am acceptance mask
 * \param ac acceptance code
 * \param baudrate the bus baudrate
 * \return 0 on success, -1 on errors
 */
int can_init(unsigned int am, unsigned int ac, unsigned short baudrate);

/*!
 * CAN event message sending
 * \brief Send a CAN event on the bus.
 *
 * \param msg the can event message
 * \return 0 on success, -1 on errors
 */
int can_send(can_event_msg_t msg);

/*!
 * CAN event receiving
 * \brief Receive a message on the bus
 *
 * \param timeout max delay after which the function returns if no message is pending
 * \param msg the CAN event message to be filled
 * \return 0 on success, -1 on errors
 */
int can_recv(unsigned int timeout, can_event_msg_t* msg);



/*!
 * close CAN port
 * \brief close the CAN port.
 *
 *
 *
 */
void can_destroy(void);
