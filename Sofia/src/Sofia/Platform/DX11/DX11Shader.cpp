#include <pch.h>
#include "DX11Shader.h"

#include "Sofia/Platform/DX11/DX11Context.h"
#include "Sofia/Renderer/Renderer.h"
#include <d3dcompiler.h>

namespace Sofia {

	static std::string GetNameFromFilepath(const std::filesystem::path& filepath)
	{
		std::string path = filepath.string();
		std::string name;
		uint64_t found = path.find_last_of("/\\");
		name = found != std::string::npos ? path.substr(found + 1) : path;
		found = name.find_last_of(".vert.cso");
		name = found != std::string::npos ? name.substr(0, found) : name;
		return name;
	}
	DX11Shader::DX11Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
		: m_VPath(vertexPath), m_FPath(fragmentPath), m_Name(GetNameFromFilepath(vertexPath))
	{
		Reload();
	}
	DX11Shader::~DX11Shader()
	{
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader> fragmentShader;
		ComPtr<ID3DBlob> vertexSource;
		Renderer::Submit([vertexShader, fragmentShader, vertexSource]() mutable
		{
			vertexShader.Reset();
			fragmentShader.Reset();
			vertexSource.Reset();
		});
	}

	void DX11Shader::Reload()
	{
		Ref<DX11Shader> instance = this;
		Renderer::Submit([instance]() mutable
		{
			HRESULT hr;
			auto device = DX11Context::GetContextFromApplication()->GetDevice();
			SOF_DX_GRAPHICS_CALL_INFO(D3DReadFileToBlob(instance->m_FPath.c_str(), &instance->m_VSource));
			SOF_DX_GRAPHICS_CALL_INFO(device->CreatePixelShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_FShader));
			SOF_DX_GRAPHICS_CALL_INFO(D3DReadFileToBlob(instance->m_VPath.c_str(), &instance->m_VSource));
			SOF_DX_GRAPHICS_CALL_INFO(device->CreateVertexShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_VShader));

			if (instance->m_Loaded)
			{
				for (auto& callback : instance->m_ShaderReloadedCallback)
					callback();
			}
			instance->m_Loaded = true;
		});
	}

	void DX11Shader::Bind() const noexcept
	{
		Ref<const DX11Shader> instance = this;
		Renderer::Submit([instance]()
		{
			auto context = DX11Context::GetContextFromApplication()->GetContext();
			context->VSSetShader(instance->m_VShader.Get(), nullptr, 0u);
			context->PSSetShader(instance->m_FShader.Get(), nullptr, 0u);
		});
	}

	void DX11Shader::AddReloadedCallback(const ShaderReloadedCallback & callback) noexcept
	{
		m_ShaderReloadedCallback.push_back(callback);
	}
}
