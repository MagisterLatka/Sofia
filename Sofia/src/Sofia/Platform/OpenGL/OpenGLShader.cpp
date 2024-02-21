#include <pch.h>
#include "OpenGLShader.h"

#include "Sofia/Renderer/Renderer.h"
#include "Sofia/Utils/FileProcessing.h"
#include <glad/glad.h>

namespace Sofia {

	static std::string GetNameFromFilepath(const std::filesystem::path& filepath)
	{
		std::string path = filepath.string();
		std::string name;
		uint64_t found = path.find_last_of("/\\");
		name = found != std::string::npos ? path.substr(found + 1) : path;
		found = name.rfind(".");
		name = found != std::string::npos ? name.substr(0, found) : name;
		return name;
	}
	OpenGLShader::OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
		: m_VPath(vertexPath), m_FPath(fragmentPath), m_Name(GetNameFromFilepath(vertexPath)), m_UseFilepath(true)
	{
		Reload();
	}
	OpenGLShader::OpenGLShader(const std::filesystem::path& path)
		: m_VPath(path), m_Name(GetNameFromFilepath(path))
	{
		Reload();
	}
	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& debugName)
		: m_Name(debugName)
	{
		Reload(vertexSource, fragmentSource);
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}
	void OpenGLShader::Reload()
	{
		if (m_UseFilepath)
		{
			m_VSource = FileProcessing::ReadFromFile(m_VPath);
			m_FSource = FileProcessing::ReadFromFile(m_FPath);
		}
		else if (!m_VPath.empty())
			PreProcess();
		else return;

		Compile();
	}
	void OpenGLShader::Reload(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_VSource = vertexSource;
		m_FSource = fragmentSource;

		Compile();
	}
	void OpenGLShader::PreProcess()
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
				m_VSource = source.substr(nextLinePos, pos == std::string::npos ? std::string::npos : pos - nextLinePos);
			else
				m_FSource = source.substr(nextLinePos, pos == std::string::npos ? std::string::npos : pos - nextLinePos);
		}
	}
	void OpenGLShader::Compile()
	{
		Ref<OpenGLShader> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->m_ID = glCreateProgram();

			uint32_t vertexID = glCreateShader(GL_VERTEX_SHADER);
			const char* vSource = instance->m_VSource.c_str();
			glShaderSource(vertexID, 1, &vSource, nullptr);
			glCompileShader(vertexID);
			int isCompiled = 0;
			glGetShaderiv(vertexID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(vertexID, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(vertexID);
				SOF_CORE_THROW_INFO(std::string("Shader compilation failed (") + instance->m_VPath.string() + "):\n" + &infoLog[0]);
			}
			glAttachShader(instance->m_ID, vertexID);

			uint32_t fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fSource = instance->m_FSource.c_str();
			glShaderSource(fragmentID, 1, &fSource, nullptr);
			glCompileShader(fragmentID);
			isCompiled = 0;
			glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(fragmentID, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(fragmentID);
				SOF_CORE_THROW_INFO(std::string("Shader compilation failed (") + instance->m_VPath.string() + "):\n" + &infoLog[0]);
			}
			glAttachShader(instance->m_ID, fragmentID);

			glLinkProgram(instance->m_ID);
			glValidateProgram(instance->m_ID);

			glDetachShader(instance->m_ID, vertexID);
			glDetachShader(instance->m_ID, fragmentID);
			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);

			int isLinked = 0;
			glGetProgramiv(instance->m_ID, GL_LINK_STATUS, &isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(instance->m_ID, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(instance->m_ID, maxLength, &maxLength, &infoLog[0]);
				SOF_CORE_ERROR("Shader linking failed ({0}):\n{1}", instance->m_VPath, &infoLog[0]);

				glDeleteProgram(instance->m_ID);
			}

			if (instance->m_Loaded)
			{
				for (auto& callback : instance->m_ShaderReloadedCallback)
					callback();
			}
			instance->m_Loaded = true;
		});
	}

	void OpenGLShader::Bind() const noexcept
	{
		Ref<const OpenGLShader> instance = this;
		Renderer::Submit([instance]()
		{
			glUseProgram(instance->m_ID);
		});
	}
	void OpenGLShader::AddReloadedCallback(const ShaderReloadedCallback& callback) noexcept
	{
		m_ShaderReloadedCallback.push_back(callback);
	}
}
