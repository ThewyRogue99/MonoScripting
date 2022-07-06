#pragma once

#include <vector>

#include "ScriptField.h"

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoImage MonoImage;
typedef struct _MonoObject MonoObject;
typedef struct _MonoMethod MonoMethod;

struct MonoMethodDesc;

class ScriptCore
{
public:
	ScriptCore(const char* AppName = "Script App");
	~ScriptCore();

	bool Init(const char* AppName);

	void Deinit();

	bool AddScript(const char* Name);

	bool AddScripts(std::vector<const char*> Names);

	void Update();

private:
	void SetLibs();

	void SetConsoleLib();

private:
	struct ScriptObject
	{
		std::string Name = std::string();
		MonoObject* Object = nullptr;
		uint32_t GCHandle = 0;

		MonoMethod* BeginMethod = nullptr;
		MonoMethod* UpdateMethod = nullptr;

		std::vector<ScriptField> Fields = { };
	};

	bool bIsInit = false;

	std::vector<ScriptObject> ScriptObjects = { };

	MonoDomain* m_ptrMonoDomain = nullptr;
	MonoAssembly* m_ptrGameAssembly = nullptr;
	MonoImage* m_ptrGameAssemblyImage = nullptr;

	MonoMethodDesc* m_ptrBeginMethodDesc = nullptr;
	MonoMethodDesc* m_ptrUpdateMethodDesc = nullptr;
};