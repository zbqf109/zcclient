
// zc-clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zc-client.h"
#include "zc-clientDlg.h"
#include <afxdialogex.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <afxinet.h>
#include "Base64.h"
#include "MyHttpRequest.h"
#include "Sms.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CzcclientDlg dialog



CzcclientDlg::CzcclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ZCCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CzcclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CzcclientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CzcclientDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON1, &CzcclientDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CzcclientDlg message handlers

BOOL CzcclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Get card drive type and path
	GetCardDrive(m_cdtype, m_cdpath);
	if (m_cdtype == 0)
	{
		AppendLog(_T("没有找到卡池驱动!"));
	}
	else if (m_cdtype == 1)
	{
		CString log;
		log.Format(_T("找到酷卡：%s"), m_cdpath);
		AppendLog(log);

		// Get Numbers
	}

	InitCardPoolNumbers();

	m_lbLog = (CListBox *)GetDlgItem(IDC_LIST2);

	// Initialize list control
	CListCtrl *lst = (CListCtrl *)GetDlgItem(IDC_LIST1);
	DWORD lst_style = lst->GetExtendedStyle();
	lst->SetExtendedStyle(lst_style | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// Insert columns
	lst->InsertColumn(0, _T("COM"));
	lst->InsertColumn(1, _T("手机号码"));
	lst->InsertColumn(2, _T("状态"));

	// Set Column Width
	lst->SetColumnWidth(0, 50);
	lst->SetColumnWidth(1, 200);
	lst->SetColumnWidth(2, 100);

	// Insert numbers
	POSITION p = m_lNumbers.GetHeadPosition();
	for (int i = 0; i < m_lNumbers.GetCount(); i++)
	{
		CString sNumber = m_lNumbers.GetNext(p);
		CString sComserial;
		sComserial.Format(_T("COM%d"), i + 2);
		lst->InsertItem(i, sComserial);
		lst->SetItemText(i, 1, sNumber);

		if (sNumber.GetLength() > 0)
		{
			lst->SetItemText(i, 2, _T("就绪"));
		}
	}

	// server
	SetDlgItemText(IDC_EDIT3, _T("192.168.4.194:8088"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CzcclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CzcclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CzcclientDlg::OnBnClickedButtonStart()
{
	CString name;

	GetDlgItemText(IDC_EDIT1, name);

	name = name.Trim();

	if (name.GetLength() == 0)
	{
		MessageBox(_T("请填写工号"), _T("提示"));
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return;
	}

	
	GetDlgItemText(IDC_EDIT3, m_sServer);

	if (m_sServer.GetLength() == 0)
	{
		MessageBox(_T("服务器地址错误"), _T("提示"));
		return;
	}

	int slen;
	PBYTE buffer = _ServerInitConnRequestData(&slen);

	if (buffer == NULL)
	{
		MessageBox(_T("内存不足"), _T("提示"));
		return;
	}

	int datalen;
	PBYTE base64data = CBase64::encode(buffer, slen, &datalen);
	if (base64data == NULL)
	{
		delete[] buffer;
		//conn->Close();
		//session.Close();
		MessageBox(_T("内存不足"), _T("提示"));
		return;
	}
	PBYTE data = new BYTE[datalen + 4];
	if (data == NULL)
	{
		delete[] base64data;
		delete[] buffer;
		//conn->Close();
		//session.Close();
		MessageBox(_T("内存不足"), _T("提示"));
		return;
	}

	data[0] = 'n';
	data[1] = '=';
	memcpy(data + 2, base64data, datalen);
	delete[] base64data;

	CString surl;
	surl.Format(_T("http://%s/1/%s/1"), m_sServer, name);

	CString sContent;

	DWORD dwRet = CMyHttpRequest::Post(surl, data, datalen+2, sContent);

	if (dwRet < 0)
	{
		MessageBox(_T("不能建立网络连接"), _T("提示"));
		return;
	}
	else if (dwRet != HTTP_STATUS_OK)
	{
		CString slog;
		slog.Format(_T("网络连接错误：%d"), dwRet);
		MessageBox(slog, _T("提示"));
		return;
	}

	CString sResultMsg;
	//MessageBox();
	sResultMsg.Format(_T("初始化成功，服务器返回 %s"), sContent);
	AppendLog(sResultMsg);

	m_ctObj.m_sServer = m_sServer;
	m_ctObj.m_sName = name;

	m_clientThread = AfxBeginThread(CzcclientDlg::MyClientRunFunc, &m_ctObj);

	
#if 0

	CString domain;
	INTERNET_PORT port;

	int colon = m_sServer.Find(_T(':'));
	if (colon < 0)
	{
		domain = m_sServer;
		port = 80; // DEFAULT 
	}
	else
	{
		domain = m_sServer.Mid(0, colon);

		CString sPort = m_sServer.Mid(colon + 1);

		port = (INTERNET_PORT)_ttoi(sPort);
	}

	// MessageBox(_T("你的工号是: "), _T("提示"));
	CInternetSession session;
	CHttpConnection *conn;
	CString sUrlpath;
	CHttpFile *pHttpFile;

	

	
	
	CString headerString(_T("Content-Type: application/x-www-form-urlencoded;"));
	
 	//CString dataString = CBase64::encode(sCurrentSiteNumbers, slen);
	

	sUrlpath.Format(_T("/1/%s/1"), name);

	conn = session.GetHttpConnection(domain, port);
	pHttpFile = conn->OpenRequest(CHttpConnection::HTTP_VERB_POST, sUrlpath);
	if (pHttpFile == NULL)
	{
		conn->Close();
		session.Close();

		MessageBox(_T("不能建立网络连接"), _T("提示"));
		return;
	}
	pHttpFile->SendRequest(headerString, (LPVOID)data, datalen+2);

	DWORD dwRet;
	pHttpFile->QueryInfoStatusCode(dwRet);

	if (dwRet != HTTP_STATUS_OK)
	{
		delete[] buffer;
		delete[] data;

		pHttpFile->Close();
		conn->Close();
		session.Close();

		CString slog;
		slog.Format(_T("网络连接错误：%d"), dwRet);
		MessageBox(slog, _T("提示"));
		return;
	}

	delete[] buffer;
	delete[] data;

	int len = (int) pHttpFile->GetLength();
	buffer = new BYTE[len+1];
	pHttpFile->Read((LPVOID)buffer, len);
	buffer[len + 1] = 0;

	CString sResult((char *)buffer, len);
	CString sResultMsg;
	//MessageBox();
	sResultMsg.Format(_T("初始化成功，服务器返回 %s"), sResult);
	AppendLog(sResultMsg);
	
	conn->Close();
	session.Close();
#endif
}

PBYTE CzcclientDlg::_ServerInitConnRequestData(int *bufflen)
{
	DWORD slen = 0;
	size_t buflen = m_lNumbers.GetCount() * sizeof(long long) + 4;
	PBYTE buffer = new BYTE[buflen];
	long long number;
	ULONG low, high, nw_low, nw_high;

	if (buffer == NULL)
	{
		/*conn->Close();
		session.Close();
		MessageBox(_T("内存不足"), _T("提示"));*/
		return NULL;
	}

	POSITION p = m_lNumbers.GetHeadPosition();
	for (int i = 0; i < m_lNumbers.GetCount(); i++)
	{
		CString &name = m_lNumbers.GetNext(p);

		if (name.GetLength() == 0)
			continue;

		number = _ttoll(name);

		low  = (ULONG)number;
		high = (ULONG)(number >> 32);

		nw_low  = htonl(low);
		nw_high = htonl(high);

		if (slen > 0)
		{
			//sCurrentSiteNumbers += _T(",");
			*(buffer + slen) = ',';
			slen += 1;
		}
		//sCurrentSiteNumbers += name;
		//slen += name.GetLength();
		memcpy(buffer + slen, &nw_high, sizeof(ULONG));
		slen += sizeof(ULONG);
		memcpy(buffer + slen, &nw_low, sizeof(ULONG));
		slen += sizeof(ULONG);
	}
	buffer[slen] = 0;

	*bufflen = slen;
	return buffer;
}

int CzcclientDlg::GetCardDrive(int &cdtype, CString &cdpath)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	cdtype = 0;

	// 给系统内所有的进程拍个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		AppendLog(_T("CreateToolhelp32Snapshot 调用失败"));
		return -1;
	}

	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (wcsstr(pe32.szExeFile, _T("CoolCard.exe")) != NULL)
		{
			cdtype = 1; // Cool Card
			GetExePath(pe32.th32ProcessID, cdpath);
			break;
		}
		else {
			// TODO PowerMMS.exe, 叮叮摩卡.exe
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	return 0;
}

void CzcclientDlg::AppendLog(const CString &logText)
{
	if (m_lbLog == NULL)
	{
		m_lbLog = (CListBox *)GetDlgItem(IDC_LIST2);
	}

	if (m_lbLog == NULL)
	{
		MessageBox(_T("无法记录日志!"), _T("提示"));
		return;
	}

	m_lbLog->InsertString(-1, logText);
}

int CzcclientDlg::GetExePath(DWORD proccId, CString &sPath)
{
	//GetWindowThreadProcessId(this->GetSafeHwnd(), &pe32.th32ProcessID);
	/*HWND hWnd = ::GetTopWindow(NULL);
	DWORD pid;

	while (hWnd)
	{
		DWORD dwProcessId = GetWindowThreadProcessId(hWnd, &pid);
		if (pid == proccId)
			break;
	}*/

	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	TCHAR szModName[MAX_PATH];

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proccId);
	if (hProcess == NULL)
		return -1;
	
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		/*for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				_tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
			}
		}*/

		if (GetModuleFileNameEx(hProcess, hMods[0], szModName,
			sizeof(szModName) / sizeof(TCHAR)))
		{
			sPath = szModName;
			int p = sPath.ReverseFind(_T('\\'));
			sPath = sPath.Mid(0, p+1);
			return 0;
		}
	}

	return -1;
}

