/*
 * FS Simulator Cockpit Project
 * Date: 26/02/2009
 * Author: Julien Peeters <julien.peeters@u-psud.fr>
 * Version: 0.1
 *
 */

/* FSUIPC Connector.
 * Can read from and write to FSUIPC offsets.
 */

#ifndef _CCONNECTOR_H_
#define _CCONNECTOR_H_

#include <windows.h>

#include "FSUIPC_User.h"

class CConnector {

	DWORD m_dwResult;
	bool m_bOpened;

public:

	enum FSVersion { ANY=0, FS98, FS2000, CFS2, 
					 CFS1, FLY, FS2002, FS2004, FSX, ESP };
	
	CConnector();
	~CConnector();

	bool open(FSVersion = ANY);
	bool isOpened() const;
	void close();

	template<typename T>
	bool read(DWORD, T*, bool = true) const;
	template<typename T>
	bool write(DWORD, T*, bool = true);
	
	bool process() const;

};

/* Templates implemenations. */

template<typename T>
bool CConnector::read(DWORD dwOffset, T* pDest, bool bAutoProcess) const {
	if(FSUIPC_Read(dwOffset, sizeof(T), pDest, &static_cast<DWORD>(m_dwResult))) {
		if(bAutoProcess) {
			return process();
		}
		return true;
	}
	return false;
}

template<typename T>
bool CConnector::write(DWORD dwOffset, T* pSrc, bool bAutoProcess) {
	if(FSUIPC_Write(dwOffset, sizeof(T), pSrc, &m_dwResult)) {
		if(bAutoProcess) {
			return process();
		}
		return true;
	}
	return false;
}

#endif
