#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

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
	bool isActive;
	Command *prevCommand;
	Vec3InputHelper translationInput;
	Vec3InputHelper rotationInput;
	Vec3InputHelper scaleInput;
};