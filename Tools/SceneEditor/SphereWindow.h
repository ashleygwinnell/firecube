#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"
#include "imguifilesystem.h"

class NodeDescriptor;
class EditorState;
class SphereDescriptor;

class SphereWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(SphereWindow);
public:
	SphereWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, SphereDescriptor *descriptor);
private:
	FloatInputHelper radiusInput;
	UIntInputHelper columnsInput;
	UIntInputHelper ringsInput;
	ImGuiFs::Dialog openDialog;
	HexInputHelper lightMaskInput;
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;
};