#include "CAutoPilotFamily.h"

CAutoPilotFamily::CAutoPilotFamily(WORD wCanId)
: CAbstractOffsetFamily(wCanId) {
  m_lstData.push_back(new AltitudeHoldOffset());
	m_lstData.push_back(new AltitudeOffset());
	// Adding other offsets.
	// ...
}

can_event_msg_t CAutoPilotFamily::transformToCanMessage() const {
	BYTE filled = 0;
	can_event_msg_t msg;

	for(std::list<CAbstractOffsetData*>::const_iterator it =
			m_lstData.begin(); filled < 8 && it != m_lstData.end(); it++) {
		filled += (*it)->fillAsRawData(msg.data+filled);
	}
	msg.length = filled;
	msg.id = m_wCanId;

	return msg;
}

void CAutoPilotFamily::setValueFromCanMessage(can_event_msg_t& oCanEventMsg) {
	unsigned int index = 0;

  for(std::list<CAbstractOffsetData*>::iterator it =
			m_lstData.begin(); it != m_lstData.end(); it++) {
		(*it)->setValueFromRawData(oCanEventMsg.data+index);
    index += (*it)->sizeInBytes();
	}
}
