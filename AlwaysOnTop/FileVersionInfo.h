#ifndef FILEVERSIONINFO_H
#define FILEVERSIONINFO_H

#pragma once

#include <vector>
#include <string>

class FileVersionInfo
{
private:
	struct LangAndCodePage
	{
		WORD language;
		WORD code_page;
	};
public:
	FileVersionInfo() = default;
	~FileVersionInfo();

	bool Open(const wchar_t* const module_name);
	bool Open(std::wstring module_name);

	size_t GetNumOfLangCode() const;

	std::wstring GetFileDescription(size_t lang_index = 0) const { return QueryValue(L"FileDescription", lang_index); }
	std::wstring GetFileVersion(size_t lang_index = 0) const { return QueryValue(L"FileVersion", lang_index); }
	std::wstring GetInternalName(size_t lang_index = 0) const { return QueryValue(L"InternalName", lang_index); }
	std::wstring GetCompanyName(size_t lang_index = 0) const { return QueryValue(L"CompanyName", lang_index); }
	std::wstring GetLegalCopyright(size_t lang_index = 0) const { return QueryValue(L"LegalCopyright", lang_index); }
	std::wstring GetOriginalFilename(size_t lang_index = 0) const { return QueryValue(L"OriginalFilename", lang_index); }
	std::wstring GetProductName(size_t lang_index = 0) const { return QueryValue(L"ProductName", lang_index); }
	std::wstring GetProductVersion(size_t lang_index = 0) const { return QueryValue(L"ProductVersion", lang_index); }

	bool GetFixedInfo(VS_FIXEDFILEINFO& vs_ffi) const;
	std::wstring GetFixedFileVersion() const;
	std::wstring GetFixedProductVersion() const;

private:
	void Close();
	std::wstring QueryValue(std::wstring value_name, size_t lang_index = 0) const;

private:
	unsigned char* version_info_data_ = nullptr;
	std::vector<LangAndCodePage> lang_and_code_page_;
};

#endif