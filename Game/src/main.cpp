#include <pch.h>
#include <Sofia.h>
#include <Sofia/EntryPoint.h>

#include "ExampleLayer.h"
#include <imgui.h>

Sofia::Application* Sofia::CreateApplication()
{
	ApplicationSpecifications spec;
	spec.WindowIconPath = L"assets/Icon.png";
	spec.CustomTitleBar = true;
	Application* app = new Application(spec);
	app->PushLayer(new ExampleLayer);
	app->SetMenuBarCallbackFunc([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close"))
				app->Close();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::MenuItem("Test2"))
				SOF_TRACE("Test");

			ImGui::EndMenu();
		}
	});
	return app;
}
