#include "stdafx.h"
#include "WindowEntry.h"


WindowEntry::WindowEntry(HWND hWnd, DWORD processId, std::wstring title, HICON iconHandle, BOOL isVisible) : 
	_HWnd(hWnd), _ProcessId(processId), _Title(title), _IconHandle(iconHandle), _IsVisible(isVisible)
{
}


WindowEntry::~WindowEntry()
{
}
