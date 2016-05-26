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

DWORD CMyHttpRequest::UploadFile(const CString &url, const CString &sFileName, CString &sContent)
{
	DWORD dwTotalRequestLength;
	DWORD dwChunkLength;
	DWORD dwReadLength;
	//DWORD dwResponseLength;
	DWORD dwRet;
	CHttpFile* pHTTP = NULL;
	int nBytes;
	char *buffer;


	dwChunkLength = 64 * 1024;
	void* pBuffer = malloc(dwChunkLength);
	CFile file;

	CInternetSession session;
	CHttpConnection *connection = NULL;

	CMyUrl myurl(url);
	CString domain = myurl.GetDomain();
	INTERNET_PORT port = myurl.GetPort();
	CString sUrlpath = myurl.GetPath();

	//Create the multi-part form data that goes before and after the actual file upload.

	file.Open(sFileName, CFile::modeRead);

	CString strHTTPBoundary = _T("FFF3F395A90B452BB8BEDC878DDBD152");
	CString strPreFileData;
	CString sServerFacedName;
	if (sFileName.Mid(sFileName.GetLength() - 4) == _T(".zip"))
	{
		sServerFacedName = _T("1.zip");
	}
	else
	{
		sServerFacedName = _T("1.txt");		
	}
	strPreFileData = CMyHttpRequest::MakePreFileData(strHTTPBoundary, sServerFacedName);
	CString strPostFileData = CMyHttpRequest::MakePostFileData(strHTTPBoundary);
	CString strRequestHeaders = CMyHttpRequest::MakeRequestHeaders(strHTTPBoundary);
	dwTotalRequestLength = (ULONG)strPreFileData.GetLength() + (ULONG) strPostFileData.GetLength() + (ULONG)file.GetLength();

	connection = session.GetHttpConnection(domain, port);

	pHTTP = connection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sUrlpath);
	pHTTP->AddRequestHeaders(strRequestHeaders);
	pHTTP->SendRequestEx(dwTotalRequestLength, HSR_SYNC | HSR_INITIATE);

	//Write out the headers and the form variables
	nBytes = WideCharToMultiByte(CP_ACP, 0, strPreFileData, strPreFileData.GetLength(), NULL, 0, NULL, NULL);
	buffer = new char[nBytes];
	if (buffer == NULL)
	{
		pHTTP->Close();
		connection->Close();
		session.Close();
		file.Close();
		return -1;
	}
	WideCharToMultiByte(CP_ACP, 0, strPreFileData, strPreFileData.GetLength(), buffer, nBytes, NULL, NULL);
	pHTTP->Write(buffer, nBytes);
	delete[] buffer;

	//upload the file.

	dwReadLength = -1;
	ULONG length = (ULONG)file.GetLength(); //used to calculate percentage complete.
	while (0 != dwReadLength)
	{
		dwReadLength = file.Read(pBuffer, dwChunkLength);
		if (0 != dwReadLength)
		{
			pHTTP->Write(pBuffer, dwReadLength);
		}
	}

	file.Close();

	//Finish the upload.
	nBytes = WideCharToMultiByte(CP_ACP, 0, strPostFileData, strPostFileData.GetLength(), NULL, 0, NULL, NULL);
	buffer = new char[nBytes];
	if (buffer == NULL)
	{
		pHTTP->Close();
		connection->Close();
		session.Close();
		file.Close();
		return -1;
	}
	WideCharToMultiByte(CP_ACP, 0, strPostFileData, strPostFileData.GetLength(), buffer, nBytes, NULL, NULL);

	pHTTP->Write(buffer, nBytes);
	delete[] buffer;
	dwRet = pHTTP->EndRequest(HSR_SYNC);


	//get the response from the server.
	if (!dwRet)
	{
		//delete[] buffer;
		//delete[] data;

		pHTTP->Close();
		connection->Close();
		session.Close();

		//CString slog;
		//slog.Format(_T("网络连接错误：%d"), dwRet);
		//MessageBox(slog, _T("提示"));
		return dwRet;
	}

	int len = (int)pHTTP->GetLength();
	buffer = new char[len + 1];
	pHTTP->Read((LPVOID)buffer, len);
	buffer[len] = 0;

	CString sResult((char *)buffer, len);
	sContent = sResult;

	delete[] buffer;
	pHTTP->Close();
	connection->Close();
	session.Close();

	return HTTP_STATUS_OK;
}

CString CMyHttpRequest::MakeRequestHeaders(const CString& strBoundary)
{
	CString strFormat;
	CString strData;
	strFormat = _T("Content-Type: multipart/form-data; boundary=%s\r\n");
	strData.Format(strFormat, strBoundary);
	return strData;
}

CString CMyHttpRequest::MakePreFileData(const CString& strBoundary, const CString& strFileName)
{
	CString strFormat;
	CString strData;

	strFormat += _T("--%s");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Disposition: form-data; name=\"filename\"; filename=\"%s\"");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Type: text/plain");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Transfer-Encoding: binary");
	strFormat += _T("\r\n\r\n");

	strData.Format(strFormat, strBoundary, strFileName);

	return strData;
}

CString CMyHttpRequest::MakePostFileData(const CString& strBoundary)
{

	CString strFormat;
	CString strData;

	strFormat = _T("\r\n");
	strFormat += _T("--%s");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Disposition: form-data; name=\"submitted\"");
	strFormat += _T("\r\n\r\n");
	strFormat += _T("");
	strFormat += _T("\r\n");
	strFormat += _T("--%s--");
	strFormat += _T("\r\n");

	strData.Format(strFormat, strBoundary, strBoundary);

	return strData;

}
