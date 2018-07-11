// WindowsListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WindowsListCtrl.h"

// WindowsListCtrl

IMPLEMENT_DYNAMIC(WindowsListCtrl, CListCtrl)

WindowsListCtrl::WindowsListCtrl()
{

}

WindowsListCtrl::~WindowsListCtrl()
{
}


BEGIN_MESSAGE_MAP(WindowsListCtrl, CListCtrl)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()


// WindowsListCtrl message handlers


void WindowsListCtrl::DrawItem(LPDRAWITEMSTRUCT pDrawItemStruct)
{
	LV_COLUMN lvc, lvcprev;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;
	
	CDC* pDC = CDC::FromHandle(pDrawItemStruct->hDC);

	CFont font;

	font.CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FF_DONTCARE, L"Arial");

	pDC->SelectObject(&font);

	for (auto n_col = 0; GetColumn(n_col, &lvc); n_col++)
	{
		wchar_t buffer[1024] = { 0, };

		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem = pDrawItemStruct->itemID;
		lvi.iSubItem = n_col;
		lvi.pszText = buffer;
		lvi.cchTextMax = 1024;
		VERIFY(GetItem(&lvi));

		if (n_col > 0)
		{
			GetColumn(n_col - 1, &lvcprev);
			pDrawItemStruct->rcItem.left += lvcprev.cx;
			pDrawItemStruct->rcItem.right = pDrawItemStruct->rcItem.left + lvc.cx;
		}

		if (pDrawItemStruct->itemState & ODS_SELECTED)
		{
			pDC->FillSolidRect(&pDrawItemStruct->rcItem, RGB(0x7F, 0x7F, 0xFF)/*GetSysColor(COLOR_HIGHLIGHT)*/);
			pDC->SetTextColor(RGB(0xFF, 0x7F, 0x3F)/*GetSysColor(COLOR_HIGHLIGHTTEXT)*/);
		}
		else
		{
			pDC->FillSolidRect(&pDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}

		if (n_col > 0)
		{
			CRect rc(pDrawItemStruct->rcItem);
			CRect rc_temp(pDrawItemStruct->rcItem);
			
			int height = pDC->DrawTextW(buffer, static_cast<int>(wcslen(buffer)), &rc_temp, DT_CALCRECT);
			rc.left += default_margin_size_;
			rc.top = rc.CenterPoint().y - (height / 2);
			rc.bottom = rc.top + height;

			pDC->DrawTextW(buffer, static_cast<int>(wcslen(buffer)), &rc, DT_LEFT);
		}
		else
		{
			CRect rc(pDrawItemStruct->rcItem);
			rc.right = rc.left + lvc.cx;
			CRect rc_temp(&rc);

			wchar_t hwnd_text[16] = { 0, };
			GetItemText(pDrawItemStruct->itemID, 2, hwnd_text, 16);
			HWND hWnd = reinterpret_cast<HWND>(std::wcstoull(hwnd_text, nullptr, 16));

			HICON hIcon = WindowIcon::GetAppIcon(hWnd);
			
			pDC->DrawIcon(rc.left + default_margin_size_, rc.top + default_margin_size_, hIcon);

			
			rc_temp.left += (3 * default_margin_size_) + default_icons_size_;
			const auto height = pDC->DrawTextW(buffer, static_cast<int>(wcslen(buffer)), &rc_temp, DT_CALCRECT | DT_END_ELLIPSIS);

			rc.left += (3 * default_margin_size_) + default_icons_size_;
			rc.top = rc.CenterPoint().y - (height / 2);
			rc.bottom = rc.top + height;
			pDC->DrawTextW(buffer, static_cast<int>(wcslen(buffer)), &rc, DT_LEFT | DT_END_ELLIPSIS);
		}
	}
}


LRESULT WindowsListCtrl::OnSetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT res = Default();

	CRect rc;
	GetWindowRect(&rc);

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM>(&wp));

	return res;
}

void WindowsListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = default_icons_size_ + (2 * default_margin_size_);
}


void WindowsListCtrl::UpdateWindowList()
{
	auto index = 0;
	proc_list_ = std::move(windows_list_factory_.GetProcessList());

	for (auto procInfo : proc_list_)
	{
		wchar_t curHWnd[16] = { 0, };
		wsprintf(curHWnd, L"%08X", procInfo.HWnd);
		wchar_t hwnd[16] = { 0, };
		GetItemText(index, 2, hwnd, 16);

		int cmpResult = wcscmp(curHWnd, hwnd);
		if (index == GetItemCount() || cmpResult < 0)
		{
			InsertItem(index, procInfo.Title.c_str(), index);

			wsprintf(curHWnd, L"%08X", procInfo.ProcessId);
			SetItem(index, 1, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
			wsprintf(curHWnd, L"%08X", procInfo.HWnd);
			SetItem(index, 2, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
			SetItem(index, 3, LVIF_TEXT, procInfo.ProcessName.c_str(), -1, 0, 0, 0);
			++index;
		}
		else if (cmpResult == 0)
		{
			wchar_t title[1024] = { 0, };
			GetItemText(index, 0, title, 1024);
			if (procInfo.Title.compare(title) != 0)
				SetItem(index, 0, LVIF_TEXT, procInfo.Title.c_str(), index, 0, 0, 0);
			++index;
		}
		else // if (cmpResult > 0)
		{
			while (true)
			{
				GetItemText(index, 2, hwnd, 16);
				cmpResult = wcscmp(curHWnd, hwnd);

				if (cmpResult > 0)
				{
					DeleteItem(index);
					continue;
				}
				else if (cmpResult == 0)
				{
					wchar_t title[1024] = { 0, };
					GetItemText(index, 0, title, 1024);
					if (procInfo.Title.compare(title) != 0)
						SetItem(index, 0, LVIF_TEXT, procInfo.Title.c_str(), -1, 0, 0, 0);
					++index;
					break;
				}
				else
				{
					InsertItem(index, procInfo.Title.c_str());

					wsprintf(curHWnd, L"%08X", procInfo.ProcessId);
					SetItem(index, 1, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
					wsprintf(curHWnd, L"%08X", procInfo.HWnd);
					SetItem(index, 2, LVIF_TEXT, curHWnd, -1, 0, 0, 0);
					SetItem(index, 3, LVIF_TEXT, procInfo.ProcessName.c_str(), -1, 0, 0, 0);
					++index;
					break;
				}
			}

		}
	}
	for (int i = GetItemCount(); i >= index; --i)
	{
		DeleteItem(i);
	}
}
