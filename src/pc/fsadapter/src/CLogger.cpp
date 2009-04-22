#include <iostream>

#include "CLogger.h"

std::ostream& CLogger::log(CLogger::LogType type) {
	switch(type) {
		case CLogger::LINFO: return std::cout << "[ INFO ]";
		case CLogger::LDEBUG: return std::cout << "[ DEBUG ]";
		case CLogger::LWARNING: return std::cerr << "[ WARNING ]";
		case CLogger::LERROR: return std::cerr << "[ ERROR ]";
		default: return std::cout;
	}
}
