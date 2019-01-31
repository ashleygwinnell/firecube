#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class LuaScriptDescriptor;

class LuaScriptWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(LuaScriptWindow);
public:
	LuaScriptWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, LuaScriptDescriptor *descriptor);
private:
	class Property
	{
	public:
		std::string name;
		FireCube::ScriptPropertyType type;
		std::string defaultValue;
		std::vector<std::string> options;
		TextInputHelper stringInput;
		FloatInputHelper floatInput;
		Vec2InputHelper vec2Input;
		Vec3InputHelper vec3Input;
		Vec4InputHelper vec4Input;
		ColorInputHelper colorInput;
		CheckBoxHelper boolInput;
	};
	std::string GetObjectNameFromScript(const std::string &filename);
	void UpdateScriptProperties(const std::string &filename, const std::string &objectName);
	
	TextInputHelper objectNameInput;
	std::vector<Property> properties;
	LuaScriptDescriptor *lastDescriptor;
};