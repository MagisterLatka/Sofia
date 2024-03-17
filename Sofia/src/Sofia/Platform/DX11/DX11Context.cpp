#include <pch.h>
#include "DX11Context.h"

#include "dxerr.h"
#include <dxgi1_3.h>

#include "Sofia/Platform/Windows/WindowsWindow.h"
#include "Sofia/Renderer/Renderer.h"

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

		ComPtr<ID3D11Debug> debug;
		hr = m_Device.As(&debug);
		if (SUCCEEDED(hr))
		{
			ComPtr<ID3D11InfoQueue> queue;
			hr = debug.As(&queue);
			if (SUCCEEDED(hr))
			{
				D3D11_MESSAGE_ID hide[] = {
					D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET
				};
				D3D11_INFO_QUEUE_FILTER filter = { 0 };
				filter.DenyList.NumIDs = _countof(hide);
				filter.DenyList.pIDList = hide;
				queue->AddStorageFilterEntries(&filter);
			}
		}
	}
	void DX11Context::InitForWindow(void* window)
	{
		WindowsWindow* wnd = (WindowsWindow*)window;
		auto& [swapChain, targetView] = m_WindowData[wnd->m_Window];

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Width = wnd->m_Data.width;
		swapChainDesc.BufferDesc.Height = wnd->m_Data.height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0u;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 0u;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1u;
		swapChainDesc.SampleDesc.Quality = 0u;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2u;
		swapChainDesc.OutputWindow = wnd->m_Window;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		ComPtr<IDXGIDevice3> dxgiDevice;
		ComPtr<IDXGIAdapter> adapter;
		ComPtr<IDXGIFactory> factory;
		m_Device.As(&dxgiDevice);
		HRESULT hr = dxgiDevice->GetAdapter(&adapter);
		if (SUCCEEDED(hr))
		{
			adapter->GetParent(IID_PPV_ARGS(&factory));
			SOF_DX_GRAPHICS_CALL_INFO(factory->CreateSwapChain(m_Device.Get(), &swapChainDesc, &swapChain));
		}

		ComPtr<ID3D11Texture2D> backBuffer;
		SOF_DX_GRAPHICS_CALL_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
		SOF_DX_GRAPHICS_CALL_INFO(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &targetView));
	}

	void DX11Context::ShutdownForWindow(void* window)
	{
		WindowsWindow* wnd = (WindowsWindow*)window;
		auto& [swapChain, targetView] = m_WindowData[wnd->m_Window];
		targetView.Reset();
		swapChain.Reset();
	}

	void DX11Context::SwapBuffers(void* window)
	{
		WindowsWindow* wnd = (WindowsWindow*)window;
		auto& [swapChain, targetView] = m_WindowData[wnd->m_Window];
		HRESULT hr;
		if (wnd->m_Data.vSync)
		{
			SOF_DX_GRAPHICS_CALL_INFO(swapChain->Present(1u, 0u));
		}
		else
		{
			SOF_DX_GRAPHICS_CALL_INFO(swapChain->Present(0u, DXGI_PRESENT_ALLOW_TEARING));
		}
	}
	void DX11Context::RecreateSwapChain(void* window)
	{
		WindowsWindow* wnd = (WindowsWindow*)window;
		auto& [swapChain, targetView] = m_WindowData[wnd->m_Window];

		if (swapChain && wnd->m_Data.width != 0 && wnd->m_Data.height != 0)
		{
			m_Context->OMSetRenderTargets(0u, nullptr, nullptr);
			targetView.Reset();
			HRESULT hr;
			SOF_DX_GRAPHICS_CALL_INFO(swapChain->ResizeBuffers(0u, 0u, 0u, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

			ComPtr<ID3D11Texture2D> backBuffer;
			SOF_DX_GRAPHICS_CALL_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
			SOF_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &targetView));
		}
	}
	void DX11Context::BindToRender(void* window)
	{
		Ref<DX11Context> instance = this;
		Renderer::Submit([instance, window]() mutable
		{
			WindowsWindow* wnd = (WindowsWindow*)window;
			auto& [swapChain, targetView] = instance->m_WindowData[wnd->m_Window];

			instance->m_Context->OMSetRenderTargets(1u, targetView.GetAddressOf(), nullptr);
			D3D11_VIEWPORT viewport;
			viewport.Width = (float)wnd->m_Data.width;
			viewport.Height = (float)wnd->m_Data.height;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			instance->m_Context->RSSetViewports(1u, &viewport);
		});
	}
	void DX11Context::Clear(void* window, const glm::vec4& color)
	{
		Ref<DX11Context> instance = this;
		Renderer::Submit([instance, window, color]() mutable
		{
			WindowsWindow* wnd = (WindowsWindow*)window;
			auto& [swapChain, targetView] = instance->m_WindowData[wnd->m_Window];

			instance->m_Context->ClearRenderTargetView(targetView.Get(), glm::value_ptr(color));
		});
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
