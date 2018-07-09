#include "stdafx.h"
#include "WindowsListFactory.h"

#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>
#include <algorithm>

WindowsListFactory::WindowsListFactory()
{
}

WindowsListFactory::~WindowsListFactory()
{
}

const std::vector<std::wstring> WindowsListFactory::WindowsClassNamesToSkip = {
	L"Shell_TrayWnd",
	L"DV2ControlHost",
	L"MsgrIMEWindowClass",
	L"SysShadow",
	L"Button"
};

std::list<WindowEntry> WindowsListFactory::GetProcessList()
{
	std::list<WindowEntry> list;
	
	::EnumWindows([] (HWND hWnd, LPARAM lParam) -> BOOL {
		HWND lShellWindow = GetShellWindow();
		DWORD currentProcessId = GetCurrentProcessId();

		if (!EligibleForActivation(hWnd, lShellWindow))
			return TRUE;

		WindowEntry window = WindowEntryFactory::Create(hWnd);
		if (window.ProcessId == currentProcessId || window.Title.length() == 0)
		{
			return TRUE;
		}

		window.ProcessName = GetProcessNameFromPID(window.ProcessId);

		if (IsKnownException(window))
			return TRUE;

		std::list<WindowEntry>* pList = reinterpret_cast<std::list<WindowEntry>*>(lParam);
		pList->push_back(window);

		return TRUE;

	}, (LPARAM)&list);
	
	/*std::sort(list.begin(), list.end(), [](WindowEntry a, WindowEntry b) {
		return a.HWnd < b.HWnd;
	});
	*/
	list.sort([](WindowEntry a, WindowEntry b) {
		return a.HWnd < b.HWnd;
	});
	
	return std::move(list);
}

bool WindowsListFactory::EligibleForActivation(HWND hWnd, HWND lShellWindow)
{
	// http://stackoverflow.com/questions/210504/enumerate-windows-like-alt-tab-does

	if (hWnd == lShellWindow)
		return false;

	HWND root = ::GetAncestor(hWnd, GA_ROOTOWNER);

	if (GetLastVisibleActivePopUpOfWindow(root) != hWnd)
		return false;

	wchar_t className[MAX_PATH] = { 0, };
	auto length = GetClassNameW(hWnd, className, MAX_PATH);
	if (length == 0)
		return false;

	/*if (std::find(WindowsClassNamesToSkip.cbegin(), WindowsClassNamesToSkip.cend(), className) != WindowsClassNamesToSkip.cend())
	{
		return false;
	}

	/*
	if (className.StartsWith("WMP9MediaBarFlyout")) //WMP's "now playing" taskbar-toolbar
		return false;
	*/

	return true;
}

HWND WindowsListFactory::GetLastVisibleActivePopUpOfWindow(HWND window)
{
	int level = MaxLastActivePopupIterations;
	HWND currentWindow = window;
	while (level-- > 0)
	{
		HWND lastPopUp = GetLastActivePopup(currentWindow);

		if (IsWindowVisible(lastPopUp))
			return lastPopUp;

		if (lastPopUp == currentWindow)
			return nullptr;

		currentWindow = lastPopUp;
	}
	return nullptr;
}

bool WindowsListFactory::IsKnownException(WindowEntry window)
{
	if (window.ProcessName.compare(L"Fiddler") == 0 && window.Title.compare(L"SSFiddlerMsgWin") == 0)
		return true;

	return false;
}

std::wstring WindowsListFactory::GetProcessNameFromPID(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
	DWORD length = MAX_PATH;
	wchar_t procName[MAX_PATH] = { 0, };
	if (FALSE == ::QueryFullProcessImageNameW(hProcess, 0, procName, &length))
		return std::wstring();
	
	std::wstring result = std::wstring(::PathFindFileNameW(procName));

	return result;
}
