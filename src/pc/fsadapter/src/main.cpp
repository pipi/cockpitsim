#include <iostream>

#include <windows.h>

#include "CConnector.h"
#include "Offsets.h"
#include "CFsAdapter.h"

int main() {

	CConnector oConnector;
	CCanNodeConfig oConfig = { 0xFFFF, 0xFFFF, CAN_BAUDRATE_1M };
	CFsAdapter oAdapter(oConfig, oConnector);

	return 0;
}
