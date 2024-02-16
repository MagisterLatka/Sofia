#pragma once

#include "Sofia/Core.h"
#include "Sofia/LayerStack.h"

int main(int argc, char** argv, char** envp);

namespace Sofia {

	struct ApplicationSpecifications
	{
		std::string name = "Sofia Engine";
	};
	
	class Application
	{
		friend int ::main(int, char**, char**);
	public:
		SOF_CORE Application(const ApplicationSpecifications& applicationSpecifications = ApplicationSpecifications());
		SOF_CORE virtual ~Application() = default;

		SOF_CORE void Close() noexcept;

		SOF_CORE Layer* PushLayer(Layer* layer) { return m_LayerStack->PushLayer(layer); }
		SOF_CORE Layer* PushOverlay(Layer* overlay) { return m_LayerStack->PushOverlay(overlay); }

		SOF_CORE const ApplicationSpecifications& GetApplicationSpecifications() const noexcept { return m_Specs; }

		SOF_CORE static Application& Get() noexcept { return *s_Application; }
	private:
		SOF_CORE void Init();
		SOF_CORE void Shutdown();
		SOF_CORE int Run();

		SOF_CORE void OnEvent(Event& e);
	private:
		ApplicationSpecifications m_Specs;

		Scope<LayerStack> m_LayerStack;

		Timer m_Timer;
		Timestep m_Timestep;

		bool m_Running = true, m_Minimized = true;

		SOF_CORE static Application* s_Application;
	};

	Application* CreateApplication();
}
