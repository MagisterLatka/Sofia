#include <pch.h>
#include <Sofia.h>
#include <Sofia/EntryPoint.h>

#include "ExampleLayer.h"

class Game : public Sofia::Application
{
public:
	Game(Sofia::ApplicationCommandLineArgs args)
		: Sofia::Application("Game", args)
	{
		PushLayer(new ExampleLayer);
	}

	~Game() {}
};

Sofia::Application* Sofia::CreateApplication(ApplicationCommandLineArgs args)
{
	return new Game(args);
}
