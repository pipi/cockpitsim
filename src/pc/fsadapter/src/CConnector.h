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

/*!
 * \class CConnector
 *
 * \brief Singleton which offers to interact with
 * FSUIPC.
 */
class CConnector {

	DWORD m_dwResult;
	bool m_bOpened;

	static CConnector* m_oInstance;

	CConnector();
	CConnector(const CConnector&) { }
	CConnector& operator=(const CConnector&) { }

public:

	enum FSVersion { ANY=0, FS98, FS2000, CFS2, 
					 CFS1, FLY, FS2002, FS2004, FSX, ESP };
	~CConnector();

	/*!
	 * \brief Open the connection with FSUIPC.
	 *
	 * \param version the version of FlightSimulator.
	 *
	 * \see FSVersion
	 *
	 * \return false if the connection fails, true otherwise.
	 */
	bool open(FSVersion = ANY);
	/*!
	 * \brief Test if the connection with FSUIPC is opened.
	 *
	 * \return true if the connection is opened, false otherwise.
	 */
	bool isOpened() const;
	/*!
	 * \brief Close the connection with FSUIPC.
	 */
	void close();

	/*!
	 * \brief Read offset value from FSUIPC.
	 *
	 * \tparam T the type of the offset (must be the exact alignment).
	 *
	 * \param dwOffset the internal FSUIPC address of the offset.
	 * \param pDest a pointer to the data to be filled.
	 * \param bAutoProcess auto process the read request.
	 *
	 * \return true if the reading succeeds, false otherwise.
	 */
	template<typename T>
	bool read(DWORD, T*, bool = true) const;
	/*!
	 * \brief Write offset value to FSUIPC.
	 *
	 * \tparam T the type of the offset (must be the exact alignment).
	 *
	 * \param dwOffset the internal FSUIPC address of the offset.
	 * \param pSrc a pointer to the data to be read.
	 * \param bAutoProcess auto process the write request.
	 *
	 * \return true if the writing succeeds, false otherwise.
	 */
	template<typename T>
	bool write(DWORD, T*, bool = true);
	
	/*!
	 * \brief Process pending requests.
	 *
	 * \note It is possible to enqueue several requests before processing
	 * them.
	 */
	bool process() const;

	/*!
	 * \brief Get the unique instance of the singleton.
	 */
	static CConnector* getInstance();
	/*!
	 * \brief Destroy the unique instance of the singleton.
	 */
	static void destroyInstance();

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
