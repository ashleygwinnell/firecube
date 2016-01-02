#pragma once

#include "../mainframe.h"
#include "FireCube.h"
#include "../AssetUtils.h"
#include <wx/fswatcher.h>
class EditorState;

class AssetBrowserPanelImpl : public AssetBrowserPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(AssetBrowserPanelImpl)
public:
	AssetBrowserPanelImpl(wxWindow* parent);
	~AssetBrowserPanelImpl();
	void PopulateDirectoryTree();
	virtual void DirectoryTreeSelectionChanged(wxTreeEvent& event);
	virtual void FileListItemActivated(wxListEvent& event);
	virtual void FileListBeginDrag(wxListEvent& event);
	virtual void FileListKeyDown(wxListEvent& event);
	void SetAssetsPath(const std::string &path);
private:
	void OnFileSystemEvent(wxFileSystemWatcherEvent& event);
	void PopulateDirectoryNode(wxTreeItemId item, const std::string &path);
	void UpdateFileList(const std::string &path);
	
	EditorState *editorState;
	std::string currentFileName;
	wxFileSystemWatcher fsWatcher;
	std::map<std::string, wxTreeItemId> pathToTreeItem;
	std::string currentFileListPath;
	wxImageList *imageList;
};

class DirectoryItemData : public wxTreeItemData
{
public:
	DirectoryItemData(const std::string &path) : path(path) {}
	std::string path;	
};

class FileItemData
{
public:
	FileItemData(const std::string &path, bool isDirectory) : path(path), isDirectory(isDirectory) {}
	std::string path;
	bool isDirectory;
	AssetType assetType;
};
