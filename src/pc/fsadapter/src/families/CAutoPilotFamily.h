#ifndef _CAUTOPILOTFAMILY_H_
#define _CAUTOPILOTFAMILY_H_

#include "../CAbstractOffsetFamily.h"

class CAutoPilotFamily: public CAbstractOffsetFamily {

public:

	CAutoPilotFamily(CConnector&, WORD);

	can_event_msg_t transformToCanMessage() const;
	void setValueFromCanMessage(can_event_msg_t&);

};

#endif
