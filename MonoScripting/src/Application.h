#pragma once

#include <string>

#include "Script/ScriptCore.h"

class Application
{
public:
	Application(const std::string& Name = "Application");

	~Application();

	void Run();

	void Shutdown();

private:
	std::string AppName;

	bool bIsRunning = true;

	ScriptCore SCore;
};

