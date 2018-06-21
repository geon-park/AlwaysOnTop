#pragma once

#include <vector>
#include <string>

class PGProcessList
{
public:
	struct ProcessInfo {

	};

public:
	PGProcessList();
	~PGProcessList();


	std::vector<std::wstring> GetProcessList();
	// static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

private:
	
};

