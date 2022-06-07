#include "Application.h"

#include "Input.h"

void Application::Run()
{
	bool bIsKeyReleased = true;

	while (bIsRunning)
	{
		if (Input::IsKeyDown(VK_TAB))
		{
			if (bIsKeyReleased)
				Console::Print("Hello %s\n", "World");

			bIsKeyReleased = false;
		}
		else
			bIsKeyReleased = true;

		if (Input::IsKeyDown(VK_ESCAPE))
			Shutdown();
	}
}

void Application::Shutdown()
{
	bIsRunning = false;
}
