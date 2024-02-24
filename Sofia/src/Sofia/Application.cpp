#include <pch.h>
#include "Application.h"

#include "Sofia/Renderer/Renderer.h"
#include "Sofia/ImGui/ImGui.h"

#include "Sofia/Embed/DefaultIcon.embed"
#include "Sofia/Embed/WindowIcons.embed"

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
		windowProps.Resizable = m_Specs.ResizableWindow;
		windowProps.HasTitleBar = !m_Specs.CustomTitleBar;
		m_Window = Window::Create(windowProps);
		m_Window->SetEventCallback(SOF_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetTitleBarHitTestCallback([this](const int xMousePos, const int yMousePos, int& hitTest)
		{
			hitTest = m_TitleBarHovered;
		});
		if (!m_Specs.WindowIconPath.empty())
			m_Window->SetIcon(m_Specs.WindowIconPath);

		//init renderer command queue & shader library
		Renderer::Init();

		//init ui layer
		m_ImGuiLayer = new ImGuiLayer;
		m_LayerStack->PushOverlay(m_ImGuiLayer);

		//call OnAttach() on layers in LayerStack
		m_LayerStack->Init(); 
		
		uint32_t width, height;
		void* data = nullptr;
		if (m_Specs.TitlebarIconData)
			data = Texture2D::Decode(m_Specs.TitlebarIconData.Data, m_Specs.TitlebarIconData.Size, width, height);
		else
			data = Texture2D::Decode((void*)s_DefaultIconData, sizeof(s_DefaultIconData), width, height);
		m_TitlebarIcon = Texture2D::Create(width, height, data);

		data = Texture2D::Decode((void*)s_WindowCloseIcon, sizeof(s_WindowCloseIcon), width, height);
		m_CloseIcon = Texture2D::Create(width, height, data);

		data = Texture2D::Decode((void*)s_WindowMaximizeIcon, sizeof(s_WindowMaximizeIcon), width, height);
		m_MaximizeIcon = Texture2D::Create(width, height, data);

		data = Texture2D::Decode((void*)s_WindowMinimizeIcon, sizeof(s_WindowMinimizeIcon), width, height);
		m_MinimizeIcon = Texture2D::Create(width, height, data);

		data = Texture2D::Decode((void*)s_WindowRestoreIcon, sizeof(s_WindowRestoreIcon), width, height);
		m_RestoreIcon = Texture2D::Create(width, height, data);

		Renderer::Render();
	}
	void Application::Shutdown()
	{
		m_LayerStack.reset();

		m_TitlebarIcon.Reset();
		m_CloseIcon.Reset();
		m_MinimizeIcon.Reset();
		m_MaximizeIcon.Reset();
		m_RestoreIcon.Reset();

		Renderer::Shutdown();
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

			//update ui
			m_ImGuiLayer->Begin();
			ImGuiRender();
			m_ImGuiLayer->End();

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

	void Application::ImGuiRender()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		if (!m_Specs.CustomTitleBar && m_MenuBarCallback)
			windowFlags |= ImGuiWindowFlags_MenuBar;

		const bool isMaximized = m_Window->IsMaximized();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

		ImGui::Begin("DockSpaceWindow", nullptr, windowFlags);

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(4);

		if (!m_Window->IsMaximized())
		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0x30u, 0x30u, 0x30u, 0xffu));
			UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());
			ImGui::PopStyleColor();
		}

		if (m_Specs.CustomTitleBar)
		{
			float titlebarHeight;
			DrawTitleBar(titlebarHeight);
			ImGui::SetCursorPosY(titlebarHeight);
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		style.WindowMinSize.x = minWinSizeX;

		if (!m_Specs.CustomTitleBar)
			DrawMenuBarUI();

		for (auto layer : *m_LayerStack)
			layer->OnUIRender();

		ImGui::End();
	}
	void Application::DrawTitleBar(float& outTitlebarHeight)
	{
		constexpr float titlebarHeight = 58.0f;
		const bool isMaximized = m_Window->IsMaximized();
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
		constexpr ImU32 debugColor = IM_COL32(0xffu, 0x20u, 0x20u, 0xffu);

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									 ImGui::GetCursorScreenPos().y + titlebarHeight };
		auto* backgroundDrawList = ImGui::GetBackgroundDrawList();
		auto* foregroundDrawList = ImGui::GetForegroundDrawList();
		backgroundDrawList->AddRectFilled(titlebarMin, titlebarMax, IM_COL32(0x18u, 0x18u, 0x18u, 0xffu));
		//debug title bar bounds
		//foregroundDrawList->AddRect(titlebarMin, titlebarMax, debugColor);

		const int iconWidth = 48;
		const int iconHeight = 48;
		const ImVec2 iconOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
		const ImVec2 iconRectMin = { ImGui::GetItemRectMin().x + iconOffset.x, ImGui::GetItemRectMin().y + iconOffset.y };
		const ImVec2 iconRectMax = { iconRectMin.x + iconWidth, iconRectMin.y + iconHeight };
		foregroundDrawList->AddImage(m_TitlebarIcon->GetRawPointer(), iconRectMin, iconRectMax);

		ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

		const float width = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 112.0f;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		//debug drag bounds
		//foregroundDrawList->AddRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + width - buttonsAreaWidth,
		//			ImGui::GetCursorScreenPos().y + titlebarHeight), debugColor);
		ImGui::SetNextItemAllowOverlap();
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(width - buttonsAreaWidth, titlebarHeight));

		m_TitleBarHovered = ImGui::IsItemHovered();

		if (isMaximized)
		{
			float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
			if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
				m_TitleBarHovered = true;
		}

		if (m_MenuBarCallback)
		{
			ImGui::SuspendLayout();

			const float iconHorizontalOffset = 16.0f * 2.0f + (float)iconWidth + windowPadding.x;
			ImGui::SetCursorPos(ImVec2(iconHorizontalOffset, 6.0f + titlebarVerticalOffset));
			DrawMenuBarUI();

			if (ImGui::IsItemHovered())
				m_TitleBarHovered = false;

			ImGui::ResumeLayout();
		}

		const ImVec2 currentCursorPos = ImGui::GetCursorPos();
		const ImVec2 textSize = ImGui::CalcTextSize(m_Specs.Name.c_str());
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
		ImGui::Text(m_Specs.Name.c_str());
		ImGui::SetCursorPos(currentCursorPos);

		const ImU32 buttonNormalColor = IM_COL32(0xb0u, 0xb0u, 0xb0u, 0xffu);
		const ImU32 buttonHoveredColor = IM_COL32(0xd0u, 0xd0u, 0xd0u, 0xffu);
		const ImU32 buttonPressedColor = IM_COL32(0xd8u, 0xd8u, 0xd8u, 0xffu);
		const float buttonWidth = 14.0f;
		const float buttonHeight = 14.0f;

		ImGui::Spring();
		UI::ShiftCursorX(8.0f);
		{
			const uint32_t iconWidth = m_MinimizeIcon->GetWidth();
			const uint32_t iconHeight = m_MinimizeIcon->GetHeight();
			const float pad = (buttonHeight - (float)iconHeight) / 2.0f;
			if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
				m_Window->Minimize();

			UI::DrawButtonImage(m_MinimizeIcon, buttonNormalColor, buttonHoveredColor, buttonPressedColor, UI::RectExpand(UI::GetItemRect(), 0.0f, -pad));
		}

		ImGui::Spring(-1.0f, 17.0f);
		UI::ShiftCursorX(8.0f);
		{
			const uint32_t iconWidth = m_MaximizeIcon->GetWidth();
			const uint32_t iconHeight = m_MaximizeIcon->GetHeight();
			const bool maximized = m_Window->IsMaximized();

			if (ImGui::InvisibleButton("Maximize", ImVec2(buttonWidth, buttonHeight)))
			{
				if (maximized)
					m_Window->Restore();
				else
					m_Window->Maximize();
			}

			UI::DrawButtonImage(maximized ? m_RestoreIcon : m_MaximizeIcon, buttonNormalColor, buttonHoveredColor, buttonPressedColor, UI::GetItemRect());
		}

		ImGui::Spring(-1.0f, 15.0f);
		UI::ShiftCursorX(8.0f);
		{
			const uint32_t iconWidth = m_CloseIcon->GetWidth();
			const uint32_t iconHeight = m_CloseIcon->GetHeight();

			if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
				m_Running = false;

			UI::DrawButtonImage(m_CloseIcon, buttonNormalColor, buttonHoveredColor, buttonPressedColor, UI::GetItemRect());
		}

		ImGui::Spring(-1.0f, 18.0f);
		ImGui::EndHorizontal();
		outTitlebarHeight = titlebarHeight;
	}
	void Application::DrawMenuBarUI()
	{
		if (!m_MenuBarCallback)
			return;

		if (m_Specs.CustomTitleBar)
		{
			const ImRect menuBarRect = { ImGui::GetCursorPos(), { ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing() } };

			ImGui::BeginGroup();
			if (UI::BeginMenuBar(menuBarRect))
			{
				m_MenuBarCallback();
				UI::EndMenuBar();
			}
			ImGui::EndGroup();
		}
		else
		{
			if (ImGui::BeginMenuBar())
			{
				m_MenuBarCallback();
				ImGui::EndMenuBar();
			}
		}
	}
}
