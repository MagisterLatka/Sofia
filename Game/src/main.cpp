#include <pch.h>
#include <Sofia.h>
#include <Sofia/EntryPoint.h>

#include "ExampleLayer.h"

Sofia::Application* Sofia::CreateApplication()
{
	ApplicationSpecifications spec;
	spec.WindowIconPath = L"assets/Icon.png";
	spec.CustomTitleBar = true;
	Application* app = new Application(spec);
	app->PushLayer(new ExampleLayer);
	return app;
}
