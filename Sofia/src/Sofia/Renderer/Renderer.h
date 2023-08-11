#pragma once

#include "Sofia/Renderer/RenderCommandQueue.h"
#include "Sofia/Renderer/Shader.h"

namespace Sofia {

	class Renderer
	{
		friend class Application;
	public:
		template<typename T>
		static void Submit(T&& fn) noexcept
		{
			auto renderCommand = [](void* ptr)
			{
				auto fn = (T*)ptr;
				(*fn)();
				fn->~T();
			};
			auto buffer = GetRenderCommandQueue().Allocate(renderCommand, sizeof(fn));
			new (buffer) T(std::forward<T>(fn));
		}
		static void Render();

		static ShaderLibrary& GetShaderLibrary() noexcept;
	private:
		static void Init();
		static void Shutdown();
		static RenderCommandQueue& GetRenderCommandQueue() noexcept;
	};
}
