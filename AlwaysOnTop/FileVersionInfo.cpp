#include "stdafx.h"
#include "FileVersionInfo.h"

#include <memory>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#pragma comment(lib, "Mincore.lib")

FileVersionInfo::~FileVersionInfo()
{
	Close();
}

bool FileVersionInfo::Open(const wchar_t* const module_name)
{
	return Open(std::wstring(module_name));
}

bool FileVersionInfo::Open(std::wstring module_name)
{
	if (module_name.length() == 0)
		return false;

	DWORD handle = 0;
	const auto data_size = ::GetFileVersionInfoSizeW(module_name.c_str(), &handle);
	if (data_size == 0)
		return false;
	
	version_info_data_ = new (std::nothrow) unsigned char[data_size];
	if (nullptr == version_info_data_)
		return false;

	if (!::GetFileVersionInfoW(module_name.c_str(), handle, data_size,
		reinterpret_cast<void**>(version_info_data_)))
	{
		Close();
		return false;
	}
	UINT query_size;
	LangAndCodePage* trans_table = nullptr;
	if (!::VerQueryValueW(version_info_data_, L"\\VarFileInfo\\Translation",
		reinterpret_cast<void**>(&trans_table), &query_size))
	{
		Close();
		return false;
	}
	for (int i = 0 ; i < (query_size / sizeof(LangAndCodePage)) ; ++i)
	{
		lang_and_code_page_.push_back(trans_table[0]);
	}
	return true;
}

size_t FileVersionInfo::GetNumOfLangCode() const
{
	if (version_info_data_ == nullptr)
		return 0;
	return lang_and_code_page_.size();
}

bool FileVersionInfo::GetFixedInfo(VS_FIXEDFILEINFO& vs_ffi) const
{
	if (version_info_data_ == nullptr)
		return false;

	UINT query_size;
	VS_FIXEDFILEINFO* p_vs_ffi;
	if (::VerQueryValueW(reinterpret_cast<void**>(version_info_data_), L"\\",
		reinterpret_cast<void**>(&p_vs_ffi), &query_size))
	{
		vs_ffi = *p_vs_ffi;
		return true;
	}
	return false;
}

std::wstring FileVersionInfo::GetFixedFileVersion() const
{
	std::wstringstream ss;
	VS_FIXEDFILEINFO vsffi;
	if (GetFixedInfo(vsffi))
	{
		ss << HIWORD(vsffi.dwFileVersionMS) << L", " << LOWORD(vsffi.dwFileVersionMS) << L", " <<
			HIWORD(vsffi.dwFileVersionLS) << L", " << LOWORD(vsffi.dwFileVersionLS);
	}
	return ss.str();
}

std::wstring FileVersionInfo::GetFixedProductVersion() const
{
	std::wstringstream ss;
	VS_FIXEDFILEINFO vsffi;
	if (GetFixedInfo(vsffi))
	{
		ss << HIWORD(vsffi.dwProductVersionMS) << L", " << LOWORD(vsffi.dwProductVersionMS) << L", " <<
			HIWORD(vsffi.dwProductVersionLS) << L", " << LOWORD(vsffi.dwProductVersionLS);
	}
	return ss.str();
}

void FileVersionInfo::Close()
{
	delete[] version_info_data_;
	version_info_data_ = nullptr;

	lang_and_code_page_.clear();
}

std::wstring FileVersionInfo::QueryValue(std::wstring value_name, size_t lang_index) const
{
	if (version_info_data_ == nullptr)
		return L"";

	LangAndCodePage lang_info;
	if (lang_and_code_page_.size() <= lang_index)
		lang_info = lang_and_code_page_[0];
	else
		lang_info = lang_and_code_page_[lang_index];

	UINT query_size = 0;
	void* lp_data = nullptr;
	std::wstring info_value;

	std::wstringstream ss;
	ss << L"\\StringFileInfo\\" << 
		std::setfill(L'0') << std::setw(4) << std::hex << lang_info.language <<
		std::setfill(L'0') << std::setw(4) << std::hex << lang_info.code_page <<
		L"\\" << value_name;
	std::wstring block_name = ss.str();

	if (::VerQueryValueW(reinterpret_cast<void**>(version_info_data_), block_name.c_str(), &lp_data, &query_size))
		info_value = reinterpret_cast<wchar_t*>(lp_data);
	return info_value;
}
