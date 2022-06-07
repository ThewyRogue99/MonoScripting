#pragma once

#include "Console.h"

class Application
{
public:
	void Run();

	void Shutdown();

private:
	bool bIsRunning = true;
};

