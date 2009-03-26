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
  params.m_dwAMR = am;
  params.m_dwACR = ac;
  params.m_dwBaudrate = USBCAN_BAUDEX_USE_BTR01;
  params.m_wNrOfRxBufferEntries = 10;
  params.m_wNrOfTxBufferEntries = 10;
  
  ret = UcanInitCanEx(canHandle, &params);
  if(ret != 0) {
    return ret;
  }
  
  return 0;
}
