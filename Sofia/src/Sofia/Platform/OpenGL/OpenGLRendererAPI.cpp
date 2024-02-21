#include <pch.h>
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include <glfw3.h>

namespace Sofia {

	static void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		const char* Source;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:				Source = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		Source = "WINDOW SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	Source = "SHADER COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		Source = "THIRD PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		Source = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER:
		default:								Source = "UNKNOWN"; break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			SOF_CORE_CRITICAL("({0}): {1}", Source, message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:			SOF_CORE_ERROR("({0}): {1}", Source, message); break;
		case GL_DEBUG_SEVERITY_LOW:				SOF_CORE_WARN("({0}): {1}", Source, message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	SOF_CORE_TRACE("({0}): {1}", Source, message); break;
		}
	}
	void OpenGLRendererAPI::Init()
	{
#ifdef SOF_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
	}
	void OpenGLRendererAPI::Shutdown()
	{
		glfwTerminate();
	}

	void OpenGLRendererAPI::Draw(Topology topology, uint32_t verticesCount)
	{
		glDrawArrays(GetTopology(topology), 0, verticesCount);
	}
	void OpenGLRendererAPI::DrawIndexed(Topology topology, uint32_t indicesCount)
	{
		glDrawElements(GetTopology(topology), indicesCount, GL_UNSIGNED_INT, nullptr);
	}
	int OpenGLRendererAPI::GetTopology(Topology topology)
	{
		switch (topology)
		{
		case Sofia::RendererAPI::Topology::None: SOF_CORE_ASSERT("None topology cannot be used"); return -1;
		case Sofia::RendererAPI::Topology::Points: return GL_POINTS;
		case Sofia::RendererAPI::Topology::Lines: return GL_LINES;
		case Sofia::RendererAPI::Topology::LineStrip: return GL_LINE_STRIP;
		case Sofia::RendererAPI::Topology::Triangles: return GL_TRIANGLES;
		case Sofia::RendererAPI::Topology::TriangleStrip: return GL_TRIANGLE_STRIP;
		}
		SOF_CORE_ASSERT("Unknown topology");
		return -1;
	}
}
