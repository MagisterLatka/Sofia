#include <pch.h>
#include "DX11Context.h"

#include "dxerr.h"

namespace Sofia {

	void DX11Context::Init()
	{
#if defined(SOF_DEBUG)
		s_DxgiInfoManager.Init();
#endif

		uint32_t deviceCreateFlags = 0u;
#if defined(SOF_DEBUG)
		deviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr;
		SOF_DX_GRAPHICS_CALL_INFO(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceCreateFlags, nullptr, 0, D3D11_SDK_VERSION,
			&m_Device, nullptr, &m_Context));
	}

#if defined(SOF_DEBUG)
	DxgiInfoManager DX11Context::s_DxgiInfoManager;
#endif

	DX11Context::GraphicsException::GraphicsException(int line, const char* file, HRESULT hresult, const std::string& infoMessage) noexcept
		: SofiaException(line, file, true), m_Result(hresult), m_Info(infoMessage) {}
	const char* DX11Context::GraphicsException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl;
		oss << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " (" << (uint32_t)m_Result << ")" << std::endl;
		oss << "[Error String] " << GetErrorString() << std::endl;
		oss << "[Description] " << GetErrorDescription() << std::endl;

		if (!m_Info.empty())
			oss << std::endl << "[Error Info]" << std::endl << m_Info << std::endl << std::endl;

		oss << GetOriginString();
		m_Buffer = oss.str();
		return m_Buffer.c_str();
	}
	std::string DX11Context::GraphicsException::GetErrorString() const noexcept
	{
		return DXGetErrorStringA(m_Result);
	}
	std::string DX11Context::GraphicsException::GetErrorDescription() const noexcept
	{
		return std::string();
	}
}
