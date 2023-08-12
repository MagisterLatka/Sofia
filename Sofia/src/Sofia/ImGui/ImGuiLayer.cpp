#include <pch.h>
#include "ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

#include "Sofia/Renderer/RendererAPI.h"
#include "Sofia/Application.h"
#include "Sofia/Platform/Win32/WindowsWindow.h"
#include "Sofia/Platform/DX11/DX11Context.h"

namespace Sofia {

	static void SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		io.FontDefault = io.Fonts->Fonts.back();

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Init();
	}
	void ImGuiLayer::Init()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported");  break;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); break;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); break;
			case RendererAPI::API::DX11:
			{
				auto window = Application::Get().GetWindow().As<WindowsWindow>();
				auto context = DX11Context::GetContextFromApplication();
				ImGui_ImplWin32_Init(window->GetNativeWindow());
				window->HandleInputForImgui();
				ImGui_ImplDX11_Init(context->GetDevice().Get(), context->GetContext().Get());
				break;
			}
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
		}
	}

	void ImGuiLayer::OnDetach()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported");  break;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); break;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); break;
			case RendererAPI::API::DX11:
			{
				ImGui_ImplDX11_Shutdown();
				ImGui_ImplWin32_Shutdown();
				break;
			}
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
		}
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (m_BlockEvents)
		{
			e.p_Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.p_Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported");  break;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); break;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); break;
			case RendererAPI::API::DX11:
			{
				ImGui_ImplWin32_NewFrame();
				ImGui_ImplDX11_NewFrame();
				break;
			}
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
		}
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());

		ImGui::Render();

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SOF_CORE_THROW_INFO("None API is not supported");  break;
			case RendererAPI::API::OpenGL:	SOF_CORE_THROW_INFO("OpenGL is not supported yet"); break;
			case RendererAPI::API::Vulkan:	SOF_CORE_THROW_INFO("Vulkan is not supported yet"); break;
			case RendererAPI::API::DX11:
			{
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}
				break;
			}
			case RendererAPI::API::DX12:	SOF_CORE_THROW_INFO("DirectX 12 is not supported yet"); break;
		}
	}
}
