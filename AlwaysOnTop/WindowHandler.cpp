#include "stdafx.h"
#include "WindowHandler.h"

#include <Windows.h>

WindowHandler::WindowHandler()
{
}

WindowHandler::~WindowHandler()
{
}

bool WindowHandler::SetTopMostWindow(HWND hWnd)
{
	::SetForegroundWindow(hWnd);

	::SetWindowLongPtrW(hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	return true;
}

bool WindowHandler::SetNoTopMostWindow(HWND hWnd)
{
	::SetForegroundWindow(hWnd);

	::SetWindowLongPtr(hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hWnd, GWL_EXSTYLE) & ~WS_EX_TOPMOST);
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	return true;
}

bool WindowHandler::CloseWindow(HWND hWnd)
{
	::SendMessageW(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	return true;
}

bool WindowHandler::MinimizeWindow(HWND hWnd)
{
	::ShowWindow(hWnd, SW_MINIMIZE);
	return true;
}

bool WindowHandler::MaximizeWindow(HWND hWnd)
{
	::ShowWindow(hWnd, SW_MAXIMIZE);
	return true;
}
