#include <exception>

#include <can.h>

#include "CFsAdapter.h"

void CFsAdapter::lookupForCanMessages() {
	can_event_msg_t msg;
	while(can_recv(1, &msg) == 0) { // empty the input queue.
		for(std::list<CAbstractOffsetFamily*>::iterator it =
				m_lstFamilies.begin(); it != m_lstFamilies.end(); it++) {
			if((*it)->matchesCanId(msg.id)) {
				(*it)->setValueFromCanMessage(msg);
				// Because Id <-> Family link is unique.
				break;
			}
		}
	}
}

void CFsAdapter::lookupForFsuipcChanges() {
	for(std::list<CAbstractOffsetFamily*>::iterator it =
			m_lstFamilies.begin(); it != m_lstFamilies.end(); it++) {
		if((*it)->dataHasChanged()) {
			(*it)->update();
			
			can_event_msg_t msg = (*it)->transformToCanMessage();
			can_send(msg);
		}
	}
}

CFsAdapter::CFsAdapter(CCanNodeConfig& oNodeConfig,
					   CConnector& oConnector): m_oConnector(oConnector) {
	if(!m_oConnector.isOpened() && !m_oConnector.open()) {
		throw new std::exception("Error while opening FSUIPC connection.");
	}
	if(can_init(oNodeConfig.m_wAcceptanceMask, 
			oNodeConfig.m_wAcceptanceCode, oNodeConfig.m_wBaudrate) != 0) {
		throw new std::exception("Error while initializing CAN.");
	}
}

CFsAdapter::~CFsAdapter() {
	m_oConnector.close();
}

void CFsAdapter::addFamily(CAbstractOffsetFamily* pFamily) {
	m_lstFamilies.push_front(pFamily);
}

void CFsAdapter::removeFamily(CAbstractOffsetFamily* pFamily) {
	m_lstFamilies.remove(pFamily);
}

void CFsAdapter::mainLoop() {
	// Need to be improved!!
	while(1) {
		lookupForCanMessages();
		lookupForFsuipcChanges();
	}
}
