#include "Application.h"

#include "Input.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "Script/ScriptCore.h"

#define STR_CONCAT(A, B) A##B
#define SOLUTION_REL_PATH(PATH) STR_CONCAT(_SOLUTIONDIR, PATH)

Application::Application(const std::string& Name) : AppName(Name)
{
	SCore = ScriptCore(Name.c_str());

	SCore.AddScripts({
		"ConsoleScript"
	});
}

Application::~Application()
{
	SCore.Deinit();
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
