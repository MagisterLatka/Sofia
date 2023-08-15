#pragma once

namespace Sofia {

	class SofiaException : public std::exception
	{
	public:
		SofiaException(int line, const char* file, bool core = false) noexcept : m_Line(line), m_File(file), m_Core(core) {}
		SofiaException(const SofiaException& other)
			: m_WhatBuffer(other.m_WhatBuffer), m_Line(other.m_Line), m_File(other.m_File), m_Core(other.m_Core) {}
		virtual const char* what() const noexcept override
		{
			std::ostringstream oss;
			oss << GetType() << std::endl;
			oss << GetOriginString();
			m_WhatBuffer = oss.str();
			return m_WhatBuffer.c_str();
		}
		virtual const char* GetType() const noexcept { return m_Core ? "Core exception" : "Exception"; }
		int GetLine() const noexcept { return m_Line; }
		const std::string& GetFile() const noexcept { return m_File; }
		std::string GetOriginString() const noexcept
		{
			std::ostringstream oss;
			oss << "[File] " << m_File << std::endl;
			oss << "[Line] " << m_Line;
			return oss.str();
		}
	protected:
		mutable std::string m_WhatBuffer;
	private:
		int m_Line;
		std::string m_File;
		bool m_Core;
	};

	class MessageException : public SofiaException
	{
	public:
		MessageException(int line, const char* file, const std::string& message, bool core = false) noexcept
			: SofiaException(line, file, core), m_Message(message) {}
		virtual const char* what() const noexcept override
		{
			std::ostringstream oss;
			oss << GetType() << std::endl;
			oss << "[Message] " << m_Message << std::endl;
			oss << GetOriginString();
			m_WhatBuffer = oss.str();
			return m_WhatBuffer.c_str();
		}
	private:
		std::string m_Message;
	};
}
