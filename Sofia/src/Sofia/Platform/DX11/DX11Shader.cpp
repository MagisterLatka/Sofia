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
		found = name.find(".vert.cso");
		name = found != std::string::npos ? name.substr(0, found) : name;
		return name;
	}
	DX11Shader::DX11Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
		: m_VPath(vertexPath), m_FPath(fragmentPath), m_UseFilepath(true), m_Name(GetNameFromFilepath(vertexPath))
	{
		Reload();
	}
	DX11Shader::DX11Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName)
		: m_VString(vertexSource), m_FString(fragmentSource), m_UseFilepath(false), m_Name(debugName)
	{
		Reload();
	}
	

	void DX11Shader::Reload()
	{
		Ref<DX11Shader> instance = this;
		Renderer::Submit([instance]() mutable
		{
			HRESULT hr;
			auto device = DX11Context::GetContextFromApplication()->GetDevice();
			if (instance->m_UseFilepath)
			{
				SOF_DX_GRAPHICS_CALL_INFO(D3DReadFileToBlob(instance->m_FPath.c_str(), &instance->m_VSource));
				SOF_DX_GRAPHICS_CALL_INFO(device->CreatePixelShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_FShader));
				SOF_DX_GRAPHICS_CALL_INFO(D3DReadFileToBlob(instance->m_VPath.c_str(), &instance->m_VSource));
				SOF_DX_GRAPHICS_CALL_INFO(device->CreateVertexShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_VShader));
			}
			else
			{
				uint32_t compileFlags = 0u;
#if defined(SOF_DEBUG)
				compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
				compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
				ID3DBlob* errorMsgs = nullptr;
				D3DCompile(instance->m_FString.c_str(), instance->m_FString.length(), nullptr, nullptr, nullptr, "main", "ps_5_0", compileFlags, 0u, &instance->m_VSource, &errorMsgs);
				if (errorMsgs != nullptr)
				{
					std::string errors = "Fragment shader compilation failed: ";
					errors.append((const char*)errorMsgs->GetBufferPointer(), errorMsgs->GetBufferSize());
					SOF_CORE_THROW_INFO(errors);
					errorMsgs->Release();
				}
				SOF_DX_GRAPHICS_CALL_INFO(device->CreatePixelShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_FShader));

				D3DCompile(instance->m_VString.c_str(), instance->m_VString.length(), nullptr, nullptr, nullptr, "main", "vs_5_0", compileFlags, 0u, &instance->m_VSource, &errorMsgs);
				if (errorMsgs != nullptr)
				{
					std::string errors = "Fragment shader compilation failed: ";
					errors.append((const char*)errorMsgs->GetBufferPointer(), errorMsgs->GetBufferSize());
					SOF_CORE_THROW_INFO(errors);
					errorMsgs->Release();
				}
				SOF_DX_GRAPHICS_CALL_INFO(device->CreateVertexShader(instance->m_VSource->GetBufferPointer(), instance->m_VSource->GetBufferSize(), nullptr, &instance->m_VShader));
			}

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
