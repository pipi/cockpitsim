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
#ifdef _DEBUG
		std::cerr << ">>> Exception raised <<<" << std::endl << ex.what()
			<< std::endl;
#endif
	}

	return 0;
}
