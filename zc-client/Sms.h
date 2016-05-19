#pragma once
class CSms
{
public:
	CSms(const CString &strSms);
	~CSms();

	CTime GetTime() const
	{
		return m_tSendTime;
	}

	CString GetFrom() const
	{
		return m_sFrom;
	}

	CString GetTo() const
	{
		return m_sTo;
	}

	CString GetCOM() const
	{
		return m_sWhichCom;
	}

	CString GetContent() const
	{
		return m_sContent;
	}

	CString GetVCode() const;

public:
	static CTime ConvertStringToTime(const CString &strTime);

private:
	CTime m_tSendTime;
	CString m_sFrom;
	CString m_sTo;
	CString m_sWhichCom;
	CString m_sContent;
};

