#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class FileProcessing
	{
	public:
		SOF_CORE static std::string ReadFromFile(const std::string& filepath);
		SOF_CORE static std::string ReadFromFile(const std::filesystem::path& filepath);
		SOF_CORE static std::filesystem::path ChooseFileToOpenFrom(const wchar_t* filter = nullptr);
		SOF_CORE static std::filesystem::path ChooseFileToSaveTo(const wchar_t* filter = nullptr);
	};
}
