#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"
#include "imguifilesystem.h"

class NodeDescriptor;
class EditorState;
class BoxDescriptor;

class BoxWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(BoxWindow);
public:
	BoxWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, BoxDescriptor *descriptor);
private:
	Vec3InputHelper sizeInput;
	ImGuiFs::Dialog openDialog;
	HexInputHelper lightMaskInput;
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;
};