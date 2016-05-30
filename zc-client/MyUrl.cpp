#include "stdafx.h"
#include "MyUrl.h"


CMyUrl::CMyUrl(const CString &sUrl)
{
	int scheme_ = sUrl.Find(_T("://"));

	if (scheme_ < 0)
	{
		m_sScheme = _T("http");

		scheme_ = 0;
	}
	else
	{
		m_sScheme = sUrl.Mid(0, scheme_);
		scheme_ += 3; // skip `://`
	}

	int domain_ = sUrl.Find(_T("/"), scheme_);

	if (domain_ < 0)
	{
		m_sDomain = "";
		domain_ = scheme_;
	}
	else
	{
		m_sDomain = sUrl.Mid(scheme_, domain_ - scheme_);
		//domain_ += 1; // skip `/`

		int colon = m_sDomain.Find(_T(':'));
		if (colon < 0)
		{
			m_Port = 80;
		}
		else
		{
			m_Port = (INTERNET_PORT)_ttoi(m_sDomain.Mid(colon + 1));
			m_sDomain = m_sDomain.Mid(0, colon);
		}
	}

	int path_ = sUrl.Find(_T("?"), domain_);
	if (path_ < 0)
	{
		//m_sDomain = _T("/");
		//path_ = domain_;
		m_sPath = sUrl.Mid(domain_);
		m_sQuery = "";
	}
	else
	{
		m_sPath = sUrl.Mid(domain_, path_ - domain_);
		path_ += 1; // skip `?`

		m_sQuery = sUrl.Mid(path_);
	}

}


CMyUrl::~CMyUrl()
{
}
