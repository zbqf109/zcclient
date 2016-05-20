#include "stdafx.h"
#include "MyHttpRequest.h"

#include <afxinet.h>
#include "MyUrl.h"


CMyHttpRequest::CMyHttpRequest()
{
}


CMyHttpRequest::~CMyHttpRequest()
{
}

DWORD CMyHttpRequest::Get(const CString &url, CString &sContent)
{
	CInternetSession session;
	CHttpConnection *conn;
	CHttpFile *pHttpFile;

	CString sEmpty("");

	CMyUrl myurl(url);

	CString domain = myurl.GetDomain();
	INTERNET_PORT port = myurl.GetPort();
	CString sUrlpath = myurl.GetPath();

	conn = session.GetHttpConnection(domain, port);
	pHttpFile = conn->OpenRequest(CHttpConnection::HTTP_VERB_GET, sUrlpath);

	if (pHttpFile == NULL)
	{
		conn->Close();
		session.Close();

		//MessageBox(_T("不能建立网络连接"), _T("提示"));
		return -1;
	}
	pHttpFile->SendRequest();

	DWORD dwRet;
	pHttpFile->QueryInfoStatusCode(dwRet);

	if (dwRet != HTTP_STATUS_OK)
	{
		//delete[] buffer;
		//delete[] data;

		pHttpFile->Close();
		conn->Close();
		session.Close();

		//CString slog;
		//slog.Format(_T("网络连接错误：%d"), dwRet);
		//MessageBox(slog, _T("提示"));
		return dwRet;
	}

	int len = (int)pHttpFile->GetLength();
	PBYTE buffer = new BYTE[len + 1];
	pHttpFile->Read((LPVOID)buffer, len);
	buffer[len] = 0;

	CString sResult((char *)buffer, len);
	sContent = sResult;
	
	delete[] buffer;
	pHttpFile->Close();
	conn->Close();
	session.Close();

	return dwRet;
}

DWORD CMyHttpRequest::Post(const CString &url, const LPVOID lpData, DWORD dwLen, CString &sContent)
{
	CInternetSession session;
	CHttpConnection *conn;
	CHttpFile *pHttpFile;

	CString sEmpty("");

	CMyUrl myurl(url);

	CString domain = myurl.GetDomain();
	INTERNET_PORT port = myurl.GetPort();
	CString sUrlpath = myurl.GetPath();

	conn = session.GetHttpConnection(domain, port);
	pHttpFile = conn->OpenRequest(CHttpConnection::HTTP_VERB_POST, sUrlpath);

	if (pHttpFile == NULL)
	{
		conn->Close();
		session.Close();

		//MessageBox(_T("不能建立网络连接"), _T("提示"));
		return -1;
	}
	pHttpFile->SendRequest(sEmpty, lpData, dwLen);

	DWORD dwRet;
	pHttpFile->QueryInfoStatusCode(dwRet);

	if (dwRet != HTTP_STATUS_OK)
	{
		//delete[] buffer;
		//delete[] data;

		pHttpFile->Close();
		conn->Close();
		session.Close();

		//CString slog;
		//slog.Format(_T("网络连接错误：%d"), dwRet);
		//MessageBox(slog, _T("提示"));
		return dwRet;
	}

	int len = (int)pHttpFile->GetLength();
	PBYTE buffer = new BYTE[len + 1];
	pHttpFile->Read((LPVOID)buffer, len);
	buffer[len] = 0;

	CString sResult((char *)buffer, len);
	sContent = sResult;

	delete[] buffer;
	pHttpFile->Close();
	conn->Close();
	session.Close();

	return dwRet;
}

