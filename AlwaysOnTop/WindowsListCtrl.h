#pragma once

#include "WindowIcon.h"
#include "WindowsListFactory.h"

#include <string>

// WindowsListCtrl

class WindowsListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(WindowsListCtrl)

public:
	WindowsListCtrl();
	virtual ~WindowsListCtrl();

public:
	void UpdateWindowList();

private:
	WindowsListFactory m_proclist;
	const int default_icons_size_ = 32;
	const int default_margin_size_ = 4;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void DrawItem(LPDRAWITEMSTRUCT) override;
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};
