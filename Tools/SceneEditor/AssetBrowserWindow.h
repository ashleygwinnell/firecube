#pragma once

#include "FireCube.h"
#include "AssetUtils.h"
#include "AuxRenderWindow.h"

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
	class FileInfo
	{
	public:
		std::string path;
		std::string label;
		bool isDirectory;
		AssetType assetType;
	};
	std::vector<std::string> GetDirectoriesInPath(const std::string &path);
	std::vector<FileInfo> GetItemsInPath(const std::string &path);

	NodeDescriptor *rootDesc;
	EditorState *editorState;	
	bool firstRender;
	std::string selectedPath;
	FileInfo *selectedItem;
	std::vector<FileInfo> itemsInSelectedPath;
	FireCube::Texture2D *texturePreview;
	AuxRenderWindow auxRenderWindow;
};