void CzcclientDlg::InitCardPoolNumbers()
{
	if (m_cdtype == 0)
		return;

	char *buffer;
	UINT flen, rlen;

	if (m_cdtype == 1)
	{
		// Cool Card
		CString txt = _T("当前本机号码.txt");

		CFile file;
		if (!file.Open(m_cdpath + txt, CFile::modeRead))
		{
			CString log;
			log.Format(_T("获取号码失败，因为 %s%s 文件不存在！"), m_cdpath, txt);
			AppendLog(log);
			return;
		}

		

		flen = (size_t) file.GetLength();
		buffer = new char[flen];

		if (buffer == NULL)
		{
			AppendLog(_T("内存不足!"));
			file.Close();
			return;
		}

		rlen = file.Read(buffer, flen);
		ASSERT(rlen == flen);

		file.Close();

		UINT lasti = 0;
		int nPhoneNumber = 0;

		for (UINT i = 0; i < flen; i++)
		{
			if (buffer[i] == '\r' || buffer[i] == '\n')
			{
				
				if (i - lasti > 1)
				{
					nPhoneNumber++;
					CString s(buffer + lasti, i - lasti);
					m_lNumbers.AddTail(s);
				}
				else
				{
					m_lNumbers.AddTail(_T(""));
				}
				i++;
				lasti = i;
				
			}
		}

		// the last part
		if (flen - lasti > 1)
		{
			CString s(buffer + lasti, flen - lasti);
			m_lNumbers.AddTail(s);
		}

		delete[] buffer;

		CString sLog;
		sLog.Format(_T("读取到 %d 个手机号"), nPhoneNumber);
		AppendLog(sLog);
	}
}

