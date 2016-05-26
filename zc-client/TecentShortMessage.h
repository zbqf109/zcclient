#pragma once
#include "Sms.h"

class IMatch
{
public:
	virtual BOOL Match() = 0;
};

class CTecentShortMessage :
	public CSms, IMatch
{
public:
	CTecentShortMessage(const CString &text);
	~CTecentShortMessage();

public:
	virtual BOOL Match();

private:
	CArray <CString> m_arrayKeywords;
};

