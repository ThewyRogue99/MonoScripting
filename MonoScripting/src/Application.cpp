#include "Application.h"

#include "Input.h"

#define STR_CONCAT(A, B) A##B
#define SOLUTION_REL_PATH(PATH) STR_CONCAT(_SOLUTIONDIR, PATH)

Application::Application(const std::string& Name) : AppName(Name)
{
	mono_set_dirs(
		SOLUTION_REL_PATH("MonoScripting\\vendor\\Mono\\lib"),
		SOLUTION_REL_PATH("MonoScripting\\vendor\\Mono\\lib")
	);

	m_ptrMonoDomain = mono_jit_init(AppName.c_str());

	if (m_ptrMonoDomain)
	{
		int i = 0;
		return;
	}
}

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
