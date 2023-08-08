#pragma once

#include "Sofia/Renderer/GraphicsContext.h"
#include "Sofia/Platform/DX11/dxgiInfoManager.h"
#include "Sofia/Application.h"
#include <d3d11.h>

namespace Sofia {

	class DX11Context : public GraphicsContext
	{
#if defined(SOF_DEBUG)
	public:
		static DxgiInfoManager s_DxgiInfoManager;
#endif	
	public:
		class GraphicsException : public SofiaException
		{
		public:
			GraphicsException(int line, const char* file, HRESULT hresult, const std::string& infoMessage = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override { return "Graphics exception"; }
			HRESULT GetErrorCode() const noexcept { return m_Result; }
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
			const std::string& GetErrorInfo() const noexcept { return m_Info; }
		private:
			HRESULT m_Result;
			std::string m_Info;
		};
	public:
		DX11Context() = default;
		~DX11Context() = default;

		virtual void Init() override;

		ComPtr<ID3D11Device> GetDevice()		{ SOF_CORE_ASSERT(m_Device,  "No DX11Device created"); return m_Device; }
		ComPtr<ID3D11DeviceContext> GetContext(){ SOF_CORE_ASSERT(m_Context, "No DX11Device created"); return m_Context; }

		static Ref<DX11Context> GetContextFromApplication() noexcept { return Application::Get().GetGraphicsContext().As<DX11Context>(); }
	private:
		ComPtr<ID3D11Device> m_Device;
		ComPtr<ID3D11DeviceContext> m_Context;
	};
}

#define SOF_DX_GRAPHICS_EXCEPT(hr) Sofia::DX11Context::GraphicsException(__LINE__, __FILE__, hr)
#define SOF_DX_GRAPHICS_CALL(func) { if (FAILED(hr = (func))) throw SOF_DX_GRAPHICS_EXCEPT(hr); }

#if defined(SOF_DEBUG)
#define SOF_DX_GRAPHICS_CALL_INFO(func) { Sofia::DX11Context::s_DxgiInfoManager.Set(); if (FAILED(hr = (func)))\
								throw Sofia::DX11Context::GraphicsException(__LINE__, __FILE__, hr, Sofia::DX11Context::s_DxgiInfoManager.GetMessages()); }
#define SOF_DX_GRAPHICS_CALL_INFO_ONLY(func) { Sofia::DX11Context::s_DxgiInfoManager.Set(); (func); { auto str = Sofia::DX11Context::s_DxgiInfoManager.GetMessages();\
								if (!str.empty()) throw Sofia::MessageException(__LINE__, __FILE__, str); } }
#else
#define SOF_DX_GRAPHICS_CALL_INFO(func) { if (FAILED(hr = (func))) throw Sofia::DX11Context::GraphicsException(__LINE__, __FILE__, hr); }
#define SOF_DX_GRAPHICS_CALL_INFO_ONLY(func) (func)
#endif
