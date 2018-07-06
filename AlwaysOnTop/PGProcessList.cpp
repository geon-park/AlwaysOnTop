#include "stdafx.h"
#include "PGProcessList.h"

#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>
#include <algorithm>

PGProcessList::PGProcessList()
{
}

PGProcessList::~PGProcessList()
{
}

const std::vector<std::wstring> PGProcessList::WindowsClassNamesToSkip = {
	L"Shell_TrayWnd",
	L"DV2ControlHost",
	L"MsgrIMEWindowClass",
	L"SysShadow",
	L"Button"
};

std::vector<PGProcessList::ProcessInfo> PGProcessList::GetProcessList()
{
	std::vector<ProcessInfo> list;
	
	::EnumWindows([] (HWND hWnd, LPARAM lParam) -> BOOL {
		HWND lShellWindow = GetShellWindow();
		DWORD currentProcessId = GetCurrentProcessId();

		if (!EligibleForActivation(hWnd, lShellWindow))
			return TRUE;

		wchar_t szCaption[1024] = { 0, };
		::GetWindowTextW(hWnd, szCaption, 1024);
		DWORD processId;
		::GetWindowThreadProcessId(hWnd, &processId);
		BOOL isVisible = ::IsIconic(hWnd);

		if (processId == currentProcessId || wcslen(szCaption) == 0)
		{
			return TRUE;
		}

		std::wstring procName = GetProcessNameFromPID(processId);

		ProcessInfo info = { hWnd, processId, std::wstring(procName), std::wstring(szCaption), nullptr, isVisible };
		
		std::vector<ProcessInfo>* pList = reinterpret_cast<std::vector<ProcessInfo>*>(lParam);
		pList->push_back(info);

		return TRUE;

	}, (LPARAM)&list);

	// ::EnumWindows(EnumWindowsProc, (LPARAM)&list);
	
	std::sort(list.begin(), list.end(), [](ProcessInfo a, ProcessInfo b) {
		return a.Title.compare(b.Title) < 0;
	});
	
	return std::move(list);
}

/*
BOOL PGProcessList::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	wchar_t szCaption[1024] = { 0, };
	DWORD dwPID = 0;

	LONG dwStyle = ::GetWindowLongW(hWnd, GWL_STYLE);
	LONG dwExStyle = ::GetWindowLongW(hWnd, GWL_EXSTYLE);

	BOOL bVisible = ::IsWindowVisible(hWnd);
	BOOL bToolWindow = (dwExStyle & WS_EX_TOOLWINDOW);
	BOOL bAppWindow = (dwExStyle & WS_EX_APPWINDOW);
	// BOOL bOwned = ::GetWindow(hWnd, GW_OWNER) ? TRUE : FALSE;
	BOOL bOwned = (::GetParent(hWnd) != nullptr);
	BOOL bPopup = (dwStyle & WS_POPUP);
	BOOL bClipChildren = (dwStyle & WS_CLIPCHILDREN);
	int textLength = ::GetWindowTextLengthW(hWnd);

	::GetWindowText(hWnd, szCaption, 1024);
	std::wstring caption(szCaption);
	::GetWindowThreadProcessId(hWnd, &dwPID);

	// if (!(bVisible && !bOwned && !bPopup && (bAppWindow || !bToolWindow)))
	// if (!bVisible || bOwned || (bPopup && !bClipChildren) || (!bAppWindow && bToolWindow))
	if (!bVisible || bOwned || (!bAppWindow && bToolWindow) || textLength == 0)
		return TRUE;

	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwPID);
	::Process32First(hSnapshot, &pEntry);

	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPID);
	while ((dwPID != pEntry.th32ProcessID))
	{
		BOOL bRes = ::Process32Next(hSnapshot, &pEntry);
		if (FALSE == bRes)
			break;

		hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPID);
	}

	std::vector<ProcessInfo>* pList = reinterpret_cast<std::vector<ProcessInfo>*>(lParam);
	
	ProcessInfo temp = {szCaption, hWnd, dwPID};
	pList->push_back(temp);

	return TRUE;
}
*/

bool PGProcessList::EligibleForActivation(HWND hWnd, HWND lShellWindow)
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

	if (std::find(WindowsClassNamesToSkip.cbegin(), WindowsClassNamesToSkip.cend(), className) != WindowsClassNamesToSkip.cend())
	{
		return false;
	}

	/*
	if (className.StartsWith("WMP9MediaBarFlyout")) //WMP's "now playing" taskbar-toolbar
		return false;
	*/

	return true;
}

HWND PGProcessList::GetLastVisibleActivePopUpOfWindow(HWND window)
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

std::wstring PGProcessList::GetProcessNameFromPID(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
	DWORD length = MAX_PATH;
	wchar_t procName[MAX_PATH] = { 0, };
	if (FALSE == ::QueryFullProcessImageNameW(hProcess, 0, procName, &length))
		return std::wstring();
	
	std::wstring result = std::wstring(::PathFindFileNameW(procName));

	return result;
}
