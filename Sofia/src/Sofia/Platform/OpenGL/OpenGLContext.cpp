#include <pch.h>
#include "OpenGLContext.h"

#if defined(SOF_PLATFORM_WINDOWS)
#	include "Sofia/Platform/Windows/WindowsWindow.h"

#	define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#	define WGL_CONTEXT_MINOR_VERSION_ARB 0X2092
#	define WGL_CONTEXT_FLAGS_ARB 0X2094
#	define WGL_CONTEXT_COREPROFILE_BIT_ARB 0x00000001
#	define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);
typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);
#else
#	include "Sofia/Platform/Linux/LinuxWindow.h"
#endif
#include <glad/glad.h>

#include "Sofia/Renderer/Renderer.h"

namespace Sofia {

	void OpenGLContext::Shutdown()
	{
#if defined(SOF_PLATFORM_LINUX)
		glfwTerminate();
#endif
	}
	void OpenGLContext::InitForWindow(void* window)
	{
#if defined(SOF_PLATFORM_WINDOWS)
		WindowsWindow* wnd = (WindowsWindow*)window;

		HDC hdc = wnd->m_DC;

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if (pixelFormat == 0)
			throw SOF_WINDOWS_WINDOW_LAST_EXCEPTION();
		SetPixelFormat(hdc, pixelFormat, &pfd);

		HGLRC tempRC = wglCreateContext(hdc);
		wglMakeCurrent(hdc, tempRC);
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		const int attribList[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 6,
			WGL_CONTEXT_FLAGS_ARB, 0,
			WGL_CONTEXT_PROFILE_MASK_ARB,
			WGL_CONTEXT_COREPROFILE_BIT_ARB, 0,
		};

		wnd->m_Context = wglCreateContextAttribsARB(hdc, 0, attribList);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempRC);
		wglMakeCurrent(hdc, wnd->m_Context);

		int success = gladLoadGL();
		SOF_CORE_ASSERT(success, "Failed to initialize GLAD");

		static bool initialized = false;
		if (!initialized)
		{
			SOF_CORE_INFO("OpenGL Info:");
			SOF_CORE_INFO("\tRenderer: {0}", (char*)glGetString(GL_RENDERER));
			SOF_CORE_INFO("\tVersion: {0}", (char*)glGetString(GL_VERSION));
			initialized = true;
		}

		PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
		if ((m_SwapControlEnabled = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control")) != 0)
		{
			m_WGLSwapInternalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
			m_WGLSwapInternalEXT(0);
		}
#else
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SOF_CORE_ASSERT(success, "Failed to initialize GLAD");

		static bool initialized = false;
		if (!initialized)
		{
			SOF_CORE_INFO("OpenGL Info:");
			SOF_CORE_INFO("\tRenderer: {0}", (char*)glGetString(GL_RENDERER));
			SOF_CORE_INFO("\tVersion: {0}", (char*)glGetString(GL_VERSION));
			initialized = true;
		}
#endif
	}
	void OpenGLContext::ShutdownForWindow(void* window)
	{
#if defined(SOF_PLATFORM_WINDOWS)
		WindowsWindow* wnd = (WindowsWindow*)window;
		HDC hdc = GetDC(wnd->m_Window);

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(wnd->m_Context);
		ReleaseDC(wnd->m_Window, hdc);
#endif
	}
	void OpenGLContext::SwapBuffers(void* window)
	{
#if defined(SOF_PLATFORM_WINDOWS)
		WindowsWindow* wnd = (WindowsWindow*)window;
		HDC hdc = wnd->m_DC;
		static bool vsync = false;

		if (vsync != wnd->m_Data.vSync)
		{
			vsync = wnd->m_Data.vSync;
			m_WGLSwapInternalEXT(vsync ? 1 : 0);
		}

		::SwapBuffers(hdc);

		if (vsync)
			glFinish();
#else
		glfwSwapBuffers(((LinuxWindow*)window)->m_Window);
#endif
	}
	void OpenGLContext::BindWindow(void* window)
	{
#if defined(SOF_PLATFORM_WINDOWS)
		WindowsWindow* wnd = (WindowsWindow*)window;
		HDC hdc = GetDC(wnd->m_Window);
		wglMakeCurrent(hdc, wnd->m_Context);
#else
		glfwMakeContextCurrent(((LinuxWindow*)window)->m_Window);
#endif
	}
	void OpenGLContext::BindToRender(void* window)
	{
		Renderer::Submit([window]()
		{
			LinuxWindow* wnd = (LinuxWindow*)window;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, wnd->m_Data.width, wnd->m_Data.height);
		});
	}
	void OpenGLContext::Clear(void* window, const glm::vec4& color)
	{
		Renderer::Submit([color]()
		{
			glClearColor(color.x, color.y, color.r, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		});
	}
}
