#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"
#include "StaticModelWindow.h"
#include "LightWindow.h"
#include "LuaScriptWindow.h"

class NodeDescriptor;
class EditorState;
class Command;

class InspectorWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(InspectorWindow);
public:
	InspectorWindow(FireCube::Engine *engine);
	void Render();
	void SetScene(NodeDescriptor *rootDesc, EditorState *editorState);
private:
	NodeDescriptor * rootDesc;
	EditorState *editorState;
	FireCube::vec3 prevValue;
	Vec3InputHelper translationInput;
	Vec3InputHelper rotationInput;
	Vec3InputHelper scaleInput;

	StaticModelWindow staticModelWindow;
	LightWindow lightWindow;
	LuaScriptWindow luaScriptWindow;
};