void CzcclientDlg::OnBnClickedButton1()
{
	CWnd *pEditButton = GetDlgItem(IDC_BUTTON1);
	CWnd *pWnd = GetDlgItem(IDC_EDIT3);
	if (pEditButton == NULL || pWnd == NULL)
		return;

	if (pWnd->IsWindowEnabled())
	{
		pWnd->EnableWindow(FALSE);
		pEditButton->SetWindowText(_T("Edit"));
	}
	else
	{
		pWnd->EnableWindow();
		pEditButton->SetWindowText(_T("Done"));
	}	
}

void CzcclientDlg::GetSmsFromCardDrive(const CString &phone)
{
	CFile file;

	CString sSmsFilePath;
	sSmsFilePath.Format(_T("%s总保存记录中\\短信接收记录.txt"), m_cdpath);

	if (!file.Open(sSmsFilePath, CFile::modeRead))
	{
		CString sLog;
		sLog.Format(_T("不能读取短信记录，因为文件 %s 不存在！"), sSmsFilePath);
		AppendLog(sLog);
		return;
	}

	file.Close();

}


UINT CzcclientDlg::MyClientRunFunc(LPVOID param)
{
	CMyClientThreadParam *ctObj = (CMyClientThreadParam *)param;

	CString sUrl;
	sUrl.Format(_T("%s/1/%s/3?t=%d"), ctObj->m_sServer, ctObj->m_sName, rand());

	CString sContent;
	DWORD dwRet;

	// GetSmsFromCardDrive(_T("18049634161"));
	while (TRUE)
	{
		// 1. request /1/<name>/2
		dwRet = CMyHttpRequest::Get(sUrl, sContent);
		if (dwRet != HTTP_STATUS_OK)
		{
			continue;
		}

		// parse response text
		if (sContent == _T("ok"))
			continue;

		int len;
		CString n = CBase64::decode(sContent, &len);
	}

	return 0;
}