#pragma once

namespace Sofia {

	class FileDialogs
	{
	public:
		static std::filesystem::path WindowsOpen(const wchar_t* filter);
		static std::filesystem::path WindowsSave(const wchar_t* filter);
	};
}
