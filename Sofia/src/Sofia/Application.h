#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/ApplicationEvents.h"
#include "Sofia/LayerStack.h"
#include "Sofia/Window.h"
#include "Sofia/Renderer/GraphicsContext.h"
#include "Sofia/ImGui/ImGuiLayer.h"
#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Renderer/Texture.h"

int main(int argc, char** argv, char** envp);

namespace Sofia {

	struct ApplicationSpecifications
	{
		std::string Name = "Sofia Engine";
		uint32_t Width = 1600u, Height = 900;
		std::filesystem::path WindowIconPath;
		Buffer TitlebarIconData;
		bool ResizableWindow = true;
		bool CustomTitleBar = false;

		RendererAPI::API GraphicsAPI = RendererAPI::API::DX11;
	};

	class Application
	{
		friend int ::main(int argc, char** argv, char** envp);
	public:
		using MenuBarCallbackFunc = std::function<void()>;

		Application(ApplicationSpecifications applicationSpecifications = ApplicationSpecifications());
		virtual ~Application();

		void Init() noexcept;
		void Shutdown() noexcept;
		void Close() noexcept;

		Layer* PushLayer(Layer* layer) noexcept { return m_LayerStack->PushLayer(layer); }
		Layer* PushOverlay(Layer* overlay) noexcept { return m_LayerStack->PushOverlay(overlay); }

		Ref<GraphicsContext> GetGraphicsContext() const noexcept { return m_GraphicsContext; }
		Ref<Window> GetWindow() const noexcept { return m_Window; }
		ImGuiLayer* GetImGuiLayer() noexcept { return m_ImGuiLayer; }
		void SetMenuBarCallbackFunc(const MenuBarCallbackFunc& callback) noexcept { m_MenuBarCallback = callback; }

		const ApplicationSpecifications& GetApplicationSpecifications() const noexcept { return m_Specs; }

		static Application& Get() noexcept { return *s_Application; }
	private:
		int Run();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void ImGuiRender();
		void DrawTitleBar(float& titlebarHeight);
		void DrawMenuBarUI();
	private:
		ApplicationSpecifications m_Specs;

		bool m_Running = true;
		bool m_Minimized = false;

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

		Ref<GraphicsContext> m_GraphicsContext;
		Ref<Window> m_Window;

		static Application* s_Application;
	};

	Application* CreateApplication();
}
