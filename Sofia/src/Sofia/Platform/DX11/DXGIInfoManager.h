#pragma once

#include "Sofia/Core.h"
#include <dxgidebug.h>

class DxgiInfoManager
{
public:
	DxgiInfoManager() = default;
	~DxgiInfoManager() = default;
	DxgiInfoManager( const DxgiInfoManager& ) = delete;
	DxgiInfoManager& operator=( const DxgiInfoManager& ) = delete;

	void Init();
	void Set() noexcept;
	std::string GetMessages() const;
private:
	uint64_t m_Next = 0u;
	ComPtr<IDXGIInfoQueue> m_DxgiInfoQueue;
};
