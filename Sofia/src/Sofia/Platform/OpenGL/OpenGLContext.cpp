#include <pch.h>
#include "OpenGLContext.h"

#include <glad/glad.h>

namespace Sofia {

	OpenGLContext::OpenGLContext(void* window)
		: m_Window((GLFWwindow*)window) {}
	
	void OpenGLContext::InitWindow()
	{
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SOF_CORE_ASSERT(success, "Failed to initialize GLAD");

		SOF_CORE_INFO("OpenGL Info:");
		SOF_CORE_INFO("\tRenderer: {0}", (char*)glGetString(GL_RENDERER));
		SOF_CORE_INFO("\tVersion: {0}", (char*)glGetString(GL_VERSION));
	}
}
