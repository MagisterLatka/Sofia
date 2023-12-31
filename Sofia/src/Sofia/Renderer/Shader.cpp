#include <pch.h>
#include "Shader.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Platform/DX11/DX11Shader.h"

namespace Sofia {

	Ref<Shader> Shader::Create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
			case RendererAPI::API::DX11:	return Ref<DX11Shader>::Create(vertexPath, fragmentPath);
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); return nullptr;
		case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
		case RendererAPI::API::DX11:	return Ref<DX11Shader>::Create(vertexSource, fragmentSource, debugName);
		case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
		}

		SOF_CORE_THROW_INFO("Unknown API");
		return nullptr;
	}


	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		SOF_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader {0} already exist", name);
		m_Shaders[name] = shader;
	}
	void ShaderLibrary::Add(const std::string & name, const Ref<Shader>&shader)
	{
		SOF_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader {0} already exist", name);
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		Ref<Shader> shader = Shader::Create(vertexPath, fragmentPath);
		const std::string& name = shader->GetName();
		SOF_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader {0} already exist", name);
		m_Shaders[name] = shader;
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string & name, const std::filesystem::path & vertexPath, const std::filesystem::path & fragmentPath)
	{
		SOF_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader {0} already exist", name);
		Ref<Shader> shader = Shader::Create(vertexPath, fragmentPath);
		m_Shaders[name] = shader;
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		SOF_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader {0} already exist", name);
		Ref<Shader> shader = Shader::Create(vertexSource, fragmentSource, name);
		m_Shaders[name] = shader;
		return shader;
	}

	const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const
	{
		auto shader = m_Shaders.find(name);
		SOF_CORE_ASSERT(shader != m_Shaders.end(), "Shader {0} doesn't exist", name);
		return shader->second;
	}
}
