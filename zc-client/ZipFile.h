#pragma once
class CZipFile
{
public:
	CZipFile();
	~CZipFile();

public:
	static int Zip(const CString &sFilePath, const CString &outFilePath);
	static int UnZip(const CString &srcFilePath, const CString &outFilePath);
};

