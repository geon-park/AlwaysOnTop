#pragma once
class WindowIcon
{
public:
	WindowIcon();
	~WindowIcon();

public:
	static HICON GetAppIcon(HWND hWnd);
};

