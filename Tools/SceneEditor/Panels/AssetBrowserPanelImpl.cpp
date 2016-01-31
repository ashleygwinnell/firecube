#include "AssetBrowserPanelImpl.h"
#include "../app.h"
#include "../EditorState.h"
#include "../Commands/TransformCommands.h"
#include "../Commands/RenameNodeCommand.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../AssetUtils.h"
#include <wx/dir.h>
#include <wx/msgdlg.h> 
#include <wx/dataobj.h>
#include <wx/dnd.h>
#include <wx/textdlg.h> 
#include <fstream>

using namespace FireCube;

AssetBrowserPanelImpl::AssetBrowserPanelImpl(wxWindow* parent) : AssetBrowserPanel(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), editorState(((MyApp*)wxTheApp)->GetEditorState())
{
	fsWatcher.SetOwner(this);
	Bind(wxEVT_FSWATCHER, &AssetBrowserPanelImpl::OnFileSystemEvent, this);
	imageList = new wxImageList(32, 32);
	imageList->Add(wxArtProvider::GetIcon(wxART_FOLDER), wxSize(32, 32));
	imageList->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_MESSAGE_BOX), wxSize(32, 32));
	fileListCtrl->AssignImageList(imageList, wxIMAGE_LIST_NORMAL);	
}

AssetBrowserPanelImpl::~AssetBrowserPanelImpl()
{

}

void AssetBrowserPanelImpl::PopulateDirectoryTree()
{
	directoryTreeCtrl->DeleteAllItems();
	pathToTreeItem.clear();
	auto assetsFolder = Filesystem::GetAssetsFolder();
	if (assetsFolder.empty() == false)
	{				
		auto rootItem = directoryTreeCtrl->AddRoot("Assets", -1, -1, new DirectoryItemData(assetsFolder));
		pathToTreeItem[assetsFolder] = rootItem;
		PopulateDirectoryNode(rootItem, assetsFolder);		
		directoryTreeCtrl->Expand(rootItem);
	}		
}

void AssetBrowserPanelImpl::PopulateDirectoryNode(wxTreeItemId item, const std::string &path)
{
	wxDir dir(path);
	wxString dirName;
	bool cont = dir.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
	while (cont)
	{
		auto newPath = path + Filesystem::PATH_SEPARATOR + dirName.ToStdString();
		auto newItem = directoryTreeCtrl->AppendItem(item, dirName, -1, -1, new DirectoryItemData(newPath));
		pathToTreeItem[newPath] = newItem;
		PopulateDirectoryNode(newItem, newPath);
		cont = dir.GetNext(&dirName);
	}
}

void AssetBrowserPanelImpl::DirectoryTreeSelectionChanged(wxTreeEvent& event)
{
	DirectoryItemData *data = (DirectoryItemData *) directoryTreeCtrl->GetItemData(event.GetItem());
	UpdateFileList(data->path);
}

void AssetBrowserPanelImpl::UpdateFileList(const std::string &path)
{
	for (auto i = 0; i < fileListCtrl->GetItemCount(); ++i)
	{
		auto itemData = (FileItemData *) fileListCtrl->GetItemData(i);
		delete itemData;
	}

	fileListCtrl->ClearAll();
	wxDir dir(path);
	wxString itemName;
	bool cont = dir.GetFirst(&itemName);
	while (cont)
	{	
		std::string itemPath = path + Filesystem::PATH_SEPARATOR + itemName;
		bool isDirectory = wxFileName::DirExists(itemPath);
		auto itemData = new FileItemData(itemPath, isDirectory);		
		itemData->assetType = AssetUtils::GetAssetTypeByPath(itemPath);
		auto itemId = fileListCtrl->InsertItem(0, itemName, isDirectory ? 0 : 1);
		fileListCtrl->SetItemPtrData(itemId, (wxUIntPtr)itemData);
		cont = dir.GetNext(&itemName);
	}

	currentFileListPath = path;
}

