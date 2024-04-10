#include <pch.h>
#include "FileProcessing.h"

namespace Sofia {

	std::string FileProcessing::ReadFromFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios_base::in | std::ios_base::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
		}
		else
		{
			SOF_CORE_ASSERT(false, "Could not load from " + filepath);
		}
		in.close();
		return result;
	}
	std::string FileProcessing::ReadFromFile(const std::filesystem::path& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios_base::in | std::ios_base::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
		}
		else
		{
			SOF_CORE_ASSERT(false, "Could not load from " + filepath.string());
		}
		in.close();
		return result;
	}

#if defined(SOF_PLATFORM_WINDOWS)
	extern std::filesystem::path WindowsOpen(const wchar_t* filter);
	extern std::filesystem::path WindowsSave(const wchar_t* filter);

	std::filesystem::path FileProcessing::ChooseFileToOpenFrom(const wchar_t* filter)
	{
		return WindowsOpen(filter);
	}
	std::filesystem::path FileProcessing::ChooseFileToSaveTo(const wchar_t* filter)
	{
		return WindowsSave(filter);
	}
#else
	std::filesystem::path FileProcessing::ChooseFileToOpenFrom(const wchar_t* filter) //TODO
	{
		return filter;
	}
	std::filesystem::path FileProcessing::ChooseFileToSaveTo(const wchar_t* filter)
	{
		return filter;
	}
#endif
}
