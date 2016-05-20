#pragma once
class CMyClientThreadParam
{
public:
	CMyClientThreadParam();
	~CMyClientThreadParam();

	CString m_sServer;
	CString m_sName;
	HWND m_wndDlg;
	CMutex m_mutex;
	BOOL m_bQuit;
};

struct MySmsItem
{
	CString m_sPhone;
	CTime m_tAfter;
};

class MyScanThreadParam
{
public:
	MyScanThreadParam();
	~MyScanThreadParam();

	CString m_sPath;
	int m_nType;
	CList<MySmsItem> m_listItems;
	CTime m_tStart;
	CMutex m_mutex;
};

