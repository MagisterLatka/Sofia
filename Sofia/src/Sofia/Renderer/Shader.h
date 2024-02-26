#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class Shader : public RefCounted
	{
	public:
		using ShaderReloadedCallback = std::function<void()>;

		SOF_CORE virtual ~Shader() = default;

		SOF_CORE virtual void Reload() = 0;
		SOF_CORE virtual void Reload(const std::string& vertexSource, const std::string& fragmentSource) = 0;
		SOF_CORE virtual void Bind() const noexcept = 0;

		SOF_CORE virtual const std::string& GetName() const = 0;
		SOF_CORE virtual void AddReloadedCallback(const ShaderReloadedCallback& callback) noexcept = 0;

		SOF_CORE static Ref<Shader> Create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
		SOF_CORE static Ref<Shader> Create(const std::filesystem::path& path);
		SOF_CORE static Ref<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName = {});
	};

	class ShaderLibrary : public RefCounted
	{
	public:
		SOF_CORE ShaderLibrary() = default;
		SOF_CORE ~ShaderLibrary() = default;

		SOF_CORE Ref<Shader> Add(const Ref<Shader>& shader);
		SOF_CORE Ref<Shader> Add(const std::string& name, const Ref<Shader>& shader);
		SOF_CORE Ref<Shader> Load(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
		SOF_CORE Ref<Shader> Load(const std::filesystem::path& path);
		SOF_CORE Ref<Shader> Load(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
		SOF_CORE Ref<Shader> Load(const std::string& name, const std::filesystem::path& path);
		SOF_CORE Ref<Shader> Load(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);

		SOF_CORE const Ref<Shader>& Get(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
