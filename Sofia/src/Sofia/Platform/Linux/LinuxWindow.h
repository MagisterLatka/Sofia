#pragma once

#include "Sofia/Core.h"
#include "Sofia/Window.h"
#include <GLFW/glfw3.h>

namespace Sofia {

	class LinuxWindow : public Window
	{
		friend class OpenGLContext;
		friend class ImGuiLayer;
	public:
		SOF_CORE LinuxWindow(const WindowProps& props);
		LinuxWindow(const LinuxWindow&) = delete;
		SOF_CORE ~LinuxWindow();

		LinuxWindow& operator=(const LinuxWindow&) = delete;

		SOF_CORE virtual void OnUpdate() override;
		SOF_CORE static std::optional<int> ProcessEvents();
		SOF_CORE virtual void BindWindow() noexcept override;
		SOF_CORE virtual void BindToRender() noexcept override {}
		SOF_CORE virtual void Clear(const glm::vec4& color) noexcept override;

		SOF_CORE virtual void SetTitle(const std::string& title) override;
		SOF_CORE virtual const std::string& GetTitle() const noexcept override { return m_Data.title; }

		SOF_CORE virtual void SetEventCallback(const EventCallbackFn& callback) noexcept override { m_Data.eventCallback = callback; }
		SOF_CORE virtual void SetTitleBarHitTestCallback(const TitleBarHitTestCallbackFn& callback) noexcept { m_Data.titlebarHitTest = callback; }
		SOF_CORE virtual void SetInputDropCallback(const InputDropCallbackFn& callback) noexcept override { m_Data.inputDropCallback = callback; }
		SOF_CORE virtual void SetVSync(bool vsync) noexcept override;
		SOF_CORE virtual bool IsVSyncEnabled() const noexcept override { return m_Data.vSync; }

		SOF_CORE virtual void Minimize() noexcept override;
		SOF_CORE virtual bool IsMinimized() const noexcept override { return m_Data.minimized; }
		SOF_CORE virtual void Maximize() noexcept override;
		SOF_CORE virtual void Restore() noexcept override;
		SOF_CORE virtual bool IsMaximized() const noexcept override { return m_Data.maximized; }

		SOF_CORE virtual void SetIcon(const std::filesystem::path& iconPath) override;

		SOF_CORE virtual uint32_t GetWidth() const noexcept override { return m_Data.width; }
		SOF_CORE virtual uint32_t GetHeight() const noexcept override { return m_Data.height; }

		SOF_CORE virtual int GetXClientPos() const noexcept override { return m_Data.pos.x; }
		SOF_CORE virtual int GetYClientPos() const noexcept override { return m_Data.pos.y; }

		SOF_CORE virtual Keyboard& GetKeyboard() noexcept { return m_Data.keyboard; };
		SOF_CORE virtual Mouse& GetMouse() noexcept { return m_Data.mouse; }
	private:
		SOF_CORE void Init(const WindowProps& props);
		SOF_CORE void Shutdown();

		SOF_CORE void DefaultEventCallback(Event& e) {}
	private:
		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			glm::ivec2 pos;
			bool vSync = false, maximized = false, minimized = false, titlebar = true;

			EventCallbackFn eventCallback;
			TitleBarHitTestCallbackFn titlebarHitTest;
			InputDropCallbackFn inputDropCallback;

			Keyboard keyboard;
			Mouse mouse;
		};
		WindowData m_Data;

		GLFWwindow* m_Window = nullptr;
	};
}