void AssetBrowserPanelImpl::FileListItemActivated(wxListEvent& event)
{	
	auto itemData = (FileItemData *) event.GetItem().GetData();
	if (itemData->isDirectory)
	{
		auto nodeIter = pathToTreeItem.find(itemData->path);
		if (nodeIter != pathToTreeItem.end())
		{
			auto node = nodeIter->second;
			directoryTreeCtrl->Expand(node);
			directoryTreeCtrl->SelectItem(node);
			UpdateFileList(itemData->path);
		}
	}
	else
	{
		if (itemData->assetType == AssetType::MATERIAL)
		{
			std::string materialFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), itemData->path);
			std::replace(materialFileName.begin(), materialFileName.end(), '\\', '/');
			editorState->materialPicked(editorState, engine->GetResourceCache()->GetResource<Material>(materialFileName));
			editorState->showMaterialEditor(editorState);
		}
		else if (itemData->assetType == AssetType::MESH)
		{
			std::string meshFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), itemData->path);
			editorState->addMesh(editorState, meshFileName);
		}
		else if (itemData->assetType == AssetType::SCRIPT)
		{
			std::string scriptFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), itemData->path);			
			editorState->showScriptEditor(editorState, scriptFileName);
		}
		else if (itemData->assetType == AssetType::PREFAB)
		{
			std::string prefabFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), itemData->path);
			editorState->addPrefab(editorState, prefabFileName);
		}
	}
}

void AssetBrowserPanelImpl::SetAssetsPath(const std::string &path)
{
	fsWatcher.RemoveAll();
	fsWatcher.AddTree(wxFileName::DirName(path), wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE | wxFSW_EVENT_RENAME);
}

void AssetBrowserPanelImpl::OnFileSystemEvent(wxFileSystemWatcherEvent& event)
{
	switch (event.GetChangeType())
	{
	case wxFSW_EVENT_CREATE:
	{
		if (event.GetPath().IsDir() && event.GetPath().DirExists())
		{
			auto newPath = event.GetPath().GetFullPath().ToStdString();
			auto parent = pathToTreeItem[Filesystem::GetDirectoryName(newPath)];
			auto newItem = directoryTreeCtrl->AppendItem(parent, Filesystem::GetLastPathComponent(newPath), -1, -1, new DirectoryItemData(newPath));
			pathToTreeItem[newPath] = newItem;
		}
		
		std::string basePath = Filesystem::GetDirectoryName(event.GetPath().GetFullPath().ToStdString());
		if (basePath == currentFileListPath)
		{
			UpdateFileList(currentFileListPath);
		}

		break;
	}
	case wxFSW_EVENT_DELETE:
	{
		if (event.GetPath().IsDir() && event.GetPath().DirExists())
		{
			auto path = event.GetPath().GetFullPath().ToStdString();
			auto nodeIter = pathToTreeItem.find(path);
			if (nodeIter != pathToTreeItem.end())
			{
				directoryTreeCtrl->Delete(nodeIter->second);
				pathToTreeItem.erase(nodeIter);
			}
		}

		std::string basePath = Filesystem::GetDirectoryName(event.GetPath().GetFullPath().ToStdString());
		if (basePath == currentFileListPath)
		{
			UpdateFileList(currentFileListPath);
		}
	
		break;
	}
	case wxFSW_EVENT_RENAME:
	{
		if (event.GetPath().IsDir() && event.GetPath().DirExists())
		{
			std::string path = event.GetPath().GetFullPath().ToStdString();
			auto nodeIter = pathToTreeItem.find(path);
			if (nodeIter != pathToTreeItem.end())
			{
				auto node = nodeIter->second;
				std::string newPath = event.GetNewPath().GetFullPath().ToStdString();
				pathToTreeItem.erase(nodeIter);
				pathToTreeItem[newPath] = node;
				DirectoryItemData *data = (DirectoryItemData *)directoryTreeCtrl->GetItemData(node);
				data->path = newPath;
				directoryTreeCtrl->SetItemText(node, Filesystem::GetLastPathComponent(newPath));
			}
		}
		
		std::string basePath = Filesystem::GetDirectoryName(event.GetPath().GetFullPath().ToStdString());
		if (basePath == currentFileListPath)
		{
			UpdateFileList(currentFileListPath);
		}

		break;
	}
		
	default:
		break;
	}
}

