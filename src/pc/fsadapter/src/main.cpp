#include <iostream>
#include <exception>

#include <windows.h>

#include "CConnector.h"
#include "Offsets.h"
#include "Families.h"
#include "CFsAdapter.h"

int main() {

	try {
		CCanNodeConfig oConfig = { 0xFFFF, 0xFFFF, CAN_BAUDRATE_1M };
		CFsAdapter oAdapter(oConfig);

		oAdapter.addFamily(new CAutoPilotFamily(0x8000));
	} catch(std::exception& ex) {
#ifdef DEBUG
		std::cerr << ">>> Exception raised <<<" << std::endl << ex.what()
			<< std::endl;
#endif
	}

	return 0;
}
