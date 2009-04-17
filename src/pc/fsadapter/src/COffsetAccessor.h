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

#ifndef _COFFSET_ACCESSOR_H_
#define _COFFSET_ACCESSOR_H_

#include <iostream>

#include <windows.h>

#include "CConnector.h"

template<typename T, DWORD dwOffset>
class COffsetAccessor {

	CConnector m_oConnector;

public:

	COffsetAccessor(CConnector&);
	//virtual ~COffsetAccessor();

	T getValue() const;
	void setValue(const T&);

};

/* Offset accessor implementation. */

template<typename T, DWORD dwOffset>
COffsetAccessor<T, dwOffset>::COffsetAccessor(CConnector& oConnector)
: m_oConnector(oConnector) { }

/*template<typename T, DWORD dwOffset>
COffsetAccessor<T, dwOffset>::~COffsetAccessor() {
	m_bRunning = false;
}*/

template<typename T, DWORD dwOffset>
T COffsetAccessor<T, dwOffset>::getValue() const {
    T v;
    m_oConnector.read<T>(dwOffset, &v);
    return v;
}

template<typename T, DWORD dwOffset>
void COffsetAccessor<T, dwOffset>::setValue(const T& newValue) {
	oConnector.write<T>(dwOffset, &newValue);
}

#endif
