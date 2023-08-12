#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/ApplicationEvents.h"
#include "Sofia/LayerStack.h"
#include "Sofia/Window.h"
#include "Sofia/Renderer/GraphicsContext.h"
#include "Sofia/ImGui/ImGuiLayer.h"

int main(int argc, char** argv, char** envp);

namespace Sofia {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;
		char** Env = nullptr;

		const char* operator[](int index) const
		{
			SOF_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class Application
	{
		friend int ::main(int argc, char** argv, char** envp);
	public:
		Application(const std::string& title = "Sofia Engine", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Init() noexcept;
		void Shutdown() noexcept;
		void Close() noexcept;

		void PushLayer(Layer* layer) noexcept { m_LayerStack->PushLayer(layer); }
		void PushOverlay(Layer* overlay) noexcept { m_LayerStack->PushOverlay(overlay); }

		Ref<GraphicsContext> GetGraphicsContext() const noexcept { return m_GraphicsContext; }
		Ref<Window> GetWindow() const noexcept { return m_Window; }
		ImGuiLayer* GetImGuiLayer() noexcept { return m_ImGuiLayer; }

		ApplicationCommandLineArgs GetCommandLineArgs() const noexcept { return m_CommandLineArgs; }

		static Application& Get() noexcept { return *s_Application; }
	private:
		int Run();
		void ChooseGraphicsContext();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;

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

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
