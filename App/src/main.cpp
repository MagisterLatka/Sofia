#include <pch.h>
#include "Sofia.h"
#include "Sofia/EntryPoint.h"
#include "ExampleLayer.h"

Sofia::Application* Sofia::CreateApplication()
{
	ApplicationSpecifications spec;
	Application* app = new Application(spec);
	ExampleLayer* layer = (ExampleLayer*)app->PushLayer(new ExampleLayer);
	return app;
}
