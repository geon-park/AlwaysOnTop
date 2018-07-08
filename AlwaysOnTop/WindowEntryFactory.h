#pragma once

#include <memory>
#include <string>
#include "WindowEntry.h"
#include "WindowIcon.h"

class WindowEntryFactory
{
public:
	WindowEntryFactory();
	~WindowEntryFactory();

	static WindowEntry Create(HWND hWnd);
	static std::wstring GetWindowTitle(HWND hWnd);
};

