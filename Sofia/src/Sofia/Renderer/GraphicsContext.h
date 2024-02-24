#pragma once

#include "Sofia/Core.h"

namespace Sofia {

	class GraphicsContext : public RefCounted
	{
		friend class Application;
		friend class WindowsWindow;
		friend class LinuxWindow;
	public:
		SOF_CORE virtual ~GraphicsContext() = default;

	protected:
		SOF_CORE virtual void Init() = 0;
		SOF_CORE virtual void Shutdown() = 0;
		SOF_CORE virtual void InitForWindow(void* window) = 0;
		SOF_CORE virtual void ShutdownForWindow(void* window) = 0;

		SOF_CORE virtual void SwapBuffers(void* window) = 0;
		SOF_CORE virtual void RecreateSwapChain(void* window) = 0;
		SOF_CORE virtual void BindWindow(void* window) = 0;
		SOF_CORE virtual void BindToRender(void* window) = 0;
		SOF_CORE virtual void Clear(void* window, const glm::vec4& color) = 0;

		SOF_CORE static Ref<GraphicsContext> Create();
	};
}
