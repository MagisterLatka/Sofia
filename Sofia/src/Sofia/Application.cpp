#include <pch.h>
#include "Sofia/Application.h"

namespace Sofia {

	Application* Application::s_Application = nullptr;

	Application::Application(const std::string& title, ApplicationCommandLineArgs args)
		: m_CommandLineArgs(args)
	{
		SOF_CORE_ASSERT(!s_Application, "Application already exist!");
		s_Application = this;

		m_LayerStack = CreateScope<LayerStack>();
	}
	Application::~Application()
	{

	}

	void Application::Init() noexcept
	{}
	void Application::Shutdown() noexcept
	{
		m_LayerStack.reset();
	}
	void Application::Close() noexcept
	{
		m_Running = false;
	}

	int Application::Run()
	{
		Init();

		m_Timer.Reset();
		int returnVal = 0;
		while (m_Running)
		{ 
			m_Timestep = m_Timer.Elapsed();
			m_Timer.Reset();
			{ auto e = AppTickEvent(m_Timestep); OnEvent(e); }

			if (!m_Minimized)
			{
				for (auto layer : *m_LayerStack)
					layer->OnUpdate(m_Timestep);
			}
			{ auto e = AppUpdateEvent(); OnEvent(e); }

			{ auto e = AppRenderEvent(); OnEvent(e); }
		}

		Shutdown();
		return returnVal;
	}

	void Application::OnEvent(Event& e)
	{
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SOF_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SOF_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.p_Handled)
				break;
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		int width = e.GetXSize(), height = e.GetYSize();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		return false;
	}
}
