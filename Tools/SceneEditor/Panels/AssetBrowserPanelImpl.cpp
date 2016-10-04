#define NOMINMAX
#include "AssetBrowserPanelImpl.h"
#include "../app.h"
#include "../EditorState.h"
#include "../Commands/TransformCommands.h"
#include "../Commands/RenameNodeCommand.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../AssetUtils.h"
#include "TexturePreviewPanelImpl.h"
#include "../AuxGLCanvas.h"
#include "../SceneReader.h"
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

void AssetBrowserPanelImpl::FileListItemSelected(wxListEvent& event)
{
	previewPanel->DestroyChildren();
	auto itemData = (FileItemData *)event.GetItem().GetData();

	if (itemData->assetType == AssetType::TEXTURE)
	{
		wxImage textureImage = wxImage(itemData->path, wxBITMAP_TYPE_ANY);

		TexturePreviewPanelImpl *texturePreviewPanel = new TexturePreviewPanelImpl(previewPanel, textureImage);
		previewPanelSizer->Add(texturePreviewPanel, 1, wxALL | wxEXPAND, 1);
		
		previewPanel->Layout();
	}
	else if (itemData->assetType == AssetType::MESH)
	{		
		AuxGLCanvas *glCanvas = new AuxGLCanvas(previewPanel);
		std::string meshPath = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), itemData->path);

		glCanvas->SetConstructSceneCallback([&meshPath, this](AuxGLCanvas *glCanvas) {
			auto root = glCanvas->GetRootNode();
			auto meshNode = root->CreateChild();						
			
			auto mesh = engine->GetResourceCache()->GetResource<Mesh>(meshPath);
			meshNode->CreateComponent<StaticModel>(mesh);
			auto bbox = mesh->GetBoundingBox();
			float maxSize = std::max(bbox.GetWidth(), std::max(bbox.GetHeight(), bbox.GetDepth()));
			meshNode->Scale(vec3(1.0f / maxSize));

			auto lightNode1 = root->CreateChild();
			auto light = lightNode1->CreateComponent<Light>();
			light->SetLightType(LightType::DIRECTIONAL);
			light->SetColor(1.0f);
			lightNode1->Rotate(vec3((float) -M_PI * 0.25f, (float) -M_PI * 0.25f, 0.0f));

			auto lightNode2 = root->CreateChild();
			light = lightNode2->CreateComponent<Light>();
			light->SetLightType(LightType::DIRECTIONAL);
			light->SetColor(1.0f);
			lightNode2->Rotate(vec3((float)M_PI * 0.25f, (float) M_PI * 0.25f, 0.0f));
		});		

		previewPanelSizer->Add(glCanvas, 1, wxALL | wxEXPAND, 1);

		previewPanel->Layout();
	}
	else if (itemData->assetType == AssetType::MATERIAL)
	{
		AuxGLCanvas *glCanvas = new AuxGLCanvas(previewPanel);
		std::string materialPath = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), itemData->path);

		glCanvas->SetConstructSceneCallback([&materialPath, this](AuxGLCanvas *glCanvas) {
			auto root = glCanvas->GetRootNode();
			auto sphereNode = root->CreateChild();

			auto material = engine->GetResourceCache()->GetResource<Material>(materialPath);

			auto geometry = GeometryGenerator::GenerateSphere(engine, 0.5f, 16, 16);
			SharedPtr<Mesh> sphereMesh = new Mesh(engine);
			sphereMesh->AddGeometry(geometry, BoundingBox(vec3(-0.5f), vec3(0.5f)), material);
			
			sphereNode->CreateComponent<StaticModel>(sphereMesh);			

			auto lightNode1 = root->CreateChild();
			auto light = lightNode1->CreateComponent<Light>();
			light->SetLightType(LightType::DIRECTIONAL);
			light->SetColor(1.0f);
			lightNode1->Rotate(vec3((float)-M_PI * 0.25f, (float)-M_PI * 0.25f, 0.0f));

			auto lightNode2 = root->CreateChild();
			light = lightNode2->CreateComponent<Light>();
			light->SetLightType(LightType::DIRECTIONAL);
			light->SetColor(1.0f);
			lightNode2->Rotate(vec3((float)M_PI * 0.25f, (float)M_PI * 0.25f, 0.0f));
		});

		previewPanelSizer->Add(glCanvas, 1, wxALL | wxEXPAND, 1);

		previewPanel->Layout();
	}
	else if (itemData->assetType == AssetType::PREFAB)
	{
		AuxGLCanvas *glCanvas = new AuxGLCanvas(previewPanel);
		std::string prefabPath = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), itemData->path);

		glCanvas->SetConstructSceneCallback([&prefabPath, this](AuxGLCanvas *glCanvas) {
			auto root = glCanvas->GetRootNode();
			
			std::map<Node *, NodeDescriptor *> nodeMap;
			::SceneReader sceneReader(engine, nodeMap);
			auto prefab = sceneReader.ReadPrefab(prefabPath);
			auto instance = prefab->Instantiate(nullptr, engine, nodeMap);						
			instance->SetTranslation(vec3(0.0f));
			instance->SetScale(vec3(1.0f));

			std::vector<StaticModel *> staticModels;
			std::vector<AnimatedModel *> animatedModels;
			instance->GetComponents(staticModels, true);
			BoundingBox bbox;
			for (auto &staticModel : staticModels)
			{
				bbox.Expand(staticModel->GetWorldBoundingBox());
			}

			for (auto &animatedModel : animatedModels)
			{
				bbox.Expand(animatedModel->GetWorldBoundingBox());
			}
			
			float scale = 1.0f / std::max(std::max(bbox.GetWidth(), bbox.GetHeight()), bbox.GetDepth());

			instance->SetTranslation(-bbox.GetCenter() * scale);			
			instance->SetScale(vec3(scale));
			root->AddChild(instance);

			auto lightNode1 = root->CreateChild();
			auto light = lightNode1->CreateComponent<Light>();
			light->SetLightType(LightType::DIRECTIONAL);
			light->SetColor(1.0f);
			lightNode1->Rotate(vec3((float)-M_PI * 0.25f, (float)-M_PI * 0.25f, 0.0f));

			auto lightNode2 = root->CreateChild();
			light = lightNode2->CreateComponent<Light>();
			light->SetLightType(LightType::DIRECTIONAL);
			light->SetColor(1.0f);
			lightNode2->Rotate(vec3((float)M_PI * 0.25f, (float)M_PI * 0.25f, 0.0f));
		});

		previewPanelSizer->Add(glCanvas, 1, wxALL | wxEXPAND, 1);

		previewPanel->Layout();
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
	else if (AssetUtils::GetAssetTypeByPath(currentFileListPath) == AssetType::MATERIAL)
	{
		wxMenu* menu = new wxMenu;
		auto newMaterialItem = menu->Append(wxID_ANY, wxT("New Material"));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, [newMaterialItem, this](wxCommandEvent &event) {
			if (event.GetId() == newMaterialItem->GetId())
			{
				std::string materialName = wxGetTextFromUser("Enter name of material", "New Material").ToStdString();
				if (materialName.empty() == false)
				{
					std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials" + Filesystem::PATH_SEPARATOR + materialName + ".xml";
					std::ofstream f(targetPath, std::ofstream::trunc);
					f << "<material name=\"" << materialName << "\">" << std::endl;
					f << "\t<technique name=\"Techniques/NoTexture.xml\" />" << std::endl;
					f << "\t<parameter name=\"materialDiffuse\" value=\"0.7 0.7 0.7\" />" << std::endl;
					f << "\t<parameter name=\"materialSpecular\" value=\"0 0 0\" />" << std::endl;
					f << "\t<parameter name=\"materialShininess\" value=\"0.000000\" />" << std::endl;
					f << "\t<parameter name=\"uOffset\" value=\"1 0 0\" />" << std::endl;
					f << "\t<parameter name=\"vOffset\" value=\"0 1 0\" />" << std::endl;
					f << "</material>" << std::endl;

					f.close();
					editorState->materialPicked(editorState, engine->GetResourceCache()->GetResource<Material>("Materials" + Filesystem::PATH_SEPARATOR + materialName + ".xml"));
					editorState->showMaterialEditor(editorState);					
				}
			}
		});
		PopupMenu(menu);
		delete menu;
	}
}
