#ifndef _COFFSETFAMILY_H_
#define _COFFSETFAMILY_H_

#include <list>

#include <windows.h>

#include <can.h>

#include "Offsets.h"
#include "CAbstractOffsetData.h"

class CAbstractOffsetFamily {
	
protected:

	WORD m_wCanId;
	CConnector m_oConnector;
	std::list<CAbstractOffsetData*> m_lstData;

	static inline unsigned short dataIndexFromId(unsigned short uId) {
		return (uId >> 5) & 0x0007;
	}
	static inline unsigned short eventIdFromId(unsigned short uId) {
		return (uId >> 5) & 0xFFF8;
	}

	DWORD totalSize() const;

public:

	CAbstractOffsetFamily(CConnector&, WORD);
	virtual ~CAbstractOffsetFamily();

	bool matchesCanId(WORD) const;
	bool dataHasChanged() const;
	void update();
	const std::list<CAbstractOffsetData*>& getData() const;

	virtual can_event_msg_t transformToCanMessage() const = 0;
	virtual void setValueFromCanMessage(can_event_msg_t&) = 0;
};

#endif