#include <pch.h>
#include "DX11Shader.h"

#include "Sofia/Platform/DX11/DX11Context.h"
#include "Sofia/Renderer/Renderer.h"
#include "Sofia/Utils/FileProcessing.h"
#include <d3dcompiler.h>

namespace Sofia {

	static std::string GetNameFromFilepath(const std::filesystem::path& filepath)
	{
		std::string path = filepath.string();
		std::string name;
		uint64_t found = path.find_last_of("/\\");
		name = found != std::string::npos ? path.substr(found + 1) : path;
		found = name.rfind(".vert.cso");
		if (found == std::string::npos)
			found = name.rfind(".hshader");
		name = found != std::string::npos ? name.substr(0, found) : name;
		return name;
	}
	DX11Shader::DX11Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
		: m_VPath(vertexPath), m_FPath(fragmentPath), m_Name(GetNameFromFilepath(vertexPath)), m_UseFilepath(true)
	{
		Reload();
	}
	DX11Shader::DX11Shader(const std::filesystem::path& path)
		: m_VPath(path), m_Name(GetNameFromFilepath(path))
	{
		Reload();
	}
	DX11Shader::DX11Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName)
		: m_Name(debugName)
	{
		Reload(vertexSource, fragmentSource);
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
			else if (!instance->m_VPath.empty())
			{
				instance->PreProcess();
				instance->CompileFromSource();
			}
			else return;

			if (instance->m_Loaded)
			{
				for (auto& callback : instance->m_ShaderReloadedCallback)
					callback();
			}
			instance->m_Loaded = true;
		});
	}
	void DX11Shader::Reload(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_VsSource = vertexSource;
		m_FsSource = fragmentSource;

		Ref<DX11Shader> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->CompileFromSource();

			if (instance->m_Loaded)
			{
				for (auto& callback : instance->m_ShaderReloadedCallback)
					callback();
			}
			instance->m_Loaded = true;
		});
	}
	void DX11Shader::PreProcess()
	{
		std::string source = FileProcessing::ReadFromFile(m_VPath);

		const char* typeToken = "#type";
		uint64_t typeTokenLength = strlen(typeToken);
		uint64_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			uint64_t eol = source.find_first_of("\r\n", pos);
			SOF_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			uint64_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			SOF_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type");

			uint64_t nextLinePos = source.find_first_not_of("\r\n", eol);
			SOF_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);
			if (type == "vertex")
				m_VsSource = source.substr(nextLinePos, pos == std::string::npos ? std::string::npos : pos - nextLinePos);
			else
				m_FsSource = source.substr(nextLinePos, pos == std::string::npos ? std::string::npos : pos - nextLinePos);
		}
	}
	void DX11Shader::CompileFromSource()
	{
		HRESULT hr;
		auto device = DX11Context::GetContextFromApplication()->GetDevice();

		uint32_t compileFlags = 0u;
#if defined(SOF_DEBUG)
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

		ID3DBlob* errorMsgs = nullptr;
		D3DCompile(m_FsSource.c_str(), m_FsSource.length(), nullptr, nullptr, nullptr, "main", "ps_5_0", compileFlags, 0u, &m_VSource, &errorMsgs);
		if (errorMsgs != nullptr)
		{
			std::string errors = "Vertex shader compilation failed: ";
			errors.append((const char*)errorMsgs->GetBufferPointer(), errorMsgs->GetBufferSize());
			SOF_CORE_THROW_INFO(errors);
			errorMsgs->Release();
		}
		SOF_DX_GRAPHICS_CALL_INFO(device->CreatePixelShader(m_VSource->GetBufferPointer(), m_VSource->GetBufferSize(), nullptr, &m_FShader));

		D3DCompile(m_VsSource.c_str(), m_VsSource.length(), nullptr, nullptr, nullptr, "main", "vs_5_0", compileFlags, 0u, &m_VSource, &errorMsgs);
		if (errorMsgs != nullptr)
		{
			std::string errors = "Fragment shader compilation failed: ";
			errors.append((const char*)errorMsgs->GetBufferPointer(), errorMsgs->GetBufferSize());
			SOF_CORE_THROW_INFO(errors);
			errorMsgs->Release();
		}
		SOF_DX_GRAPHICS_CALL_INFO(device->CreateVertexShader(m_VSource->GetBufferPointer(), m_VSource->GetBufferSize(), nullptr, &m_VShader));

		m_VsSource.clear();
		m_FsSource.clear();
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

	void DX11Shader::AddReloadedCallback(const ShaderReloadedCallback& callback) noexcept
	{
		m_ShaderReloadedCallback.push_back(callback);
	}
}
