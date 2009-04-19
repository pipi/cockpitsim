#ifndef _CFSADATPER_H_
#define _CFSADAPTER_H_

#include <list>

#include "CConnector.h"
#include "CAbstractOffsetFamily.h"

struct CCanNodeConfig {
	WORD m_wAcceptanceMask;
	WORD m_wAcceptanceCode;
	WORD m_wBaudrate;
};

class CFsAdapter {

	CConnector m_oConnector;
	std::list<CAbstractOffsetFamily*> m_lstFamilies;

	void lookupForCanMessages();
	void lookupForFsuipcChanges();

public:

	CFsAdapter(CCanNodeConfig&, CConnector&);
	~CFsAdapter();

	void addFamily(CAbstractOffsetFamily*);
	void removeFamily(CAbstractOffsetFamily*);

	void mainLoop();

};

#endif