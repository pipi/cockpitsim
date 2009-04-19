#include "COffsetRegistry.h"

COffsetRegistry* COffsetRegistry::m_oInstance = NULL;

COffsetRegistry::COffsetRegistry() { }

COffsetRegistry::~COffsetRegistry() { }

void COffsetRegistry::registerOffset(DWORD dwOffset,
									 CAbstractOffsetData* pData) {
	m_mapOffsets[dwOffset] = pData;
}

void COffsetRegistry::unregisterOffset(DWORD dwOffset) {
	std::map<DWORD, CAbstractOffsetData*>::iterator it =
		m_mapOffsets.find(dwOffset);
	if(it != m_mapOffsets.end()) {
		m_mapOffsets.erase(it);
	}
}

CAbstractOffsetData* COffsetRegistry::getData(DWORD dwOffset) const {
	std::map<DWORD, CAbstractOffsetData*>::const_iterator it =
		m_mapOffsets.find(dwOffset);
	if(it != m_mapOffsets.end()) {
		return it->second;
	}
	return NULL;
}

COffsetRegistry* COffsetRegistry::getInstance() {
	if(m_oInstance == NULL) {
		m_oInstance = new COffsetRegistry();
	}
	return m_oInstance;
}

void COffsetRegistry::destroyInstance() {
	if(m_oInstance != NULL) {

		for(std::map<DWORD, CAbstractOffsetData*>::iterator it = 
				m_oInstance->m_mapOffsets.begin(); 
			it != m_oInstance->m_mapOffsets.end(); it++) {
			
			if(it->second != NULL) {
				delete it->second;
			}
		}

		delete m_oInstance;
	}
}

