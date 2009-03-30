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
 
#include <string.h>
#include <windows.h>

#include "Usbcan32.h"

#include "../can.h"
 
static DWORD baudrates[] = {
  USBCAN_BAUDEX_1MBit,
  USBCAN_BAUDEX_800kBit,
  USBCAN_BAUDEX_500kBit,
  USBCAN_BAUDEX_250kBit,
  USBCAN_BAUDEX_125kBit,
  USBCAN_BAUDEX_100kBit,
  USBCAN_BAUDEX_50kBit,
  USBCAN_BAUDEX_20kBit,
  USBCAN_BAUDEX_10kBit
};
 
static tUcanHandle canHandle;
static short initialized = 0;
 
int can_init(unsigned int am, unsigned int ac, unsigned short baudrate) {
  UCANRET ret;
  tUcanInitCanParam params;
  
  ret = UcanInitHardware(&canHandle, USBCAN_ANY_MODULE, NULL);
  if(ret != 0) {
    return ret;
  }
  
  params.m_dwSize = sizeof(params);
  params.m_bMode = kUcanModeNormal;
  params.m_bBTR0 = HIBYTE(baudrates[baudrate]);
  params.m_bBTR1 = LOBYTE(baudrates[baudrate]);
  params.m_bOCR = 0x1A;
  params.m_dwAMR = am >> 5; /* Id is on 11 bits and is left aligned ! */
  params.m_dwACR = ac >> 5;
  params.m_dwBaudrate = USBCAN_BAUDEX_USE_BTR01;
  params.m_wNrOfRxBufferEntries = 10;
  params.m_wNrOfTxBufferEntries = 10;

  ret = UcanInitCanEx(canHandle, &params);
  if(ret != 0) {
    return -1;
  }
  
  initialized = 1;
  return 0;
}

int can_send(can_event_msg_t msg) {
  UCANRET ret;
  tCanMsgStruct ucan_msg;

  if(!initialized) {
    return -1;
  }
  
  ucan_msg.m_dwID = msg.id >> 5; /* Id is on 11 bits and is left aligned ! */
  ucan_msg.m_bFF = USBCAN_MSG_FF_STD;
  ucan_msg.m_bDLC = msg.length;
  memcpy(ucan_msg.m_bData, msg.data, 8);
  ucan_msg.m_dwTime = 0;

  ret = UcanWriteCanMsg(canHandle, &ucan_msg);
  if(ret < 0) {
    return -1;
  }

  return 0;
}

int can_recv(unsigned int timeout, can_event_msg_t* msg) {
  UCANRET ret;
  tCanMsgStruct ucan_msg;

  if(!initialized || msg == NULL) {
    return -1;
  }

  ret = UcanReadCanMsg(canHandle, &ucan_msg);
  if(ret < 0) {
    memset(msg, 0, sizeof(can_event_msg_t));
    return -1;
  }

  msg->id = ucan_msg.m_dwID << 5;
  msg->length = ucan_msg.m_bDLC;
  memcpy(msg->data, ucan_msg.m_bData, 8);

  return 0;
}

void can_destroy() {
  initialized = 0;
  UcanDeinitCan(canHandle);
  UcanDeinitHardware(canHandle);
}
