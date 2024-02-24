#pragma once

#include "Sofia/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Sofia {

	class OpenGLContext : public GraphicsContext
	{
	public:
		SOF_CORE OpenGLContext() = default;
		SOF_CORE ~OpenGLContext() = default;

		SOF_CORE virtual void Init() override {}
		SOF_CORE virtual void Shutdown() override;
		SOF_CORE virtual void InitForWindow(void* window) override;
		SOF_CORE virtual void ShutdownForWindow(void* window) override;

		SOF_CORE virtual void SwapBuffers(void* window) override;
		SOF_CORE virtual void RecreateSwapChain(void* window) override {}
		SOF_CORE virtual void BindWindow(void* window) override;
		SOF_CORE virtual void BindToRender(void* window) override;
		SOF_CORE virtual void Clear(void* window, const glm::vec4& color) override;
	private:
#if defined(SOF_PLATFORM_WINDOWS)
		int (__stdcall *m_WGLSwapInternalEXT)(int) = nullptr;
		bool m_SwapControlEnabled = false;
#endif
	};
}
