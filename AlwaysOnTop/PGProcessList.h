#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <map>

class PGProcessList
{
public:
	struct ProcessInfo {
		HWND hWnd;
		DWORD ProcessId;
		std::wstring ProcessName;
		std::wstring Title;
		HWND IconHandle;
		BOOL IsVisible;
	};
	
private:
	static const int MaxLastActivePopupIterations = 50;
	static const std::vector<std::wstring> WindowsClassNamesToSkip;

public:
	PGProcessList();
	~PGProcessList();

	std::vector<ProcessInfo> GetProcessList();

	// static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static bool EligibleForActivation(HWND hWnd, HWND lShellWindow);
	static HWND GetLastVisibleActivePopUpOfWindow(HWND window);
	static std::wstring GetProcessNameFromPID(DWORD processId);
};