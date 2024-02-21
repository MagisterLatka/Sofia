#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class FileProcessing
	{
	public:
		SOF_CORE static std::string ReadFromFile(const std::string& filepath);
		SOF_CORE static std::string ReadFromFile(const std::filesystem::path& filepath);
	};
}
