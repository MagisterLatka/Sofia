#include <pch.h>
#include "Application.h"

namespace Sofia {

	SOF_CORE Application* Application::s_Application = nullptr;
	SOF_CORE RendererAPI::API RendererAPI::s_API;

	Application::Application(const ApplicationSpecifications& specs)
		: m_Specs(specs)
	{
		SOF_CORE_ASSERT(!s_Application, "Application already exist!");
		s_Application = this;

		m_LayerStack = CreateScope<LayerStack>();
	}

	void Application::Init()
	{
		//create graphics context
		RendererAPI::s_API = m_Specs.GraphicsAPI;
		m_GraphicsContext = GraphicsContext::Create();
		m_GraphicsContext->Init();

		WindowProps windowProps;
		windowProps.Width = m_Specs.Width;
		windowProps.Height = m_Specs.Height;
		windowProps.Title = m_Specs.Name;
		m_Window = Window::Create(windowProps);
		m_Window->SetEventCallback(SOF_BIND_EVENT_FN(Application::OnEvent));

		//call OnAttach() on layers in LayerStack
		m_LayerStack->Init();
	}
	void Application::Shutdown()
	{
		m_LayerStack.reset();

		m_Window.Reset();
		m_GraphicsContext.Reset();
	}
	void Application::Close() noexcept
	{
		m_Running = false;
	}

	int Application::Run()
	{
		Init();
		m_Timer.Reset();
		int returnVal = 0; //return val from quit message
		while (m_Running)
		{
			//calc time since last frame
			m_Timestep = m_Timer.Elapsed();
			m_Timer.Reset();
			{ auto e = AppTickEvent(m_Timestep); OnEvent(e); }

			if (!m_Minimized)
			{
				//update layers
				for (auto layer : *m_LayerStack)
					layer->OnUpdate(m_Timestep);
			}
			{ auto e = AppUpdateEvent(); OnEvent(e); }

			//update frame
			m_Window->OnUpdate();
			{ auto e = AppRenderEvent(); OnEvent(e); }

			//process events
			if (auto a = Window::ProcessEvents())
			{
				returnVal = *a;
				m_Running = false;
			}
		}
		Shutdown();
		return returnVal;
	}

	void Application::OnEvent(Event& e)
	{
		//dispatch events for application::events
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SOF_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SOF_BIND_EVENT_FN(Application::OnWindowResize));

		//pass events to layers
		if (m_LayerStack)
		{
			for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
			{
				(*it)->OnEvent(e);
				if (e.p_Handled)
					break;
			}
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e) noexcept
	{
		m_Running = false;
		return false;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e) noexcept
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
