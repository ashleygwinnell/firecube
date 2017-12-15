#pragma once

#include "FireCube.h"

class NodeDescriptor;
class EditorState;

class HierarchyWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(HierarchyWindow);
public:
	HierarchyWindow(FireCube::Engine *engine);
	void Render();
	void SetScene(NodeDescriptor *rootDesc, EditorState *editorState);
private:

	void RenderChildren(NodeDescriptor *root);

	NodeDescriptor *rootDesc;
	EditorState *editorState;
};