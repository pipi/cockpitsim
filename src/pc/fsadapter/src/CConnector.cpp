#include "CConnector.h"

CConnector::CConnector(): m_dwResult(0), m_bOpened(false) { }

CConnector::~CConnector() {
	close();
}

bool CConnector::open(CConnector::FSVersion version) {
	if(FSUIPC_Open(version, &m_dwResult)) {
		m_bOpened = true;
		return true;
	}
	return false;
}

void CConnector::close() {
	FSUIPC_Close();
	m_bOpened = false;
}

bool CConnector::isOpened() const {
	return m_bOpened;
}

bool CConnector::process() const {
	return FSUIPC_Process(&static_cast<DWORD>(m_dwResult));
}
