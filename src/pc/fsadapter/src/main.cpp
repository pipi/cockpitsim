#include <iostream>

#include <windows.h>

#include "CConnector.h"
#include "Offsets.h"

int main() {
	
	CConnector oConnector;
    if(oConnector.open()) {
	
	    AltitudeOffset altitude(oConnector);
        VertSpeedOffset vertSpeed(oConnector);
        while(true) {
            Sleep(50);
            std::cout << "Altitude = " << (altitude.getValue() * 3.28084) << " ft | ";
            std::cout << "Speed = " << (vertSpeed.getValue() * 3.28084) << " ft/s" << std::endl;
        }
    } else {
        std::cout << "Erreur à l'ouverture du connecteur" << std::endl;
    }

	oConnector.close();

}
