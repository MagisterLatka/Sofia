#include <pch.h>
#include "DX11Context.h"

#include "dxerr.h"

namespace Sofia {

	DX11Context::~DX11Context()
	{
#if defined(SOF_DEBUG)
		ComPtr<ID3D11Debug> debug;
		m_Device->QueryInterface(IID_PPV_ARGS(&debug));
		debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_IGNORE_INTERNAL);
#endif
	}
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
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;
		SOF_DX_GRAPHICS_CALL_INFO(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceCreateFlags, nullptr, 0, D3D11_SDK_VERSION,
													&device, nullptr, &context));
		device.As<ID3D11Device5>(&m_Device);
		context.As<ID3D11DeviceContext4>(&m_Context);

		ComPtr<ID3D11Debug> debug;
		hr = m_Device.As(&debug);
		if (SUCCEEDED(hr))
		{
			ComPtr<ID3D11InfoQueue> queue;
			hr = debug.As(&queue);
			if (SUCCEEDED(hr))
			{
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET
				};
				D3D11_INFO_QUEUE_FILTER filter = { 0 };
				filter.DenyList.NumIDs = _countof(hide);
				filter.DenyList.pIDList = hide;
				queue->AddStorageFilterEntries(&filter);
			}
		}
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
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
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
