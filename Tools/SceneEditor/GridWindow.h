#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class GridDescriptor;

class GridWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(GridWindow);
public:
	GridWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, GridDescriptor *descriptor);
private:
	Vec2InputHelper sizeInput;
	UIntInputHelper countXInput;
	UIntInputHelper countZInput;
	HexInputHelper lightMaskInput;
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;
};