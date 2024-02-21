#pragma once

#include "Sofia/Renderer/Shader.h"

namespace Sofia {

	class OpenGLShader : public Shader
	{
	public:
		SOF_CORE OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
		SOF_CORE OpenGLShader(const std::filesystem::path& path);
		SOF_CORE OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName);
		SOF_CORE ~OpenGLShader();

		SOF_CORE virtual void Reload() override;
		SOF_CORE virtual void Reload(const std::string& vertexSource, const std::string& fragmentSource) override;

		SOF_CORE virtual void Bind() const noexcept override;

		SOF_CORE virtual const std::string& GetName() const override { return m_Name; };

		SOF_CORE virtual void AddReloadedCallback(const ShaderReloadedCallback& callback) noexcept override;
	private:
		SOF_CORE void PreProcess();
		SOF_CORE void Compile();
	private:
		uint32_t m_ID;
		std::filesystem::path m_VPath, m_FPath;
		std::string m_Name, m_VSource, m_FSource;
		bool m_Loaded = false, m_UseFilepath = false;
		std::vector<ShaderReloadedCallback> m_ShaderReloadedCallback;
	};
}
