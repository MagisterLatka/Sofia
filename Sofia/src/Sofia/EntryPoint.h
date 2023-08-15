#pragma once

extern Sofia::Application* Sofia::CreateApplication();

int main(int argc, char** argv, char** envp)
{
	Sofia::InitializeCore();

	Sofia::Application* app = nullptr;
	int returnVal = 0;
	try {
		app = Sofia::CreateApplication();
		SOF_CORE_ASSERT(app, "Could not initialize app!");
		returnVal = app->Run();
	}
	catch (const Sofia::SofiaException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception" ,MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	if (app) delete app;

	Sofia::ShutdownCore();
	return returnVal;
}
