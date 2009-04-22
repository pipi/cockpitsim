#include <iostream>
#include <exception>

#include <windows.h>

#include "CConnector.h"
#include "Offsets.h"
#include "Families.h"
#include "CFsAdapter.h"

int main() {

	try {
		CConnector oConnector;
		CCanNodeConfig oConfig = { 0xFFFF, 0xFFFF, CAN_BAUDRATE_1M };
		CFsAdapter oAdapter(oConfig, oConnector);

		oAdapter.addFamily(new CAutoPilotFamily(oConnector, 0x8000));
	} catch(std::exception& ex) {
#ifdef _DEBUG
		std::cerr << ">>> Exception raised <<<" << std::endl << ex.what()
			<< std::endl;
#endif
	}

	return 0;
}
