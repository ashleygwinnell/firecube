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
	bool *GetIsOpenPtr();
private:

	void RenderChildren(NodeDescriptor *root);
	void SelectedNodeChanged(NodeDescriptor *node);

	NodeDescriptor *rootDesc;
	NodeDescriptor *newSelectedNode;
	NodeDescriptor *currentEditedNode;
	EditorState *editorState;
	bool isOpen;
	bool startRenaming;
};