void AssetBrowserPanelImpl::FileListBeginDrag(wxListEvent& event)
{
	auto itemData = (FileItemData *)event.GetItem().GetData();
	
	if (itemData->assetType == AssetType::MATERIAL || itemData->assetType == AssetType::TEXTURE || itemData->assetType == AssetType::TECHNIQUE || itemData->assetType == AssetType::MESH || itemData->assetType == AssetType::PREFAB)
	{
		wxCustomDataObject dataObject(wxDataFormat("Asset"));
		unsigned int size;
		char *data = AssetUtils::SerializeAssetDescription(itemData->assetType, itemData->path, size);
		dataObject.TakeData(size, data);
		wxDropSource source(dataObject, this);

		source.DoDragDrop(true);
	}	
}

void AssetBrowserPanelImpl::FileListKeyDown(wxListEvent& event)
{
	if (event.GetKeyCode() == WXK_DELETE)
	{
		auto itemData = (FileItemData *)event.GetItem().GetData();
		auto fileName = Filesystem::GetLastPathComponent(itemData->path);
		int ans = wxMessageBox("Delete " + fileName + "?", "Confirm", wxYES_NO);
		if (ans == wxYES)
		{
			
			wxRemoveFile(itemData->path);
		}
	}
}

wxImage ScaleImage(wxImage &image, int maxDimension)
{
	if (image.GetWidth() > maxDimension || image.GetHeight() > maxDimension)
	{
		int newWidth;
		int newHeight;
		if (image.GetWidth() > image.GetHeight())
		{
			newWidth = maxDimension;
			newHeight = (float)image.GetHeight() / (float)image.GetWidth() * maxDimension;
		}
		else
		{
			newHeight = maxDimension;
			newWidth = (float)image.GetWidth() / (float)image.GetHeight() * maxDimension;
		}

		return image.Scale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);
	}

	return image;
}

void AssetBrowserPanelImpl::FileListItemSelected(wxListEvent& event)
{
	auto itemData = (FileItemData *)event.GetItem().GetData();
	if (itemData->assetType == AssetType::TEXTURE)
	{
		currentTextureImage = wxImage(itemData->path, wxBITMAP_TYPE_ANY);		
		texturePreviewBitmap->GetParent()->Layout(); // Triggers a resize which displays the new image
	}
}

void AssetBrowserPanelImpl::TexturePreviewBitmapResize(wxSizeEvent& event)
{
	static bool inResize = false; // Prevent infinite recursive call due to Layout call below
	if (currentTextureImage.IsOk() && inResize == false)
	{
		inResize = true;
		texturePreviewBitmap->Freeze();
		auto scaledImage = ScaleImage(currentTextureImage, min(event.GetSize().GetWidth(), event.GetSize().GetHeight()));
		texturePreviewBitmap->SetBitmap(wxBitmap(scaledImage));
		texturePreviewBitmap->GetParent()->Layout();
		texturePreviewBitmap->Thaw();
		inResize = false;
	}
}

void AssetBrowserPanelImpl::FileListRightUp(wxMouseEvent& event)
{
	if (AssetUtils::GetAssetTypeByPath(currentFileListPath) == AssetType::SCRIPT)
	{
		wxMenu* menu = new wxMenu;
		auto newScriptItem = menu->Append(wxID_ANY, wxT("New Script"));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, [newScriptItem, this](wxCommandEvent &event) {
		if (event.GetId() == newScriptItem->GetId())
		{
			std::string scriptName = wxGetTextFromUser("Enter name of script object", "New Script").ToStdString();
			if (scriptName.empty() == false)
			{
				std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scripts" + Filesystem::PATH_SEPARATOR + scriptName + ".lua";				
				std::ofstream f(targetPath, std::ofstream::trunc);
				f << scriptName << " = Script()" << std::endl << std::endl;
				f << "function " << scriptName << ":Init()" << std::endl << std::endl;
				f << "end" << std::endl << std::endl;
				f << "function " << scriptName << ":Awake()" << std::endl << std::endl;
				f << "end" << std::endl << std::endl;
				f << "function " << scriptName << ":Update(time)" << std::endl << std::endl;
				f << "end" << std::endl;
				f.close();
				editorState->showScriptEditor(editorState, targetPath);
			}
		}
		});
		PopupMenu(menu);
		delete menu;
	}
}
