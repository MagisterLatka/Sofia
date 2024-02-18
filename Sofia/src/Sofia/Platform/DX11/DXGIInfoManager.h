#pragma once

#include "Sofia/Core.h"
#include <dxgidebug.h>

class DxgiInfoManager
{
public:
	SOF_CORE DxgiInfoManager() = default;
	SOF_CORE ~DxgiInfoManager() = default;
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

	SOF_CORE void Init();
	SOF_CORE void Set() noexcept;
	SOF_CORE std::string GetMessages() const;
private:
	uint64_t m_Next = 0u;
	ComPtr<IDXGIInfoQueue> m_DxgiInfoQueue;
};
