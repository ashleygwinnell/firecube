#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class LightDescriptor;

class LightWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(LightWindow);
public:
	LightWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, LightDescriptor *descriptor);
private:
	HexInputHelper lightMaskInput;
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;	
	ColorInputHelper colorInput;
};