#include "ScriptField.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>

#include <cassert>

ScriptField::ScriptField(MonoObject* object, MonoClassField* field) : FieldObject(object), Field(field)
{
	FieldName = mono_field_get_name(field);
	FieldType = mono_type_full_name(mono_field_get_type(field));
}

template<typename T>
T ScriptField::As()
{
	assert(false && "Tried to get field in wrong type");
}

template<>
int ScriptField::As<int>()
{
	bool condition = (FieldType == "int");
	assert(condition && "Tried to get field in wrong type");

	int value = 0;
	if (condition)
	{
		mono_field_get_value(FieldObject, Field, &value);

		return value;
	}

	return 0;
}

template<>
std::wstring ScriptField::As<std::wstring>()
{
	bool condition = (FieldType == "string" || FieldType == "String");
	assert(condition && "Tried to get field in wrong type");

	if (condition)
	{
		MonoString* str = nullptr;

		mono_field_get_value(FieldObject, Field, &str);

		return mono_string_to_utf16(str);
	}

	return std::wstring();
}

template<typename T>
void ScriptField::SetAs(const T& value)
{
	assert(false && "Tried to set field in wrong type");
}

template<>
void ScriptField::SetAs<int>(const int& value)
{
	bool condition = (FieldType == "int");
	assert(condition && "Tried to get field in wrong type");

	if (condition)
		mono_field_set_value(FieldObject, Field, (int*)(&value));
}

template<>
void ScriptField::SetAs<std::wstring>(const std::wstring& value)
{
	bool condition = (FieldType == "string" || FieldType == "String");
	assert(condition && "Tried to get field in wrong type");

	if (condition)
	{
		MonoString* str = mono_string_from_utf16((mono_unichar2*)(value.c_str()));

		mono_field_set_value(FieldObject, Field, str);
	}
}