#include "stdafx.h"
#include "Sms.h"


CSms::CSms(const CString &strSms)
{
	// 2016-04-10 21:59:22,10659057607559,18157762594,497825，为您的QQ注册验证码，请您在30分钟内完成注册。如非本人操作，请忽略。【腾讯科技】

	int comma = 0, comma1;

	// 1. Time
	comma1 = strSms.Find(_T(','), comma);
	if (comma1 < 0)
		return;
	
	CString sTime = strSms.Mid(0, comma1);
	m_tSendTime = ConvertStringToTime(sTime);

	comma = comma1 + 1;

	// 2. from
	comma1 = strSms.Find(_T(','), comma);
	if (comma1 < 0)
		return;
	m_sFrom = strSms.Mid(comma, comma1 - comma);
	comma = comma1 + 1;

	// 3. to
	comma1 = strSms.Find(_T(','), comma);
	if (comma1 < 0)
		return;
	m_sTo = strSms.Mid(comma, comma1 - comma);
	comma = comma1 + 1;
	
	// 4. content
	m_sContent = strSms.Mid(comma);
}


CSms::~CSms()
{
}

CString CSms::GetVCode() const
{
	CString code;

	return code;
}

CTime CSms::ConvertStringToTime(const CString &strTime)
{
	int year=2016, month=5, day=1, hour=0, minite=0, second = 0;

	int firstp = 0, lastp = 0;

	// year
	lastp = strTime.Find(_T('-'));

	if (lastp < 0)
	{
		year = _ttoi(strTime);
		return CTime(year, month, day, hour, minite, second);
	}

	year = _ttoi(strTime.Mid(firstp, lastp - firstp));

	firstp = lastp + 1;

	// month
	lastp = strTime.Find(_T('-'), firstp);

	if (lastp < 0)
	{
		month = _ttoi(strTime);
		return CTime(year, month, day, hour, minite, second);
	}

	month = _ttoi(strTime.Mid(firstp, lastp - firstp));

	firstp = lastp + 1;

	// day
	lastp = strTime.Find(_T(' '), firstp);

	if (lastp < 0)
	{
		day = _ttoi(strTime);
		return CTime(year, month, day, hour, minite, second);
	}

	day = _ttoi(strTime.Mid(firstp, lastp - firstp));

	firstp = lastp + 1;

	// hour
	lastp = strTime.Find(_T(':'), firstp);

	if (lastp < 0)
	{
		hour = _ttoi(strTime);
		return CTime(year, month, day, hour, minite, second);
	}

	hour = _ttoi(strTime.Mid(firstp, lastp - firstp));

	firstp = lastp + 1;

	// minite
	lastp = strTime.Find(_T(':'), firstp);

	if (lastp < 0)
	{
		minite = _ttoi(strTime);
		return CTime(year, month, day, hour, minite, second);
	}

	minite = _ttoi(strTime.Mid(firstp, lastp - firstp));

	firstp = lastp + 1;

	// second 
	second = _ttoi(strTime.Mid(firstp));

	return CTime(year, month, day, hour, minite, second);
}