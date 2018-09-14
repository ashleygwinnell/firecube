#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class TerrainDescriptor;

class TerrainWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(TerrainWindow);
public:
	TerrainWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, TerrainDescriptor *descriptor);
private:
	HexInputHelper lightMaskInput;
	HexInputHelper collisionQueryMaskInput;
	CheckBoxHelper castShadowCheckBox;
	CheckBoxHelper generateHardNormalsCheckBox;
	Vec3InputHelper verticesSpacingInput;
};