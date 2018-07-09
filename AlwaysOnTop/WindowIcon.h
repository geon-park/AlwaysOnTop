#pragma once
class WindowIcon
{
public:
	WindowIcon();
	~WindowIcon();

public:
	static HICON GetAppIcon(HWND hWnd);
	static CSize HIconToCBitmap(HICON hIcon, CBitmap* bmpIcon, const int defaultSize = 0x20);

private:
	static CSize GetIconSize(HICON hIcon);
};
