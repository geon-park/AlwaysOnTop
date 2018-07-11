#include "stdafx.h"
#include "WindowIcon.h"

#include <utility>

WindowIcon::WindowIcon()
{
}


WindowIcon::~WindowIcon()
{
}

HICON WindowIcon::GetAppIcon(HWND hWnd)
{
	HICON iconHandle = nullptr;
	iconHandle = reinterpret_cast<HICON>(GetClassLongPtrW(hWnd, GCLP_HICON));
	if (iconHandle == nullptr)
		iconHandle = reinterpret_cast<HICON>(SendMessageW(hWnd, WM_GETICON, ICON_BIG, 0));
	if (iconHandle == nullptr)
		iconHandle = reinterpret_cast<HICON>(SendMessageW(hWnd, WM_GETICON, ICON_SMALL, 0));
	if (iconHandle == nullptr)
		iconHandle = reinterpret_cast<HICON>(SendMessageW(hWnd, WM_GETICON, ICON_SMALL2, 0));

	return iconHandle;
}

CSize WindowIcon::HIconToCBitmap(HICON hIcon, CBitmap* bmpIcon, const int defaultSize/* = 0x20*/)
{
	CBitmap *pOldBmp = nullptr, *pOldBmp2 = nullptr;
	CDC memDC, memDC2;
	CBitmap bmpTemp;
	bool usable_size = true;

	CSize size = GetIconSize(hIcon);
	if (size.cx == 0)
	{
		usable_size = false;
		size.cx = defaultSize;
		size.cy = defaultSize;
	}

	memDC.CreateCompatibleDC(nullptr);
	bmpTemp.CreateBitmap(size.cx, size.cy, 1, 32, nullptr);
	pOldBmp = memDC.SelectObject(&bmpTemp);
	
	if (!usable_size)
	{
		memDC.FillSolidRect(0, 0, size.cx, size.cy, RGB(0xFF, 0xFF, 0xFF));
	}
	else
	{
		memDC.DrawIcon(0, 0, hIcon);
	}
	memDC2.CreateCompatibleDC(&memDC);
	bmpIcon->CreateCompatibleBitmap(&memDC, defaultSize, defaultSize);
	pOldBmp2 = memDC2.SelectObject(bmpIcon);
	
	memDC2.StretchBlt(0, 0, defaultSize, defaultSize, &memDC, 0, 0, size.cx, size.cy, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();

	bmpTemp.DeleteObject();

	memDC2.SelectObject(pOldBmp2);
	memDC2.DeleteDC();

	return size;
}

CSize WindowIcon::GetIconSize(HICON hIcon)
{
	if (hIcon == nullptr)
		return {0, 0};

	int nWidth = 0;
	int nHeight = 0;
	ICONINFO iconInfo;
	::ZeroMemory(&iconInfo, sizeof(iconInfo));
	::GetIconInfo(hIcon, &iconInfo);

	BITMAP bmp;
	::ZeroMemory(&bmp, sizeof(bmp));
	if (iconInfo.hbmColor)
	{
		const int nWrittenBytes = GetObject(iconInfo.hbmColor, sizeof(bmp), &bmp);
		if (nWrittenBytes > 0)
		{
			nWidth = bmp.bmWidth;
			nHeight = bmp.bmHeight;
		}
	}
	else if (iconInfo.hbmMask)
	{
		// Icon has no color plane, image data stored in mask
		const int nWrittenBytes = GetObject(iconInfo.hbmMask, sizeof(bmp), &bmp);
		if (nWrittenBytes > 0)
		{
			nWidth = bmp.bmWidth;
			nHeight = bmp.bmHeight / 2;
			// myinfo.nBitsPerPixel = 1;
		}
	}
	if (iconInfo.hbmColor)
		DeleteObject(iconInfo.hbmColor);
	if (iconInfo.hbmMask)
		DeleteObject(iconInfo.hbmMask);

	return {nWidth, nHeight};
}
