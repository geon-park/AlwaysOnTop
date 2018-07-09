
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
	DDX_Control(pDX, IDC_BTN_TOP, m_BtnTop);
}

BEGIN_MESSAGE_MAP(CAlwaysOnTopDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_TOP, &CAlwaysOnTopDlg::OnBnClickedBtnTop)
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

	ReplaceCtrl();

	m_ListWnd.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListWnd.InsertColumn(0, _T("Title"), LVCFMT_LEFT, 500);
	m_ListWnd.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 100);
	m_ListWnd.InsertColumn(2, _T("Handle"), LVCFMT_LEFT, 100);
	m_ListWnd.InsertColumn(3, _T("Process"), LVCFMT_LEFT, 200);

	m_imageList.Create(0x20, 0x20, ILC_COLOR32, 8, 8);
	m_ListWnd.SetImageList(&m_imageList, LVSIL_SMALL);
	
	UpdateWindowList();
	SetTimer(1394, 10000, NULL);

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
	UpdateWindowList();

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
	if (NULL == GetSafeHwnd())
		return;

	CRect rc;
	GetClientRect(&rc);

	CRect rcCtrl(rc);
	HDWP hdwp = ::BeginDeferWindowPos(GetWindowedChildCount());

	if (NULL != m_ListWnd.GetSafeHwnd())
	{
		rcCtrl.left = rc.left + 10;
		rcCtrl.top = rc.top + 10;

		::DeferWindowPos(hdwp, m_ListWnd.GetSafeHwnd(), NULL, rcCtrl.left, rcCtrl.top, rc.Width() - 20, rc.Height() - 80, SWP_NOZORDER);
	}

	if (NULL != m_BtnTop.GetSafeHwnd())
	{
		
		rcCtrl.left = (rc.Width() / 2) - 60;
		rcCtrl.top = rc.Height() - 60;

		::DeferWindowPos(hdwp, m_BtnTop.GetSafeHwnd(), NULL, rcCtrl.left, rcCtrl.top, 120, 30, SWP_NOZORDER);
	}

	::EndDeferWindowPos(hdwp);
}

void CAlwaysOnTopDlg::UpdateWindowList()
{
	int index = 0;
	std::vector<WindowEntry> list = m_proclist.GetProcessList();
	
	/*
	CBitmap bmpIcon;
	WindowIcon::HIconToCBitmap(procInfo.IconHandle, &bmpIcon, 0x20);
	m_imageList.Add(&bmpIcon, RGB(0xFF, 0xFF, 0xFF));
	bmpIcon.DeleteObject();
	*/

	for (auto procInfo : list)
	{
		wchar_t curHWnd[16] = { 0, };
		wsprintf(curHWnd, L"%08X", procInfo.HWnd);
		wchar_t hwnd[16] = { 0, };
		m_ListWnd.GetItemText(index, 2, hwnd, 16);

		int cmpResult = wcscmp(curHWnd, hwnd);
		if (index == m_ListWnd.GetItemCount() || cmpResult < 0)
		{
			CBitmap bmpIcon;
			WindowIcon::HIconToCBitmap(procInfo.IconHandle, &bmpIcon, DefaultIconSize);
			m_imageList.Add(&bmpIcon, RGB(0x00, 0x00, 0x00));
			bmpIcon.DeleteObject();

			m_ListWnd.InsertItem(index, procInfo.Title.c_str(), index);

			wsprintf(curHWnd, L"%08X", procInfo.ProcessId);
			m_ListWnd.SetItem(index, 1, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
			wsprintf(curHWnd, L"%08X", procInfo.HWnd);
			m_ListWnd.SetItem(index, 2, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
			m_ListWnd.SetItem(index, 3, LVIF_TEXT, procInfo.ProcessName.c_str(), -1, 0, 0, 0);
			++index;
		}
		else if (cmpResult == 0)
		{
			wchar_t title[1024] = { 0, };
			m_ListWnd.GetItemText(index, 0, title, 1024);
			if (procInfo.Title.compare(title) != 0 )
				m_ListWnd.SetItem(index, 0, LVIF_TEXT, procInfo.Title.c_str(), index, 0, 0, 0);
			++index;
		}
		else // if (cmpResult > 0)
		{
			while (true)
			{
				m_ListWnd.GetItemText(index, 2, hwnd, 16);
				cmpResult = wcscmp(curHWnd, hwnd);

				if (cmpResult > 0)
				{
					m_ListWnd.DeleteItem(index);
					continue;
				}
				else if (cmpResult == 0)
				{
					wchar_t title[1024] = { 0, };
					m_ListWnd.GetItemText(index, 0, title, 1024);
					if (procInfo.Title.compare(title) != 0)
						m_ListWnd.SetItem(index, 0, LVIF_TEXT, procInfo.Title.c_str(), -1, 0, 0, 0);
					++index;
					break;
				}
				else
				{
					m_ListWnd.InsertItem(index, procInfo.Title.c_str());

					wsprintf(curHWnd, L"%08X", procInfo.ProcessId);
					m_ListWnd.SetItem(index, 1, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
					wsprintf(curHWnd, L"%08X", procInfo.HWnd);
					m_ListWnd.SetItem(index, 2, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
					m_ListWnd.SetItem(index, 3, LVIF_TEXT, procInfo.ProcessName.c_str(), -1, 0, 0, 0);
					++index;
					break;
				}
			}
			
		}
	}
	for (int i = m_ListWnd.GetItemCount() ; i >= index ; --i)
	{
		m_ListWnd.DeleteItem(i);
	}
}


void CAlwaysOnTopDlg::OnBnClickedBtnTop()
{
	POSITION pos = m_ListWnd.GetFirstSelectedItemPosition();
	while (nullptr != pos)
	{
		int nItem = m_ListWnd.GetNextSelectedItem(pos);
		CString szHandle = m_ListWnd.GetItemText(nItem, 2);
		HWND hWnd = (HWND)wcstoll(szHandle, nullptr, 16);

		::SetForegroundWindow(hWnd);

		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		::SetWindowLongPtr(hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
	}

}
