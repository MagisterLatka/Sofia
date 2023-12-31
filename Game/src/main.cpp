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
	ExampleLayer* layer = (ExampleLayer*)app->PushLayer(new ExampleLayer);
	app->SetMenuBarCallbackFunc([app, layer]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
				layer->NewScene();
			if (ImGui::MenuItem("Save", "Ctrl+S"))
				layer->SaveScene();
			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				layer->SaveSceneAs();
			if (ImGui::MenuItem("Open", "Ctrl+O"))
				layer->OpenScene();

			if (ImGui::MenuItem("Close"))
				app->Close();

			ImGui::EndMenu();
		}
	});
	return app;
}
