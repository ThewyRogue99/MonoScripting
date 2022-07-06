#include "Application.h"

#include "Input.h"

#define STR_CONCAT(A, B) A##B
#define SOLUTION_REL_PATH(PATH) STR_CONCAT(_SOLUTIONDIR, PATH)

Application::Application(const std::string& Name) : AppName(Name)
{
	SCore.AddScripts({
		"ConsoleScript"
	});
}

Application::~Application()
{

}

void Application::Run()
{
	while (bIsRunning)
	{
		SCore.Update();

		if (Input::IsKeyDown(VK_ESCAPE))
			Shutdown();
	}
}

void Application::Shutdown()
{
	bIsRunning = false;
}
