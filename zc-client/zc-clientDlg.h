
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

public:
	

private:
	CListBox *m_lbLog;
	int m_cdtype;
	CString m_cdpath;
	CList<CString> m_lNumbers;
	CList<CString> m_lComPorts;
	CString m_sServer;
	CMyClientThreadParam m_ctObj;
	MyScanThreadParam m_stObj;
	CWinThread *m_clientThread;
	CWinThread *m_cdscanThread;
	int GetCardDrive(int &cdtype, CString &cdpath);
	int GetExePath(DWORD proccId, CString &sPath);
	void AppendLog(const CString &logText);
	void InitCardPoolNumbers();
	PBYTE _ServerInitConnRequestData(int *bufflen);
	void GetSmsFromCardDrive(const CString &phone);
	int SearchSerialPorts();

	static UINT MyClientRunFunc(LPVOID);
	static UINT MyScanFunc(LPVOID);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnServerNumber(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddLog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTaskEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButton1();
};
