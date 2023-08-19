#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/Event.h"
#include "Sofia/Keyboard.h"
#include "Sofia/Mouse.h"

namespace Sofia {

	struct WindowProps
	{
		std::string Title = "Sofia Engine";
		unsigned int Width = 1280;
		unsigned int Height = 720;
		bool Resizable = true;
		bool Maximized = false;
		bool HasTitleBar = true;

		WindowProps() = default;
		WindowProps(const std::string& title,
					unsigned int width,
					unsigned int height,
					bool resizable = true,
					bool maximized = false,
					bool hasTitleBar = true)
			: Title(title), Width(width), Height(height), Resizable(resizable), Maximized(maximized), HasTitleBar(hasTitleBar) {}
	};

	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		using TitleBarHitTestCallbackFn = std::function<void(const int xMousePos, const int yMousePos, int& hitTest)>;
		using InputDropCallbackFn = std::function<void(const int count, wchar_t** const paths)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;
		static std::optional<int> ProcessEvents();
		virtual void BindToRender() noexcept = 0;
		virtual void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) noexcept = 0;

		virtual void SetTitle(const std::string& title) = 0;
		virtual const std::string& GetTille() const noexcept = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) noexcept = 0;
		virtual void SetTitleBarHitTestCallback(const TitleBarHitTestCallbackFn& callback) noexcept = 0;
		virtual void SetInputDropCallback(const InputDropCallbackFn& callback) noexcept = 0;

		virtual void SetVSync(bool vsync) noexcept = 0;
		virtual bool IsVSyncEnabled() const noexcept = 0;

		virtual void Minimize() noexcept = 0;
		virtual bool IsMinimized() const noexcept = 0;
		virtual void Maximize() noexcept = 0;
		virtual void Restore() noexcept = 0;
		virtual bool IsMaximized() const noexcept = 0;

		virtual void SetIcon(const std::filesystem::path& iconPath) = 0;

		virtual unsigned int GetWidth() const noexcept = 0;
		virtual unsigned int GetHeight() const noexcept = 0;

		virtual Keyboard& GetKeyboard() noexcept = 0;
		virtual Mouse& GetMouse() noexcept = 0;

		static Ref<Window> Create(const WindowProps& props = WindowProps());
	};
}
