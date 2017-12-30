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
	void RenderDirectoryTree(const std::string &path);
private:
	std::vector<std::string> GetDirectoriesInPath(const std::string &path);
	std::vector<std::string> GetFilesInPath(const std::string &path);

	NodeDescriptor *rootDesc;
	EditorState *editorState;	

	std::string selectedPath;
	std::vector<std::string> filesInSelectedPath;
};