
// AlwaysOnTopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlwaysOnTop.h"
#include "AlwaysOnTopDlg.h"
#include "afxdialogex.h"

#include <Windows.h>
#include <ShObjIdl.h>
#include "WindowHandler.h"

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



CAlwaysOnTopDlg::CAlwaysOnTopDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ALWAYSONTOP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlwaysOnTopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_PROCESS, m_ListWnd);
	DDX_Control(pDX, IDC_BTN_TOP, m_BtnTop);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
}

BEGIN_MESSAGE_MAP(CAlwaysOnTopDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_TOP, &CAlwaysOnTopDlg::OnBnClickedBtnTop)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CAlwaysOnTopDlg::OnBnClickedBtnCancel)
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
	if (pSysMenu != nullptr)
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

	ReplaceCtrl();

	m_ListWnd.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListWnd.InsertColumn(0, L"Title", LVCFMT_LEFT, 500);
	m_ListWnd.InsertColumn(1, L"PID", LVCFMT_LEFT, 100);
	m_ListWnd.InsertColumn(2, L"Handle", LVCFMT_LEFT, 100);
	m_ListWnd.InsertColumn(3, L"Process", LVCFMT_LEFT, 200);
	
	m_ListWnd.UpdateWindowList();
	SetTimer(1394, 2000, nullptr);

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
	m_ListWnd.UpdateWindowList();

	CDialog::OnTimer(nIDEvent);
}


void CAlwaysOnTopDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 768;

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CAlwaysOnTopDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	ReplaceCtrl();
}


void CAlwaysOnTopDlg::ReplaceCtrl()
{
	if (nullptr == GetSafeHwnd())
		return;

	CRect rc;
	GetClientRect(&rc);

	CRect rcCtrl(rc);
	const HDWP hdwp = ::BeginDeferWindowPos(GetWindowedChildCount());

	if (nullptr != m_ListWnd.GetSafeHwnd())
	{
		rcCtrl.left = rc.left + 10;
		rcCtrl.top = rc.top + 10;

		::DeferWindowPos(hdwp, m_ListWnd.GetSafeHwnd(), nullptr, rcCtrl.left, rcCtrl.top, rc.Width() - 20, rc.Height() - 80, SWP_NOZORDER);
	}

	if (nullptr != m_BtnTop.GetSafeHwnd())
	{
		
		rcCtrl.left = (rc.Width() / 2) - 130;
		rcCtrl.top = rc.Height() - 60;

		::DeferWindowPos(hdwp, m_BtnTop.GetSafeHwnd(), nullptr, rcCtrl.left, rcCtrl.top, 120, 30, SWP_NOZORDER);
	}

	if (nullptr != m_BtnCancel.GetSafeHwnd())
	{

		rcCtrl.left = (rc.Width() / 2) + 10;
		rcCtrl.top = rc.Height() - 60;

		::DeferWindowPos(hdwp, m_BtnCancel.GetSafeHwnd(), nullptr, rcCtrl.left, rcCtrl.top, 120, 30, SWP_NOZORDER);
	}

	::EndDeferWindowPos(hdwp);
}

void CAlwaysOnTopDlg::OnBnClickedBtnTop()
{
	POSITION pos = m_ListWnd.GetFirstSelectedItemPosition();
	while (nullptr != pos)
	{
		int nItem = m_ListWnd.GetNextSelectedItem(pos);
		CString szHandle = m_ListWnd.GetItemText(nItem, 2);
		HWND hWnd = (HWND)wcstoll(szHandle, nullptr, 16);

		WindowHandler::SetTopMostWindow(hWnd);
	}

}

void CAlwaysOnTopDlg::OnBnClickedBtnCancel()
{
	POSITION pos = m_ListWnd.GetFirstSelectedItemPosition();
	while (nullptr != pos)
	{
		int nItem = m_ListWnd.GetNextSelectedItem(pos);
		CString szHandle = m_ListWnd.GetItemText(nItem, 2);
		HWND hWnd = (HWND)wcstoll(szHandle, nullptr, 16);

		WindowHandler::SetNoTopMostWindow(hWnd);
	}
}
