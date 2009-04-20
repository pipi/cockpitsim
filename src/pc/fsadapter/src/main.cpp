#include <iostream>
#include <exception>

#include <windows.h>

#include "CConnector.h"
#include "Offsets.h"
#include "CFsAdapter.h"

int main() {

	try {
		CConnector oConnector;
		CCanNodeConfig oConfig = { 0xFFFF, 0xFFFF, CAN_BAUDRATE_1M };
		CFsAdapter oAdapter(oConfig, oConnector);
	} catch(std::exception& ex) {
		std::cerr << ">>> Exception raised <<<" << std::endl << ex.what()
			<< std::endl;
	}

	return 0;
}
