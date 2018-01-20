#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"
#include "imguifilesystem.h"

class NodeDescriptor;
class EditorState;
class PlaneDescriptor;

class PlaneWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(PlaneWindow);
public:
	PlaneWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, PlaneDescriptor *descriptor);
private:
	Vec2InputHelper sizeInput;
	ImGuiFs::Dialog openDialog;
	HexInputHelper lightMaskInput;
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;
};