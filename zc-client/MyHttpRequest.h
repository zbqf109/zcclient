#pragma once
class CMyHttpRequest
{
public:
	CMyHttpRequest();
	~CMyHttpRequest();

public:
	static DWORD Get(const CString &url, CString &content);
	static DWORD Post(const CString &url, const LPVOID lpData, DWORD dwLen, CString &sContent);

	static DWORD UploadFile(const CString &url, const CString &sFileName, CString &sContent);
	static CString MakeRequestHeaders(const CString& strBoundary);
	static CString MakePreFileData(const CString& strBoundary, const CString& strFileName);
	static CString MakePostFileData(const CString& strBoundary);
};

