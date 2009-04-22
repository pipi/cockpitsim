#include <exception>

#include "CAbstractOffsetFamily.h"
#include "COffsetRegistry.h"

DWORD CAbstractOffsetFamily::totalSize() const {
	DWORD size = 0;

	for(std::list<CAbstractOffsetData*>::const_iterator it =
			m_lstData.begin(); it != m_lstData.end(); it++) {
		if(*it != NULL) {
			size += (*it)->sizeInBytes();
		}
	}

	return size;
}

CAbstractOffsetFamily::CAbstractOffsetFamily(WORD wCanId)
: m_wCanId(wCanId) { }

CAbstractOffsetFamily::~CAbstractOffsetFamily() {
	for(std::list<CAbstractOffsetData*>::iterator it =
		m_lstData.begin(); it != m_lstData.end(); it++) {
		if((*it) != NULL) {
			delete (*it);
		}
	}
}

bool CAbstractOffsetFamily::matchesCanId(WORD wCanId) const {
	return ((wCanId >> 8) == (m_wCanId >> 8));
}

bool CAbstractOffsetFamily::dataHasChanged() const {
	for(std::list<CAbstractOffsetData*>::const_iterator it =
			m_lstData.begin(); it != m_lstData.end(); it++) {
		if((*it)->dataHasChanged()) {
			return true;
		}
	}
	return false;
}

void CAbstractOffsetFamily::update() {
	for(std::list<CAbstractOffsetData*>::const_iterator it =
			m_lstData.begin(); it != m_lstData.end(); it++) {
		(*it)->update();
	}
}

const std::list<CAbstractOffsetData*>& CAbstractOffsetFamily::getData() const {
	return m_lstData;
}
