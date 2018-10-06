#pragma once

#include "FireCube.h"
#include "AssetUtils.h"
#include "ImguiHelpers.h"

class EditorState;
class Command;
class NodeDescriptor;

class RenderingWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(RenderingWindow);
public:
	RenderingWindow(FireCube::Engine *engine);
	void Render();
	void SetScene(FireCube::Scene *scene, NodeDescriptor *rootDesc, EditorState *editorState);	
	bool *GetIsOpenPtr();

private:
	NodeDescriptor *rootDesc;
	EditorState *editorState;
	FireCube::Scene *scene;
	bool firstRender;	
	bool isOpen;

	ColorInputHelper fogColorInput;
};