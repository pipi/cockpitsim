#include "CAutoPilotFamily.h"

CAutoPilotFamily::CAutoPilotFamily(CConnector& oConnector, DWORD dwCanId)
: CAbstractOffsetFamily(oConnector, dwCanId) { 
	m_lstData.push_back(new AltitudeOffset(oConnector));
	// Adding other offsets.
	// ...
}

can_event_msg_t CAutoPilotFamily::transformToCanMessage() const {
	DWORD filled = 0;
	can_event_msg_t msg;

	for(std::list<CAbstractOffsetData*>::const_iterator it =
			m_lstData.begin(); filled < 8 && it != m_lstData.end(); it++) {
		filled += (*it)->fillAsRawData(msg.data+filled);
	}
	msg.length = filled;
	msg.id = m_dwCanId;

	return msg;
}

void CAutoPilotFamily::setValueFromCanMessage(can_event_msg_t& oCanEventMsg) {
	for(std::list<CAbstractOffsetData*>::iterator it =
			m_lstData.begin(); it != m_lstData.end(); it++) {
		(*it)->setValueFromRawData(oCanEventMsg.data);
	}
}
