#include "stdafx.h"
#include "TecentShortMessage.h"


CTecentShortMessage::CTecentShortMessage(const CString &text)
	: CSms(text)
{
	m_arrayKeywords.Add(_T("¡¾ÌÚÑ¶¿Æ¼¼¡¿"));
	m_arrayKeywords.Add(_T("[tencent]"));
}


CTecentShortMessage::~CTecentShortMessage()
{
}

BOOL CTecentShortMessage::Match()
{
	CString sContent = GetContent();
	for (int i = 0; i < m_arrayKeywords.GetCount(); i++)
	{
		CString &key = m_arrayKeywords.GetAt(i);
		if (sContent.Find(key) >= 0)
			return TRUE;
	}
	return FALSE;
}