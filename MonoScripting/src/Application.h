#pragma once

#include "Console.h"

#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

class Application
{
public:
	Application(const std::string& Name = "Application");

	void Run();

	void Shutdown();

private:
	std::string AppName;

	bool bIsRunning = true;

	MonoDomain* m_ptrMonoDomain = nullptr;
	MonoAssembly* m_ptrGameAssembly = nullptr;
	MonoImage* m_ptrGameAssemblyImage = nullptr;
};

