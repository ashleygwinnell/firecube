#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class StaticModelDescriptor;

class StaticModelWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(StaticModelWindow);
public:
	StaticModelWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, StaticModelDescriptor *descriptor);
private:
	HexInputHelper lightMaskInput;	
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;
};