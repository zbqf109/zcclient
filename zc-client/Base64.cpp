#include "stdafx.h"
#include "Base64.h"


CBase64::CBase64()
{
}


CBase64::~CBase64()
{
}

unsigned char * base64 = (unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

CString CBase64::encode(const CString &src, int srclen)
{
	int n, buflen, i, j;
	static unsigned char *dst;
	CString buf = src;
	buflen = n = srclen;
	dst = (unsigned char*)malloc(buflen / 3 * 4 + 3);
	memset(dst, 0, buflen / 3 * 4 + 3);
	for (i = 0, j = 0;i <= buflen - 3;i += 3, j += 4) {
		dst[j] = (buf[i] & 0xFC) >> 2;
		dst[j + 1] = ((buf[i] & 0x03) << 4) + ((buf[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((buf[i + 1] & 0x0F) << 2) + ((buf[i + 2] & 0xC0) >> 6);
		dst[j + 3] = buf[i + 2] & 0x3F;
	}
	if (n % 3 == 1) {
		dst[j] = (buf[i] & 0xFC) >> 2;
		dst[j + 1] = ((buf[i] & 0x03) << 4);
		dst[j + 2] = 64;
		dst[j + 3] = 64;
		j += 4;
	}
	else if (n % 3 == 2) {
		dst[j] = (buf[i] & 0xFC) >> 2;
		dst[j + 1] = ((buf[i] & 0x03) << 4) + ((buf[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((buf[i + 1] & 0x0F) << 2);
		dst[j + 3] = 64;
		j += 4;
	}
	for (i = 0;i < j;i++) /* map 6 bit value to base64 ASCII character */
		dst[i] = base64[(int)dst[i]];
	dst[j] = 0;
	return CString(dst);
}

PBYTE CBase64::encode(const PBYTE src, int srclen, int *pdstlen)
{
	int n, buflen, i, j;
	PBYTE dst;
	const PBYTE buf = src;
	buflen = n = srclen;
	dst = new BYTE[buflen / 3 * 4 + 5];
	//memset(dst, 0, buflen / 3 * 4 + 3);
	for (i = 0, j = 0;i <= buflen - 3;i += 3, j += 4) {
		dst[j] = (buf[i] & 0xFC) >> 2;
		dst[j + 1] = ((buf[i] & 0x03) << 4) + ((buf[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((buf[i + 1] & 0x0F) << 2) + ((buf[i + 2] & 0xC0) >> 6);
		dst[j + 3] = buf[i + 2] & 0x3F;
	}
	if (n % 3 == 1) {
		dst[j] = (buf[i] & 0xFC) >> 2;
		dst[j + 1] = ((buf[i] & 0x03) << 4);
		dst[j + 2] = 64;
		dst[j + 3] = 64;
		j += 4;
	}
	else if (n % 3 == 2) {
		dst[j] = (buf[i] & 0xFC) >> 2;
		dst[j + 1] = ((buf[i] & 0x03) << 4) + ((buf[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((buf[i + 1] & 0x0F) << 2);
		dst[j + 3] = 64;
		j += 4;
	}
	for (i = 0;i < j;i++) /* map 6 bit value to base64 ASCII character */
		dst[i] = base64[(int)dst[i]];
	dst[j] = 0;
	*pdstlen = j;
	return dst;
}

CString CBase64::decode(const CString &inpt, int * len)
{
	int n, i, j, pad;
	unsigned char *p;
	wchar_t *dst;
	unsigned char * src;
	*len = 0;
	pad = 0;
	n = inpt.GetLength();
	src = new unsigned char[n];
	for (i = 0;i < n;i++)
		src[i] = (char) inpt[i];

	while (n > 0 && src[n - 1] == '=') {
		src[n - 1] = 0;
		pad++;
		n--;
	}
	for (i = 0;i < n;i++) { /* map base64 ASCII character to 6 bit value */
		p = (unsigned char *)strchr((const char *)base64, (int)src[i]);
		if (!p)
			break;
		src[i] = p - (unsigned char *)base64;
	}

	dst = (wchar_t *)malloc((n * 3 / 4 + 1) * sizeof(wchar_t));
	memset(dst, 0, n * 3 / 4 + 1);
	for (i = 0, j = 0;i < n;i += 4, j += 3) {
		dst[j] = (src[i] << 2) + ((src[i + 1] & 0x30) >> 4);
		dst[j + 1] = ((src[i + 1] & 0x0F) << 4) + ((src[i + 2] & 0x3C) >> 2);
		dst[j + 2] = ((src[i + 2] & 0x03) << 6) + src[i + 3];
		*len += 3;
	}
	*len -= pad;
	delete src;

	return CString(dst, *len+1);
}
