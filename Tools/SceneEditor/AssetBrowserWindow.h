#pragma once

#include "FireCube.h"

class NodeDescriptor;
class EditorState;
class Command;

class AssetBrowserWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(AssetBrowserWindow);
public:
	AssetBrowserWindow(FireCube::Engine *engine);
	void Render();
	void SetScene(NodeDescriptor *rootDesc, EditorState *editorState);
private:
	NodeDescriptor *rootDesc;
	EditorState *editorState;	
};