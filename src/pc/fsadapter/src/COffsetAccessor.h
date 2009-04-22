/*
 * FS Simulator Cockpit Project
 * Date: 26/02/2009
 * Author: Julien Peeters <julien.peeters@u-psud.fr>
 * Version: 0.1
 *
 */

/* FSUIPC Offset Accessor.
 * Can read from and write to a FSUIPC offset with
 * an update time rate (threaded and thread safe).
 */

#ifndef _COFFSETACCESSOR_H_
#define _COFFSETACCESSOR_H_

#include <exception>

#include <windows.h>

#include "CConnector.h"
#include "CAbstractOffsetData.h"

template<typename T, DWORD dwOffset>
class COffsetAccessor: public CAbstractOffsetData {

	T m_lastFetchedData;

public:

	COffsetAccessor();

	T getValue() const;
	void setValue(const T&);
	
	void update();
	bool dataHasChanged() const;
	DWORD sizeInBytes() const;
	DWORD fillAsRawData(BYTE*) const;
	void setValueFromRawData(const BYTE*);

};

/* Offset accessor implementation. */

template<typename T, DWORD dwOffset>
COffsetAccessor<T, dwOffset>::COffsetAccessor() { 
	if(!CConnector::getInstance()->isOpened()) {
		throw std::exception("Error while instantiating offset accessor: "
							 "connector not opened.");
	}
	m_lastFetchedData = getValue();
}

template<typename T, DWORD dwOffset>
T COffsetAccessor<T, dwOffset>::getValue() const {
    T v;
	CConnector::getInstance()->read<T>(dwOffset, &v);
    return v;
}

template<typename T, DWORD dwOffset>
void COffsetAccessor<T, dwOffset>::setValue(const T& newValue) {
	CConnector::getInstance()->write<T>(dwOffset, &static_cast<T>(newValue));
	update();
}

template<typename T, DWORD dwOffset>
void COffsetAccessor<T, dwOffset>::update() {
	m_lastFetchedData = getValue();
}

template<typename T, DWORD dwOffset>
DWORD COffsetAccessor<T, dwOffset>::sizeInBytes() const {
	return sizeof(T);
}

template<typename T, DWORD dwOffset>
bool COffsetAccessor<T, dwOffset>::dataHasChanged() const {
	return (getValue() == m_lastFetchedData);
}

template<typename T, DWORD dwOffset>
DWORD COffsetAccessor<T, dwOffset>::fillAsRawData(BYTE* buffer) const {
	T value = getValue();
	DWORD length = sizeInBytes();

	memcpy(buffer, &value, length);
	return length;
}

template<typename T, DWORD dwOffset>
void COffsetAccessor<T, dwOffset>::setValueFromRawData(const BYTE* buffer) {
	T newValue;
	memcpy(&newValue, buffer, sizeInBytes());
	setValue(newValue);
}

#endif
