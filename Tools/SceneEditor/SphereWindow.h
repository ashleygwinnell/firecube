#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

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
	HexInputHelper lightMaskInput;
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;
};