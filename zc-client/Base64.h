#pragma once
class CBase64
{
public:
	CBase64();
	~CBase64();

public:
	static CString encode(const CString &src, int srclen);
	static PBYTE   encode(const PBYTE src, int srclen, int *pdstlen);

	static CString decode(const CString &inpt, int * len);
};

