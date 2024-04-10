#include <pch.h>

#include <commdlg.h>

namespace Sofia {

	std::filesystem::path WindowsOpen(const wchar_t* filter)
	{
		OPENFILENAMEW ofn;
		WCHAR szFile[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetOpenFileNameW(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::filesystem::path();
	}
	std::filesystem::path WindowsSave(const wchar_t* filter)
	{
		OPENFILENAMEW ofn;
		WCHAR szFile[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_DONTADDTORECENT | OFN_NOCHANGEDIR | OFN_CREATEPROMPT;
		if (GetSaveFileNameW(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::filesystem::path();
	}
}
