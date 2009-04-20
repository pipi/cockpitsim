#ifndef _CABSTRACTOFFSETDATA_H_
#define _CABSTRACTOFFSETDATA_H_

#include <windows.h>

class CAbstractOffsetData {

public:

	virtual void update() = 0;
	virtual bool dataHasChanged() const = 0;
	virtual DWORD sizeInBytes() const = 0;
	virtual DWORD fillAsRawData(BYTE*) const = 0;
	virtual void setValueFromRawData(const BYTE*) = 0;

};

#endif
