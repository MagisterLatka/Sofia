#pragma once

#include "Sofia/Core.h"
#include "Sofia/Window.h"
#include "Sofia/Renderer/GraphicsContext.h"
#include <d3d11.h>

namespace Sofia {

	class WindowsWindow : public Window
	{
		friend class DX11Context;
		friend class OpenGLContext;
		friend class ImGuiLayer;
	public:
		class WindowException : public SofiaException
		{
		public:
			SOF_CORE WindowException(int line, const char* file, HRESULT hresult) noexcept;
			SOF_CORE const char* what() const noexcept override;
			SOF_CORE const char* GetType() const noexcept override { return "Window Exception"; }
			SOF_CORE HRESULT GetErrorCode() const noexcept { return m_Result; }
			SOF_CORE std::string GetErrorDescription() const noexcept;
			SOF_CORE static std::string TranslateErrorCode(HRESULT hr) noexcept;
		private:
			HRESULT m_Result;
		};

		class WindowClass
		{
		public:
			SOF_CORE static const char* GetName() noexcept { return "Sofia Game Engine Window Class"; }
			SOF_CORE static HINSTANCE GetInstance() noexcept { return s_WindowClass.m_Instance; }
		private:
			WindowClass() noexcept;
			WindowClass(const WindowClass&) = delete;
			~WindowClass();

			WindowClass& operator=(const WindowClass&) = delete;
		private:
			SOF_CORE static WindowClass s_WindowClass;
			HINSTANCE m_Instance;
		};
	public:
		SOF_CORE WindowsWindow(const WindowProps& props);
		WindowsWindow(const WindowsWindow&) = delete;
		SOF_CORE virtual ~WindowsWindow();

		WindowsWindow& operator=(const WindowsWindow&) = delete;

		SOF_CORE virtual void OnUpdate() override;
		SOF_CORE static std::optional<int> ProcessEvents();
		SOF_CORE virtual void BindWindow() noexcept override {}
		SOF_CORE virtual void BindToRender() noexcept override;
		SOF_CORE virtual void Clear(const glm::vec4& color) noexcept override;

		SOF_CORE virtual void SetTitle(const std::string& title) override;
		SOF_CORE virtual const std::string& GetTitle() const noexcept override { return m_Data.title; }

		SOF_CORE virtual void SetEventCallback(const EventCallbackFn& callback) noexcept override { m_Data.eventCallback = callback; }
		SOF_CORE virtual void SetTitleBarHitTestCallback(const TitleBarHitTestCallbackFn& callback) noexcept override { m_Data.titlebarHitTest = callback; }
		SOF_CORE virtual void SetInputDropCallback(const InputDropCallbackFn& callback) noexcept override { m_Data.inputDropCallback = callback; };
		SOF_CORE virtual void SetVSync(bool vsync) noexcept override { m_Data.vSync = vsync; }
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

		SOF_CORE virtual Keyboard& GetKeyboard() noexcept { return m_Keyboard; };
		SOF_CORE virtual Mouse& GetMouse() noexcept { return m_Mouse; }
	private:
		void Init(const WindowProps& props);
		void Shutdown();

		static HICON CreateIcon(const uint8_t* pixels, int x, int y);

		static LRESULT CALLBACK HandleMsgSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		void HandleImGuiInput(bool handle = true) noexcept { m_HandleImGuiInput = handle; }

		void DefaultEventCallback(Event& e) {}
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
		};
		WindowData m_Data;

		Keyboard m_Keyboard;
		Mouse m_Mouse;
		
		HWND m_Window = nullptr;
		HDC m_DC = nullptr;
		HGLRC m_Context = nullptr;
		HICON m_SmallIcon = nullptr, m_BigIcon = nullptr;

		bool m_HandleImGuiInput = false;
	};

}

#define SOF_WINDOWS_WINDOW_EXCEPTION(hr) Sofia::WindowsWindow::WindowException(__LINE__, __FILE__, hr)
#define SOF_WINDOWS_WINDOW_LAST_EXCEPTION() Sofia::WindowsWindow::WindowException(__LINE__, __FILE__, GetLastError())
