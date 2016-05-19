#pragma once
#include <WinInet.h>

class CMyUrl
{
public:
	CMyUrl(const CString &sUrl);
	~CMyUrl();

	const CString &GetDomain() const
	{
		return m_sDomain;
	}

	INTERNET_PORT GetPort() const
	{
		return m_Port;
	}

	const CString &GetScheme() const
	{
		return m_sScheme;
	}

	const CString &GetPath() const
	{
		return m_sPath;
	}

private:
	CString m_sScheme;
	CString m_sDomain;
	CString m_sPath;
	CString m_sQuery;
	INTERNET_PORT m_Port;

};

