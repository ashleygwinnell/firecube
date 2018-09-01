#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class RigidBodyDescriptor;

class RigidBodyWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(RigidBodyWindow);
public:
	RigidBodyWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, RigidBodyDescriptor *descriptor);
private:
	FloatInputHelper massInput;
};