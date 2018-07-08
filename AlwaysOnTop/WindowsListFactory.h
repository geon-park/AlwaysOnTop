#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <map>

#include "WindowEntryFactory.h"

class WindowsListFactory
{
public:
	WindowsListFactory();
	~WindowsListFactory();

	std::vector<WindowEntry> GetProcessList();

	static bool EligibleForActivation(HWND hWnd, HWND lShellWindow);
	static HWND GetLastVisibleActivePopUpOfWindow(HWND window);
	static bool IsKnownException(WindowEntry window);
	static std::wstring GetProcessNameFromPID(DWORD processId);

private:
	static const int MaxLastActivePopupIterations = 50;
	static const std::vector<std::wstring> WindowsClassNamesToSkip;
};