#include "stdafx.h"
#include "PGProcessList.h"

#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>

PGProcessList::PGProcessList()
{
}


PGProcessList::~PGProcessList()
{
}

std::vector<std::wstring> PGProcessList::GetProcessList()
{
	// ::EnumWindows(EnumWindowsProc, (LPARAM)nullptr);
	return std::vector<std::wstring>();
}