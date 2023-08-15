#include <pch.h>
#include <Sofia.h>
#include <Sofia/EntryPoint.h>

#include "ExampleLayer.h"

Sofia::Application* Sofia::CreateApplication()
{
	ApplicationSpecifications spec;
	spec.IconPath = L"assets/Icon.png";
	Application* app = new Application(spec);
	app->PushLayer(new ExampleLayer);
	return app;
}
