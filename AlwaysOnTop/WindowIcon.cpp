#include "stdafx.h"
#include "WindowIcon.h"


WindowIcon::WindowIcon()
{
}


WindowIcon::~WindowIcon()
{
}

HICON WindowIcon::GetAppIcon(HWND hWnd)
{
	HICON iconHandle = (HICON)SendMessageW(hWnd, WM_GETICON, ICON_BIG, 0);
	if (iconHandle == nullptr)
		iconHandle = reinterpret_cast<HICON>(GetClassLongPtrW(hWnd, GCLP_HICON));

	return iconHandle;
}
