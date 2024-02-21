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

		SOF_CORE WindowProps(const std::string& title = "Sofia Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		SOF_CORE virtual ~Window() = default;

		SOF_CORE virtual void OnUpdate() = 0;
		SOF_CORE static std::optional<int> ProcessEvents();
		SOF_CORE virtual void BindWindow() noexcept = 0;
		SOF_CORE virtual void BindToRender() noexcept = 0;
		SOF_CORE virtual void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) noexcept = 0;

		SOF_CORE virtual void SetTitle(const std::string& title) = 0;
		SOF_CORE virtual const std::string& GetTitle() const noexcept = 0;

		SOF_CORE virtual void SetEventCallback(const EventCallbackFn& callback) noexcept = 0;
		SOF_CORE virtual void SetVSync(bool vsync) noexcept = 0;
		SOF_CORE virtual bool IsVSyncEnabled() const noexcept = 0;

		SOF_CORE virtual unsigned int GetWidth() const noexcept = 0;
		SOF_CORE virtual unsigned int GetHeight() const noexcept = 0;

		SOF_CORE virtual Keyboard& GetKeyboard() noexcept = 0;
		SOF_CORE virtual Mouse& GetMouse() noexcept = 0;

		SOF_CORE static Ref<Window> Create(const WindowProps& props = WindowProps());
	};
}
