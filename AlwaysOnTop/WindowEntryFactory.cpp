#include "stdafx.h"
#include "WindowEntryFactory.h"


WindowEntryFactory::WindowEntryFactory()
{

}


WindowEntryFactory::~WindowEntryFactory()
{
}


WindowEntry WindowEntryFactory::Create(HWND hWnd)
{
	std::wstring windowTitle = GetWindowTitle(hWnd);

	DWORD processId;
	::GetWindowThreadProcessId(hWnd, &processId);

	HICON iconHandle = WindowIcon::GetAppIcon(hWnd);

	BOOL isVisible = !::IsIconic(hWnd);
	
	return WindowEntry { hWnd, processId, windowTitle, iconHandle, isVisible };
}


std::wstring WindowEntryFactory::GetWindowTitle(HWND hWnd)
{
	int titleLength = ::GetWindowTextLengthW(hWnd);
	if (0 == titleLength)
		return std::wstring();

	wchar_t* title = new (std::nothrow) wchar_t[titleLength + 1]();
	if (nullptr == title)
		return std::wstring();
	::GetWindowTextW(hWnd, title, titleLength + 1);

	std::wstring result(title);
	delete[] title;
	
	return std::move(result);
}
