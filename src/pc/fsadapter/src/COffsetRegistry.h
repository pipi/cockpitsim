#ifndef _COFFSETREGISTRY_H_
#define _COFFSETREGISTRY_H_

#include <map>

#include <windows.h>

#include "CAbstractOffsetData.h"

class COffsetRegistry {

	std::map<DWORD, CAbstractOffsetData*> m_mapOffsets;

	static COffsetRegistry* m_oInstance;

protected:

	COffsetRegistry();

public:

	virtual ~COffsetRegistry();

	void registerOffset(DWORD, CAbstractOffsetData*);
	void unregisterOffset(DWORD);

	CAbstractOffsetData* getData(DWORD) const;

	static COffsetRegistry* getInstance();
	static void destroyInstance();

};

#endif
