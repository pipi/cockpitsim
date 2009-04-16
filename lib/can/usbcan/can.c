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
#include <stdio.h>

#include "Usbcan32.h"

#include <can.h>
 
static DWORD baudrates[] = {
  USBCAN_BAUD_1MBit,
  USBCAN_BAUD_800kBit,
  USBCAN_BAUD_500kBit,
  USBCAN_BAUD_250kBit,
  USBCAN_BAUD_125kBit,
  USBCAN_BAUD_100kBit,
  USBCAN_BAUD_50kBit,
  USBCAN_BAUD_20kBit,
  USBCAN_BAUD_10kBit
};
 
static tUcanHandle canHandle;
static short initialized = 0;
 
int can_init(unsigned short am, unsigned short ac, unsigned short baudrate) {
  UCANRET ret;
  tUcanInitCanParam params;
  
  ret = UcanInitHardware(&canHandle, USBCAN_ANY_MODULE, NULL);
  if(ret != 0) {
    return -1;
  }
  
  memset(&params, 0, sizeof(tUcanInitCanParam));
  params.m_dwSize = sizeof(params);
  params.m_bMode = kUcanModeNormal;
  params.m_bBTR0 = HIBYTE(baudrates[baudrate]);
  params.m_bBTR1 = LOBYTE(baudrates[baudrate]);
  params.m_bOCR = USBCAN_OCR_DEFAULT;
  params.m_dwAMR = (am << 16) | 0x0000FFFF; /* Id is on 11 bits and is left aligned ! */
  params.m_dwACR = (ac << 16) | 0x0000FFFF;
  params.m_dwBaudrate = USBCAN_BAUDEX_USE_BTR01;
  params.m_wNrOfRxBufferEntries = 100;
  params.m_wNrOfTxBufferEntries = 100;

  ret = UcanInitCanEx2(canHandle, USBCAN_CHANNEL_CH0, &params);
  if(ret != 0) {
    return -1;
  }

  ret = UcanResetCanEx(canHandle, USBCAN_CHANNEL_CH0, USBCAN_RESET_ONLY_ALL_BUFF);
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
  
  ucan_msg.m_dwID = msg.id >> 5; /* msg id is on 11 bits and is left aligned ! */
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

int can_recv(unsigned short timeout, can_event_msg_t* msg) {
  UCANRET ret;
  tCanMsgStruct ucan_msgs[1];
  DWORD count[1] = { 1 };
  BYTE chans[1] = { USBCAN_CHANNEL_CH0 };

  if(!initialized || msg == NULL) {
    return -1;
  }

  ret = UcanReadCanMsgEx(canHandle, chans, ucan_msgs, count);
  if(ret != 0) {
    if(ret == USBCAN_WARN_NODATA) {
      //printf("No data!\n");
    }
    memset(msg, 0, sizeof(can_event_msg_t));
    return -1;
  }

  /* msg id is left aligned and ucan_msg id is right aligned. */
  msg->id = ucan_msgs[0].m_dwID << 5;
  msg->length = ucan_msgs[0].m_bDLC;
  memcpy(msg->data, ucan_msgs[0].m_bData, 8);

  return 0;
}

void can_destroy() {
  initialized = 0;
  UcanDeinitCan(canHandle);
  UcanDeinitHardware(canHandle);
}
