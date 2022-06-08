#include "Application.h"

int main()
{
	Application* app = new Application("Mono Scripting");

	app->Run();

	delete app;

	return 1;
}