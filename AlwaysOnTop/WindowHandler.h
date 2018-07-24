#pragma once

class WindowHandler
{
public:
	WindowHandler();
	~WindowHandler();

	static bool SetTopMostWindow(HWND hWnd);
	static bool SetNoTopMostWindow(HWND hWnd);
	static bool CloseWindow(HWND hWnd);
	static bool MinimizeWindow(HWND hWnd);
	static bool MaximizeWindow(HWND hWnd);
};
