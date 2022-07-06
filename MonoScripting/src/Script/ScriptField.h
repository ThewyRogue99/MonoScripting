#pragma once
#include <string>

typedef struct _MonoClassField MonoClassField;
typedef struct _MonoObject MonoObject;

class ScriptField
{
public:
	template<typename T>
	T as();

	template<typename T>
	void SetAs(const T& value);

	inline const std::string& GetName() const { return FieldName; }
	inline const std::string& GetFieldType() const { return FieldType; }

	friend class ScriptCore;

private:
	ScriptField(MonoObject* object, MonoClassField* field);

	MonoObject* FieldObject = nullptr;
	MonoClassField* Field = nullptr;

	std::string FieldName = std::string();
	std::string FieldType = std::string();
};