#include "ScriptCore.h"

#include "../Console.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>

#define STR_CONCAT(A, B) A##B
#define SOLUTION_REL_PATH(PATH) STR_CONCAT(_SOLUTIONDIR, PATH)

ScriptCore::ScriptCore(const char* AppName)
{
	Init(AppName);
}

ScriptCore::~ScriptCore()
{
	Deinit();
}

bool ScriptCore::Init(const char* AppName)
{
	if (!bIsInit)
	{
		mono_set_dirs(
			SOLUTION_REL_PATH("MonoScripting\\vendor\\Mono\\lib"),
			SOLUTION_REL_PATH("MonoScripting\\vendor\\Mono\\lib")
		);

		m_ptrMonoDomain = mono_jit_init(AppName);

		if (m_ptrMonoDomain)
		{
			m_ptrGameAssembly = mono_domain_assembly_open(m_ptrMonoDomain, SOLUTION_REL_PATH("x64\\Debug\\ScriptLib.dll"));

			if (m_ptrGameAssembly)
			{
				m_ptrGameAssemblyImage = mono_assembly_get_image(m_ptrGameAssembly);

				if (m_ptrGameAssemblyImage)
				{
					m_ptrBeginMethodDesc = mono_method_desc_new(".IScript:OnBegin()", false);
					m_ptrUpdateMethodDesc = mono_method_desc_new(".IScript:OnUpdate()", false);

					if (m_ptrBeginMethodDesc && m_ptrUpdateMethodDesc)
					{
						SetLibs();

						bIsInit = true;

						return true;
					}

					if (m_ptrBeginMethodDesc)
					{
						mono_method_desc_free(m_ptrBeginMethodDesc);
						m_ptrBeginMethodDesc = nullptr;
					}
					if (m_ptrUpdateMethodDesc)
					{
						mono_method_desc_free(m_ptrUpdateMethodDesc);
						m_ptrUpdateMethodDesc = nullptr;
					}
				}

				m_ptrGameAssemblyImage = nullptr;
			}

			m_ptrGameAssembly = nullptr;
		}

		m_ptrMonoDomain = nullptr;
	}
	else
		return true;

	return false;
}

void ScriptCore::Deinit()
{
	if (!bIsInit) return;

	for (auto& script : ScriptObjects)
	{
		mono_gchandle_free(script.GCHandle);
	}
	ScriptObjects.clear();

	if (m_ptrBeginMethodDesc)
		mono_method_desc_free(m_ptrBeginMethodDesc);
	if (m_ptrUpdateMethodDesc)
		mono_method_desc_free(m_ptrUpdateMethodDesc);

	if (m_ptrMonoDomain)
		mono_jit_cleanup(m_ptrMonoDomain);

	m_ptrMonoDomain = nullptr;
	m_ptrGameAssembly = nullptr;
	m_ptrGameAssemblyImage = nullptr;

	m_ptrBeginMethodDesc = nullptr;
	m_ptrUpdateMethodDesc = nullptr;
}

bool ScriptCore::AddScript(const char* Name)
{
	if (!bIsInit) return false;

	ScriptObject script = ScriptObject();

	MonoClass* ptrScriptClass = mono_class_from_name(m_ptrGameAssemblyImage, "ScriptLib", Name);

	if (ptrScriptClass)
	{
		script.Object = mono_object_new(m_ptrMonoDomain, ptrScriptClass);
		mono_runtime_object_init(script.Object);

		if (script.Object)
		{
			script.GCHandle = mono_gchandle_new(script.Object, false);
			if (script.GCHandle)
			{
				MonoMethod* beginVirtualMethod = mono_method_desc_search_in_class(m_ptrBeginMethodDesc, ptrScriptClass);

				if (beginVirtualMethod)
				{
					script.BeginMethod = mono_object_get_virtual_method(script.Object, beginVirtualMethod);

					MonoObject* ptrBeginExObject = nullptr;
					mono_runtime_invoke(script.BeginMethod, script.Object, nullptr, &ptrBeginExObject);

					if (ptrBeginExObject)
					{
						MonoString* exString = mono_object_to_string(ptrBeginExObject, nullptr);
						const char* exCString = mono_string_to_utf8(exString);

						Console::Print(exCString);

						mono_gchandle_free(script.GCHandle);
					}
					else
					{
						MonoMethod* updateVirtualMethod = mono_method_desc_search_in_class(m_ptrUpdateMethodDesc, ptrScriptClass);

						if (updateVirtualMethod)
						{
							script.UpdateMethod = mono_object_get_virtual_method(script.Object, updateVirtualMethod);

							script.Name = Name;

							int fieldCount = mono_class_num_fields(ptrScriptClass);
							MonoClassField* iterator = nullptr;

							for (int i = 0; i < fieldCount; i++)
							{
								mono_class_get_fields(ptrScriptClass, (void**)&iterator);

								if ((mono_field_get_flags(iterator) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK) == MONO_FIELD_ATTR_PRIVATE)
									continue;

								ScriptField field(script.Object, iterator);
								script.Fields.push_back(field);
							}

							ScriptObjects.push_back(script);

							return true;
						}
						else
						{
							mono_gchandle_free(script.GCHandle);

							Console::Print("Failed to find OnUpdate() Method\n");
						}
					}
				}
				else
				{
					mono_gchandle_free(script.GCHandle);

					Console::Print("Failed to find OnBegin() Method\n");
				}
			}
		}
		else
		{
			Console::Print("Failed to create an object\n");
		}
	}
	else
	{
		Console::Print("Failed to find class");
	}

	return false;
}

bool ScriptCore::AddScripts(std::vector<const char*> Names)
{
	if (!bIsInit) return false;

	bool bState = true;

	for (auto name : Names)
	{
		if (!AddScript(name))
			bState = false;
	}

	return bState;
}

void ScriptCore::Update()
{
	if (!bIsInit) return;

	for (auto& script : ScriptObjects)
	{
		MonoObject* ptrUpdateExObject = nullptr;
		mono_runtime_invoke(script.UpdateMethod, script.Object, nullptr, &ptrUpdateExObject);

		if (ptrUpdateExObject)
		{
			MonoString* exString = mono_object_to_string(ptrUpdateExObject, nullptr);
			const char* exCString = mono_string_to_utf8(exString);

			Console::Print(exCString);
		}
	}
}

void ScriptCore::SetLibs()
{
	if (!bIsInit) return;

	SetConsoleLib();
}

void ScriptCore::SetConsoleLib()
{
	mono_add_internal_call("ScriptLib.GameConsole::Write(string)", &Console::CSWrite);
	mono_add_internal_call("ScriptLib.GameConsole::WriteLine(string)", &Console::CSWriteLine);
}
