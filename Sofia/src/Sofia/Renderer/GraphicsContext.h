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
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void InitForWindow(void* window) = 0;
		virtual void ShutdownForWindow(void* window) = 0;

		virtual void SwapBuffers(void* window) = 0;
		virtual void RecreateSwapChain(void* window) = 0;
		virtual void BindWindow(void* window) = 0;
		virtual void BindToRender(void* window) = 0;
		virtual void Clear(void* window, const glm::vec4& color) = 0;

		static Ref<GraphicsContext> Create();
	};
}
