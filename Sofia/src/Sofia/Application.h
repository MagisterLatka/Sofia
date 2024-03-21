#pragma once

#include "Sofia/Core.h"
#include "Sofia/LayerStack.h"
#include "Sofia/Window.h"
#include "Sofia/Renderer/GraphicsContext.h"
#include "Sofia/Events/ApplicationEvents.h"
#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/ImGui/ImGuiLayer.h"
#include "Sofia/Renderer/Texture.h"

int main(int argc, char** argv, char** envp);

namespace Sofia {

	struct ApplicationSpecifications
	{
		std::string Name = "Sofia Engine";
		uint32_t Width = 1600u, Height = 900u;
		std::filesystem::path WindowIconPath;
		Buffer TitlebarIconData;
		bool ResizableWindow = true;
		bool CustomTitleBar = false;

#if defined(SOF_PLATFORM_WINDOWS)
		RendererAPI::API GraphicsAPI = RendererAPI::API::DX11;
#else
		RendererAPI::API GraphicsAPI = RendererAPI::API::OpenGL;
#endif
	};
	
	class Application
	{
		friend int ::main(int, char**, char**);
	public:
		using MenuBarCallbackFunc = std::function<void()>;

		SOF_CORE Application(const ApplicationSpecifications& applicationSpecifications = ApplicationSpecifications());
		SOF_CORE virtual ~Application() = default;

		SOF_CORE void Close() noexcept;

		SOF_CORE Layer* PushLayer(Layer* layer) { return m_LayerStack->PushLayer(layer); }
		SOF_CORE Layer* PushOverlay(Layer* overlay) { return m_LayerStack->PushOverlay(overlay); }

		SOF_CORE Ref<GraphicsContext> GetGraphicsContext() const noexcept { return m_GraphicsContext; }
		SOF_CORE Ref<Window> GetWindow() const noexcept { return m_Window; }
		SOF_CORE ImGuiLayer* GetImGuiLayer() noexcept { return m_ImGuiLayer; }
		void SetMenuBarCallbackFunc(const MenuBarCallbackFunc& callback) noexcept { m_MenuBarCallback = callback; }

		SOF_CORE const ApplicationSpecifications& GetApplicationSpecifications() const noexcept { return m_Specs; }

		SOF_CORE static Application& Get() noexcept { return *s_Application; }
	private:
		void Init();
		void Shutdown();
		SOF_CORE int Run();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e) noexcept;
		bool OnWindowResize(WindowResizeEvent& e) noexcept;

		void ImGuiRender();
		void DrawTitleBar(float& titlebarHeight);
		void DrawMenuBarUI();
	private:
		ApplicationSpecifications m_Specs;

		Ref<GraphicsContext> m_GraphicsContext;
		Ref<Window> m_Window;

		Scope<LayerStack> m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		MenuBarCallbackFunc m_MenuBarCallback;
		bool m_TitleBarHovered = false;
		Ref<Texture2D> m_TitlebarIcon;
		Ref<Texture2D> m_CloseIcon;
		Ref<Texture2D> m_MinimizeIcon;
		Ref<Texture2D> m_MaximizeIcon;
		Ref<Texture2D> m_RestoreIcon;

		Timer m_Timer;
		Timestep m_Timestep;

		bool m_Running = true, m_Minimized = false;

		SOF_CORE static Application* s_Application;
	};

	Application* CreateApplication();
}
