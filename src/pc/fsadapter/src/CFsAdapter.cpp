#include <exception>

#include <can.h>

#include "CFsAdapter.h"

#ifdef DEBUG
#include <iostream>
#endif

void CFsAdapter::lookupForCanMessages() {
	can_event_msg_t msg;

#ifdef DEBUG
	std::cout << "Lookup for CAN messages..." << std::endl;
	unsigned int msgCnt = 0;
#endif

	while(can_recv(1, &msg) == 0) { // empty the input queue.
		
#ifdef DEBUG
		std::cout << "CAN message received with id " 
			<< msg.id << "." << std::endl;
		msgCnt++;
#endif

		for(std::list<CAbstractOffsetFamily*>::iterator it =
				m_lstFamilies.begin(); it != m_lstFamilies.end(); it++) {
			if((*it)->matchesCanId(msg.id)) {

#ifdef DEBUG
				std::cout << "Family found." << std::endl;
#endif

				(*it)->setValueFromCanMessage(msg);
				// Because Id <-> Family link is unique.
				break;
			}
		}
	}
#ifdef DEBUG
	std::cout << "No more messages (" << msgCnt << " messages treated)."
		<< std::endl;
#endif
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

CFsAdapter::CFsAdapter(CCanNodeConfig& oNodeConfig) {
	if(!CConnector::getInstance()->isOpened() && 
			!CConnector::getInstance()->open()) {
		throw std::exception("Error while opening FSUIPC connection.");
	}
#ifdef DEBUG
	std::cout << "FSUIPC connection opened." << std::endl;
#endif
	if(can_init(oNodeConfig.m_wAcceptanceMask, 
			oNodeConfig.m_wAcceptanceCode, oNodeConfig.m_wBaudrate) != 0) {
		throw std::exception("Error while initializing CAN.");
	}
#ifdef DEBUG
	std::cout << "CAN connection opened." << std::endl;
#endif
}

CFsAdapter::~CFsAdapter() {
	for(std::list<CAbstractOffsetFamily*>::iterator it =
			m_lstFamilies.begin(); it != m_lstFamilies.end(); it++) {
		if((*it) != NULL) {
			delete (*it);
		}
	}
	can_destroy();
	CConnector::destroyInstance();
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
