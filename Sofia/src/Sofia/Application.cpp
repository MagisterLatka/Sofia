#include <pch.h>
#include "Sofia/Application.h"

#include "Sofia/Renderer/Renderer.h"

namespace Sofia {

	Application* Application::s_Application = nullptr;
	RendererAPI::API RendererAPI::s_API;

	Application::Application(ApplicationSpecifications applicationSpecifications)
		: m_Specs(applicationSpecifications)
	{
		SOF_CORE_ASSERT(!s_Application, "Application already exist!");
		s_Application = this;

		RendererAPI::s_API = m_Specs.GraphicsAPI;
		m_GraphicsContext = GraphicsContext::Create();
		m_GraphicsContext->Init();

		WindowProps windowProps;
		windowProps.Width = m_Specs.Width;
		windowProps.Height = m_Specs.Height;
		windowProps.Title = m_Specs.Name;
		windowProps.Resizable = m_Specs.ResizableWindow;
		m_Window = Window::Create(windowProps);
		m_Window->SetEventCallback(SOF_BIND_EVENT_FN(Application::OnEvent));
		if (!m_Specs.IconPath.empty()) m_Window->SetIcon(m_Specs.IconPath);

		Renderer::Init();

		m_LayerStack = CreateScope<LayerStack>();
		m_ImGuiLayer = new ImGuiLayer;
		m_LayerStack->PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{

	}

	void Application::Init() noexcept
	{}
	void Application::Shutdown() noexcept
	{
		m_Window.Reset();
		m_LayerStack.reset();
		Renderer::Shutdown();
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

			m_ImGuiLayer->Begin();
			for (auto layer : *m_LayerStack)
				layer->OnUIRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
			{ auto e = AppRenderEvent(); OnEvent(e); }
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
		Dispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SOF_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SOF_BIND_EVENT_FN(Application::OnWindowResize));

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
