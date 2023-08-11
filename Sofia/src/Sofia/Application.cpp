#include <pch.h>
#include "Sofia/Application.h"

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Renderer/Renderer.h"

namespace Sofia {

	Application* Application::s_Application = nullptr;

	Application::Application(const std::string& title, ApplicationCommandLineArgs args)
		: m_CommandLineArgs(args)
	{
		SOF_CORE_ASSERT(!s_Application, "Application already exist!");
		s_Application = this;

		ChooseGraphicsContext();
		m_Window = Window::Create(WindowProps{ title, 1600u, 900u });
		m_Window->SetEventCallback(SOF_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_LayerStack = CreateScope<LayerStack>();
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

	RendererAPI::API RendererAPI::s_API;
	void Application::ChooseGraphicsContext()
	{
		bool foundAPI = false;
		for (int i = 0; i < m_CommandLineArgs.Count; ++i)
		{
			std::string arg = m_CommandLineArgs[i];
			if (auto a = arg.find("API="); a != std::string::npos)
			{
				std::string api = arg.substr(a);
				if (api.find("DirectX11") || api.find("DX11"))
				{
					RendererAPI::s_API = RendererAPI::API::DX11;
					foundAPI = true;
					break;
				}
				else if (api.find("DirectX12") || api.find("DX12"))
				{
					SOF_CORE_THROW_INFO("DX12 is not supported yet");
					RendererAPI::s_API = RendererAPI::API::DX12;
					foundAPI = true;
					break;
				}
				else if (api.find("OpenGL"))
				{
					SOF_CORE_THROW_INFO("Opengl is not supported yet");
					RendererAPI::s_API = RendererAPI::API::OpenGL;
					foundAPI = true;
					break;
				}
				else if (api.find("Vulkan"))
				{
					SOF_CORE_THROW_INFO("Vulkan is not supported yet");
					RendererAPI::s_API = RendererAPI::API::Vulkan;
					foundAPI = true;
					break;
				}
			}
		}
		if (!foundAPI) RendererAPI::s_API = RendererAPI::API::DX11;
		m_GraphicsContext = GraphicsContext::Create();
		m_GraphicsContext->Init();
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
