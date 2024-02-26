#include <pch.h>
#include "Sofia.h"
#include "Sofia/EntryPoint.h"
#include "ExampleLayer.h"
#include <imgui.h>

Sofia::Application* Sofia::CreateApplication()
{
	ApplicationSpecifications spec;
	spec.WindowIconPath = L"assets/Icon.png";
	spec.CustomTitleBar = true;
	spec.GraphicsAPI = Sofia::RendererAPI::API::OpenGL;
	Application* app = new Application(spec);
	ExampleLayer* layer = (ExampleLayer*)app->PushLayer(new ExampleLayer);
	app->SetMenuBarCallbackFunc([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close"))
				app->Close();

			ImGui::EndMenu();
		}
	});
	return app;
}
