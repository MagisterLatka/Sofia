#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/Event.h"
#include "Sofia/Keyboard.h"
#include "Sofia/Mouse.h"

namespace Sofia {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Sofia Engine",
					unsigned int width = 1280,
					unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;
		static std::optional<int> ProcessEvents();
		virtual void BindToRender() noexcept = 0;
		virtual void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) noexcept = 0;

		virtual void SetTitle(const std::string& title) = 0;
		virtual const std::string& GetTille() const noexcept = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) noexcept = 0;
		virtual void SetVSync(bool vsync) noexcept = 0;
		virtual bool IsVSyncEnabled() const noexcept = 0;

		virtual unsigned int GetWidth() const noexcept = 0;
		virtual unsigned int GetHeight() const noexcept = 0;

		virtual Keyboard& GetKeyboard() noexcept = 0;
		virtual Mouse& GetMouse() noexcept = 0;

		static Ref<Window> Create(const WindowProps& props = WindowProps());
	};
}
