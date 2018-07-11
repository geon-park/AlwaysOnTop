#pragma once

#include <string>

class WindowEntry
{
public:
	WindowEntry(HWND, DWORD, std::wstring, HICON, BOOL);
	~WindowEntry();

public:
	HWND GetHWnd() const { return _HWnd; }
	void PutHWnd(HWND hWnd) { _HWnd = hWnd; }
	
	DWORD GetProcessId() const{ return _ProcessId; }
	void PutProcessId(DWORD processId) { _ProcessId = processId; }

	std::wstring GetProcessName() const { return _ProcessName; }
	void PutProcessName(std::wstring processName) { _ProcessName = processName; }

	std::wstring GetTitle() const { return _Title; }
	void PutTitle(std::wstring title) { _Title = title; }

	HICON GetIconHandle() const { return _IconHandle; }
	void PutIconHandle(HICON iconHandle) { _IconHandle = iconHandle; }

	BOOL GetIsVisible() const { return _IsVisible; }
	void PutIsVisible(BOOL isVisible) { _IsVisible = isVisible; }

	std::wstring GetFullProcessName() const { return _FullProcessName; }
	void PutFullProcessName(std::wstring fullProcessName) { _FullProcessName = fullProcessName; }
	
public:
	__declspec(property(get = GetHWnd, put = PutHWnd)) HWND HWnd;
	__declspec(property(get = GetProcessId, put = PutProcessId)) DWORD ProcessId;
	__declspec(property(get = GetProcessName, put = PutProcessName)) std::wstring ProcessName;
	__declspec(property(get = GetTitle, put = PutTitle)) std::wstring Title;
	__declspec(property(get = GetIconHandle, put = PutIconHandle)) HICON IconHandle;
	__declspec(property(get = GetIsVisible, put = PutIsVisible)) BOOL IsVisible;
	__declspec(property(get = GetFullProcessName, put = PutFullProcessName)) std::wstring FullProcessName;

private:
	HWND _HWnd;
	DWORD _ProcessId;
	std::wstring _ProcessName;
	std::wstring _Title;
	HICON _IconHandle;
	BOOL _IsVisible;
	std::wstring _FullProcessName;
};

