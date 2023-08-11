#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class Shader : public RefCounted
	{
	public:
		using ShaderReloadedCallback = std::function<void()>;

		virtual ~Shader() = default;

		virtual void Reload() = 0;

		virtual void Bind() const noexcept = 0;

		virtual const std::string& GetName() const = 0;

		virtual void AddReloadedCallback(const ShaderReloadedCallback& callback) noexcept = 0;

		static Ref<Shader> Create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	};

	class ShaderLibrary : public RefCounted
	{
	public:
		ShaderLibrary() = default;
		~ShaderLibrary() = default;

		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Load(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
		void Load(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

		const Ref<Shader>& Get(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
