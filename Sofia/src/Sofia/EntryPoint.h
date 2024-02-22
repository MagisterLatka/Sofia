#pragma once

extern Sofia::Application* Sofia::CreateApplication();

int main(int argc, char** argv, char** envp)
{
	Sofia::InitializeCore(argc, argv, envp);

	Sofia::Application* app = nullptr;
	int returnVal = 0;
	try {
		app = Sofia::CreateApplication();
		SOF_CORE_ASSERT(app, "Could not initialize app!");
		returnVal = app->Run();
	}
#if defined(SOF_PLATFORM_WINDOWS)
	catch (const Sofia::SofiaException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
#else
	catch (const Sofia::SofiaException& e)
	{
		SOF_CORE_CRITICAL(e.GetType());
		SOF_CORE_ERROR(e.what());
	}
	catch (const std::exception& e)
	{
		SOF_CORE_CRITICAL("Standard Exception");
		SOF_CORE_ERROR(e.what());
	}
	catch (...)
	{
		SOF_CORE_CRITICAL("Unknown Exception");
		SOF_CORE_ERROR("No details available");
	}
#endif
	if (app) delete app;

	Sofia::ShutdownCore();
	return returnVal;
}

