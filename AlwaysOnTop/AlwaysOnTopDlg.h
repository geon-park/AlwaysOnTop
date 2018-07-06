
// AlwaysOnTopDlg.h : header file
//

#pragma once
#include "PGProcessList.h"


// CAlwaysOnTopDlg dialog
class CAlwaysOnTopDlg : public CDialog
{
// Construction
public:
	CAlwaysOnTopDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALWAYSONTOP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CListCtrl m_ListWnd;
	PGProcessList m_proclist;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
