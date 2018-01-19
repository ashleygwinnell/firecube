#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"
#include "imguifilesystem.h"

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
	std::string GetObjectNameFromScript(const std::string &filename);

	ImGuiFs::Dialog openDialog;
	TextInputHelper objectNameInput;	
	std::map<int, TextInputHelper> propertyNameInputs;
	std::map<int, TextInputHelper> propertyValueInputs;
};