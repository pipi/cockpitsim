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

#ifdef DEBUG
#include <iostream>
#endif

/*!
 * \class COffsetAccessor
 * \brief Offers a generic way to interact with FSUIPC offsets.
 *
 * \see COffsetAccessor::getValue
 * \see COffsetAccessor::setValue
 * \see CConnector
 *
 * \tparam T the type of the offset value (must respect the offset alignment).
 * \tparam dwOffset the FSUIPC internal address of the offset.
 */
template<typename T, DWORD dwOffset>
class COffsetAccessor: public CAbstractOffsetData {

	T m_lastFetchedData;
	float m_fMultiple;

public:

	/*!
	 * Constructor
	 * \brief Constructor with optionnal multiplier parameter.
	 *
	 * \param fMultiple the multiplier which transform data from
	 * and to FSUIPC.
	 */
	COffsetAccessor(float = 1.0f);

	/*!
	 * \brief Get the offset value from FSUIPC.
	 *
	 * Get the offset value from FSUIPC and transform it with the
	 * multiplier before return it.
	 *
	 * \return the value of the offset after applying the multiplier.
	 */
	T getValue() const;
	/*!
	 * \brief Set the offset value to FSUIPC.
	 *
	 * Set the offset value to FSUIPC after transforming it with the
	 * multiplier. The call to this function updates the local value.
	 *
	 * \see COffsetAccessor::update
	 *
	 * \param newValue the new offset value to set.
	 */
	void setValue(const T&);
	/*!
	 * \brief Update the local offset value from FSUIPC.
	 *
	 * \remarks This function must be call every time you want to
	 * refresh the local value of the offset from FSUIPC.
	 */
	void update();
	/*!
	 * \brief Test if data has changed.
	 *
	 * Test if the data has changed since the last update.
	 *
	 * \see CAbstractOffsetFamily::update
	 *
	 * \return true if data has changed, false otherwise.
	 */
	bool dataHasChanged() const;
	/*!
	 * \brief Returns the size in byte of the offset value.
	 *
	 * \return the size in bytes of the offset value.
	 */
	DWORD sizeInBytes() const;
	/*!
	 * \brief Fills the buffer with the local value of the offset.
	 *
	 * \remarks If you want to fil the the buffer with the least
	 * FSUIPC value you have to call the update method.
	 *
	 * \param buffer the buffer to fill with local offset value.
	 *
	 * \return the number of written bytes.
	 */
	DWORD fillAsRawData(BYTE*) const;
	/*!
	 * \brief Set the offset value with the data contained in
	 * the buffer.
	 *
	 * \note This method use the setValue method.
	 *
	 * \param buffer the buffer from which the data is read.
	 */
	void setValueFromRawData(const BYTE*);

};

/* Offset accessor implementation. */

template<typename T, DWORD dwOffset>
COffsetAccessor<T, dwOffset>::COffsetAccessor(float fMultiple)
: m_fMultiple(fMultiple) { 
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

	v = static_cast<T>(v / m_fMultiple);

#ifdef DEBUG
	std::cout << "Value got from offset " << dwOffset << " : "
		<< v << std::endl;
#endif

    return v;
}

template<typename T, DWORD dwOffset>
void COffsetAccessor<T, dwOffset>::setValue(const T& newValue) {
	T v = newValue * m_fMultiple;
	CConnector::getInstance()->write<T>(dwOffset, &v);

#ifdef DEBUG
	std::cout << "Value set to offset " << dwOffset << " : "
		<< v << std::endl;
#endif

	update();
}

template<typename T, DWORD dwOffset>
void COffsetAccessor<T, dwOffset>::update() {
	m_lastFetchedData = getValue();

#ifdef DEBUG
	std::cout << "Value of offset " << dwOffset << " updated to "
		<< m_lastFetchedData << std::endl;
#endif
}

template<typename T, DWORD dwOffset>
DWORD COffsetAccessor<T, dwOffset>::sizeInBytes() const {
	return sizeof(T);
}

template<typename T, DWORD dwOffset>
bool COffsetAccessor<T, dwOffset>::dataHasChanged() const {
	return (getValue() != m_lastFetchedData);
}

template<typename T, DWORD dwOffset>
DWORD COffsetAccessor<T, dwOffset>::fillAsRawData(BYTE* buffer) const {
	DWORD length = sizeInBytes();

#ifdef DEBUG
	std::cout << "Fill buffer with value 0x" << std::hex
		<< m_lastFetchedData << " for offset 0x" << std::hex 
		<< dwOffset << std::endl;
#endif

	memcpy(buffer, &m_lastFetchedData, length);

	return length;
}

template<typename T, DWORD dwOffset>
void COffsetAccessor<T, dwOffset>::setValueFromRawData(const BYTE* buffer) {
	T newValue;
	memcpy(&newValue, buffer, sizeInBytes());

#ifdef DEBUG
	std::cout << "Set offset 0x" << std::hex << dwOffset 
		<< " from raw data " << newValue << std::endl;
#endif

	setValue(newValue);
}

#endif
