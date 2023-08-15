#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/ApplicationEvents.h"
#include "Sofia/LayerStack.h"
#include "Sofia/Window.h"
#include "Sofia/Renderer/GraphicsContext.h"
#include "Sofia/ImGui/ImGuiLayer.h"
#include "Sofia/Renderer/RendererAPI.h"

int main(int argc, char** argv, char** envp);

namespace Sofia {

	struct ApplicationSpecifications
	{
		std::string Name = "Sofia Engine";
		uint32_t Width = 1600u, Height = 900;
		std::filesystem::path IconPath;
		bool ResizableWindow = true;

		RendererAPI::API GraphicsAPI = RendererAPI::API::DX11;
	};

	class Application
	{
		friend int ::main(int argc, char** argv, char** envp);
	public:
		Application(ApplicationSpecifications applicationSpecifications = ApplicationSpecifications());
		virtual ~Application();

		void Init() noexcept;
		void Shutdown() noexcept;
		void Close() noexcept;

		void PushLayer(Layer* layer) noexcept { m_LayerStack->PushLayer(layer); }
		void PushOverlay(Layer* overlay) noexcept { m_LayerStack->PushOverlay(overlay); }

		Ref<GraphicsContext> GetGraphicsContext() const noexcept { return m_GraphicsContext; }
		Ref<Window> GetWindow() const noexcept { return m_Window; }
		ImGuiLayer* GetImGuiLayer() noexcept { return m_ImGuiLayer; }

		const ApplicationSpecifications& GetApplicationSpecifications() const noexcept { return m_Specs; }

		static Application& Get() noexcept { return *s_Application; }
	private:
		int Run();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationSpecifications m_Specs;

		bool m_Running = true;
		bool m_Minimized = false;

		Scope<LayerStack> m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Timer m_Timer;
		Timestep m_Timestep;

		Ref<GraphicsContext> m_GraphicsContext;
		Ref<Window> m_Window;

		static Application* s_Application;
	};

	Application* CreateApplication();
}
