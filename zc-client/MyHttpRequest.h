#pragma once
class CMyHttpRequest
{
public:
	CMyHttpRequest();
	~CMyHttpRequest();

public:
	static DWORD Get(const CString &url, CString &content);
	static DWORD Post(const CString &url, const LPVOID lpData, DWORD dwLen, CString &sContent);
};

