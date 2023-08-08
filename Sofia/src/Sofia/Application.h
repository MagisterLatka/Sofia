#pragma once

#include "Sofia/Core.h"
#include "Sofia/Events/ApplicationEvents.h"
#include "Sofia/LayerStack.h"

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

		ApplicationCommandLineArgs GetCommandLineArgs() const noexcept { return m_CommandLineArgs; }

		static Application& Get() noexcept { return *s_Application; }
	private:
		int Run();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;

		bool m_Running = true;
		bool m_Minimized = false;

		Scope<LayerStack> m_LayerStack;

		Timer m_Timer;
		Timestep m_Timestep;

		static Application* s_Application;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
