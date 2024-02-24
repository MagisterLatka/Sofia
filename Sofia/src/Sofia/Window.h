#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/Event.h"
#include "Sofia/Keyboard.h"
#include "Sofia/Mouse.h"

namespace Sofia {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width, Height;
		bool Resizable = true, Maximized = false, HasTitleBar = true;

		SOF_CORE WindowProps() = default;
		SOF_CORE WindowProps(const std::string& title,
			unsigned int width,
			unsigned int height,
			bool resizable = true,
			bool maximized = false,
			bool hasTitleBar = true)
			: Title(title), Width(width), Height(height), Resizable(resizable), Maximized(maximized), HasTitleBar(HasTitleBar)
		{}
	};

	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		using TitleBarHitTestCallbackFn = std::function<void(const int xMousePos, const int yMousePos, int& hitTest)>;
		using InputDropCallbackFn = std::function<void(const int count, wchar_t** const paths)>;

		SOF_CORE virtual ~Window() = default;

		SOF_CORE virtual void OnUpdate() = 0;
		SOF_CORE static std::optional<int> ProcessEvents();
		SOF_CORE virtual void BindWindow() noexcept = 0;
		SOF_CORE virtual void BindToRender() noexcept = 0;
		SOF_CORE virtual void Clear(const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f }) noexcept = 0;

		SOF_CORE virtual void SetTitle(const std::string& title) = 0;
		SOF_CORE virtual const std::string& GetTitle() const noexcept = 0;

		SOF_CORE virtual void SetEventCallback(const EventCallbackFn& callback) noexcept = 0;
		SOF_CORE virtual void SetTitleBarHitTestCallback(const TitleBarHitTestCallbackFn& callback) noexcept = 0;
		SOF_CORE virtual void SetInputDropCallback(const InputDropCallbackFn& callback) noexcept = 0;
		SOF_CORE virtual void SetVSync(bool vsync) noexcept = 0;
		SOF_CORE virtual bool IsVSyncEnabled() const noexcept = 0;

		SOF_CORE virtual void Minimize() noexcept = 0;
		SOF_CORE virtual bool IsMinimized() const noexcept = 0;
		SOF_CORE virtual void Maximize() noexcept = 0;
		SOF_CORE virtual void Restore() noexcept = 0;
		SOF_CORE virtual bool IsMaximized() const noexcept = 0;

		SOF_CORE virtual void SetIcon(const std::filesystem::path& iconPath) = 0;

		SOF_CORE virtual uint32_t GetWidth() const noexcept = 0;
		SOF_CORE virtual uint32_t GetHeight() const noexcept = 0;

		SOF_CORE virtual int GetXClientPos() const noexcept = 0;
		SOF_CORE virtual int GetYClientPos() const noexcept = 0;

		SOF_CORE virtual Keyboard& GetKeyboard() noexcept = 0;
		SOF_CORE virtual Mouse& GetMouse() noexcept = 0;

		SOF_CORE static Ref<Window> Create(const WindowProps& props = WindowProps());
	};
}
