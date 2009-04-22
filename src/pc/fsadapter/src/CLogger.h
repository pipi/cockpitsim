#ifndef _LOGGER_H_
#define _LOGGER_H_

class CLogger {

public:

	enum LogType { LINFO, LDEBUG, LWARNING, LERROR };

	static std::ostream& log(LogType);

};

#endif
