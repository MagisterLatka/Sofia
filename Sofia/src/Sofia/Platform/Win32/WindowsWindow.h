#pragma once

#include "Sofia/Core.h"
#include "Sofia/Window.h"
#include "Sofia/Renderer/GraphicsContext.h"
#include <d3d11.h>

namespace Sofia {

	class WindowsWindow : public Window
	{
	public:
		class WindowException : public SofiaException
		{
		public:
			WindowException(int line, const char* file, HRESULT hresult) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override { return "Window Exception"; }
			HRESULT GetErrorCode() const noexcept { return m_Result; }
			std::string GetErrorDescription() const noexcept;
			static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		private:
			HRESULT m_Result;
		};

		class WindowClass
		{
		public:
			static const char* GetName() noexcept { return "Sofia Game Engine Window Class"; }
			static HINSTANCE GetInstance() noexcept { return s_WindowClass.m_Instance; }
		private:
			WindowClass() noexcept;
			WindowClass(const WindowClass&) = delete;
			~WindowClass();

			WindowClass& operator=(const WindowClass&) = delete;
		private:
			static WindowClass s_WindowClass;
			HINSTANCE m_Instance;
		};
	public:
		WindowsWindow(const WindowProps& props) noexcept;
		WindowsWindow(const WindowsWindow&) = delete;
		~WindowsWindow();

		WindowsWindow& operator=(const WindowsWindow&) = delete;

		virtual void OnUpdate() override;
		static std::optional<int> ProcessEvents();
		virtual void BindToRender() noexcept override;
		virtual void Clear(const glm::vec4& color) noexcept override;

		virtual void SetTitle(const std::string& title) override;
		virtual const std::string& GetTille() const noexcept override { return m_Data.title; }

		virtual void SetEventCallback(const EventCallbackFn& callback) noexcept override { m_Data.eventCallback = callback; }
		virtual void SetVSync(bool vsync) noexcept override { m_Data.vSync = vsync; }
		virtual bool IsVSyncEnabled() const noexcept override { return m_Data.vSync; }

		virtual void SetIcon(const std::filesystem::path& iconPath) override;

		virtual uint32_t GetWidth() const noexcept override { return m_Data.width; }
		virtual uint32_t GetHeight() const noexcept override { return m_Data.height; }

		virtual Keyboard& GetKeyboard() noexcept { return m_Keyboard; };
		virtual Mouse& GetMouse() noexcept { return m_Mouse; }

		HWND GetNativeWindow() const noexcept { return m_Window; }

		void HandleInputForImgui(bool handle = true) noexcept { m_HandleInputForImgui = handle; }
	private:
		void Init(const WindowProps& props);
		void Shutdown();

		static HICON CreateIcon(const uint8_t* pixels, int x, int y);

		static LRESULT CALLBACK HandleMsgSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		void DefaultEventCallback(Event& e) {}
	private:
		HWND m_Window = nullptr;
		ComPtr<IDXGISwapChain> m_SwapChain;
		ComPtr<ID3D11RenderTargetView> m_TargetView;

		Keyboard m_Keyboard;
		Mouse m_Mouse;

		bool m_HandleInputForImgui = false;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			glm::ivec2 pos;
			bool vSync = true;

			EventCallbackFn eventCallback;
		};
		WindowData m_Data;

		HICON m_SmallIcon = nullptr, m_BigIcon = nullptr;
	};

}

#define SOF_WINDOWS_WINDOW_EXCEPTION(hr) Sofia::WindowsWindow::WindowException(__LINE__, __FILE__, hr)
#define SOF_WINDOWS_WINDOW_LAST_EXCEPTION() Sofia::WindowsWindow::WindowException(__LINE__, __FILE__, GetLastError())
