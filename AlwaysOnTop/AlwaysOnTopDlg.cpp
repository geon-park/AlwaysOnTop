
// AlwaysOnTopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlwaysOnTop.h"
#include "AlwaysOnTopDlg.h"
#include "afxdialogex.h"

#include <Windows.h>
#include <ShObjIdl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAlwaysOnTopDlg dialog



CAlwaysOnTopDlg::CAlwaysOnTopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ALWAYSONTOP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlwaysOnTopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_PROCESS, m_ListWnd);
}

BEGIN_MESSAGE_MAP(CAlwaysOnTopDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAlwaysOnTopDlg message handlers

BOOL CAlwaysOnTopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	m_ListWnd.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListWnd.InsertColumn(0, _T("Title"), LVCFMT_LEFT, 300);
	m_ListWnd.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 75);
	m_ListWnd.InsertColumn(2, _T("Handle"), LVCFMT_LEFT, 75);
	m_ListWnd.InsertColumn(3, _T("Process"), LVCFMT_LEFT, 75);

	m_ListWnd.DeleteAllItems();


	std::vector<WindowEntry> list = m_proclist.GetProcessList();
	
	for (auto procInfo : list) {
		int nCount = m_ListWnd.GetItemCount();
		m_ListWnd.InsertItem(nCount, procInfo.Title.c_str());
		wchar_t szID[20] = { 0, };
		wsprintf(szID, L"%08X", procInfo.ProcessId);
		m_ListWnd.SetItem(nCount, 1, LVIF_TEXT, szID, -1, 0, 0, 0);
		wsprintf(szID, L"%08X", procInfo.HWnd);
		m_ListWnd.SetItem(nCount, 2, LVIF_TEXT, szID, -1, 0, 0, 0);
		m_ListWnd.SetItem(nCount, 3, LVIF_TEXT, procInfo.ProcessName.c_str(), -1, 0, 0, 0);
	}
	SetTimer(1394, 5000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAlwaysOnTopDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAlwaysOnTopDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAlwaysOnTopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAlwaysOnTopDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_ListWnd.DeleteAllItems();
	std::vector<WindowEntry> list = m_proclist.GetProcessList();
	for (auto procInfo : list) {
		int nCount = m_ListWnd.GetItemCount();
		m_ListWnd.InsertItem(nCount, procInfo.Title.c_str());
		wchar_t szID[20] = { 0, };
		wsprintf(szID, L"%08X", procInfo.ProcessId);
		m_ListWnd.SetItem(nCount, 1, LVIF_TEXT, szID, -1, 0, 0, 0);
		wsprintf(szID, L"%08X", procInfo.HWnd);
		m_ListWnd.SetItem(nCount, 2, LVIF_TEXT, szID, -1, 0, 0, 0);
		m_ListWnd.SetItem(nCount, 3, LVIF_TEXT, procInfo.ProcessName.c_str(), -1, 0, 0, 0);
	}

	CDialog::OnTimer(nIDEvent);
}
