#pragma once

#include "Console.h"

class Application
{
public:
	Application() : console(new Console()) { }

	void Run();

private:
	Console* console;
};

