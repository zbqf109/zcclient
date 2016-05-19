
// zc-clientDlg.h : header file
//

#pragma once
#include "MyClientThreadParam.h"

// CzcclientDlg dialog
class CzcclientDlg : public CDialogEx
{
// Construction
public:
	CzcclientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZCCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CListBox *m_lbLog;
	int m_cdtype;
	CString m_cdpath;
	CList<CString> m_lNumbers;
	CString m_sServer;
	CMyClientThreadParam m_ctObj;
	CWinThread *m_clientThread;
	int GetCardDrive(int &cdtype, CString &cdpath);
	int GetExePath(DWORD proccId, CString &sPath);
	void AppendLog(const CString &logText);
	void InitCardPoolNumbers();
	PBYTE _ServerInitConnRequestData(int *bufflen);
	void GetSmsFromCardDrive(const CString &phone);

	static UINT MyClientRunFunc(LPVOID);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButton1();
};
