#include "MainFrameImpl.h"
#include <wx/msgdlg.h>
#include "wx/filedlg.h"
#include "wx/dirdlg.h"
#include "app.h"
#include "Types.h"
#include "Commands/AddNodeCommand.h"
#include "Commands/RemoveNodeCommand.h"
#include "Commands/RenameNodeCommand.h"
#include "Commands/AddComponentCommand.h"
#include "Commands/GroupCommand.h"
#include "Commands/TransformCommands.h"
#include "SceneWriter.h"
#include "SceneSettings.h"
#include "BaseComponentPanelImpl.h"
#include "StaticModelPanelImpl.h"
#include "LightPanelImpl.h"
#include "NodePropertiesPanelImpl.h"
#include "LuaScriptPanelImpl.h"
#include "CollisionShapePanelImpl.h"
#include "CharacterControllerPanelImpl.h"

using namespace FireCube;

MainFrameImpl::MainFrameImpl(wxWindow* parent) : MainFrame(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), theApp((MyApp*)wxTheApp), editorState(theApp->GetEditorState()), 
												 sceneSettings(theApp->GetSceneSettings())
{
	SubscribeToEvent(editorState, editorState->selectedNodeChanged, &MainFrameImpl::SelectedNodeChanged);	
	SubscribeToEvent(editorState, editorState->componentAdded, &MainFrameImpl::ComponentAdded);
	SubscribeToEvent(editorState, editorState->componentRemoved, &MainFrameImpl::ComponentRemoved);
	SubscribeToEvent(editorState, editorState->nodeAdded, &MainFrameImpl::NodeAdded);
	SubscribeToEvent(editorState, editorState->nodeRemoved, &MainFrameImpl::NodeRemoved);	
	SubscribeToEvent(editorState, editorState->nodeRenamed, &MainFrameImpl::NodeRenamed);
	SubscribeToEvent(editorState, editorState->commandExecuted, &MainFrameImpl::UpdateUndoRedoMenu);
	SubscribeToEvent(editorState, editorState->undoPerformed, &MainFrameImpl::UpdateUndoRedoMenu);
	SubscribeToEvent(editorState, editorState->redoPerformed, &MainFrameImpl::UpdateUndoRedoMenu);
	m_mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_NONE);	
}

/*void MainFrameImpl::MyButtonClicked( wxCommandEvent& event )
{
	NodeDescriptor *nodeDescriptor = new NodeDescriptor(engine, sceneDescriptor, "TestNode");	
	StaticModel *model = nodeDescriptor->GetNode()->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh(new Mesh(engine));
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
	model->CreateFromMesh(mesh);
	model->SetCollisionQueryMask(USER_GEOMETRY);	
	editorState->ExecuteCommand(new AddNodeCommand(editorState, nodeDescriptor, rootDescriptor));
	this->glCanvas->Refresh(false);
	this->glCanvas->SetFocus();
}*/

void MainFrameImpl::AddNodeClicked(wxCommandEvent& event)
{
	Node *node = new Node(engine, "Node");
	auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", node, root);
	editorState->ExecuteCommand(addNodeCommand);
	this->glCanvas->SetFocus();	
}

void MainFrameImpl::AddMeshClicked(wxCommandEvent& event)
{
	static std::string lastPath = "";
	wxFileDialog openFileDialog(this, "Open", lastPath, "", "Mesh files  (*.3ds,*.dae,*.obj,*.fbx)|*.3ds;*.dae;*.obj;*.fbx", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	
	std::string sfile = openFileDialog.GetPath();
	
	Node *node = new Node(engine, "Node");
	auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", node, root);
	
	auto addComponentCommand = new AddComponentCommand(editorState, "Add Component", node, [sfile](Engine *engine, Node *node) -> Component *
	{
		StaticModel *model = node->CreateComponent<StaticModel>(engine->GetResourceCache()->GetResource<Mesh>(sfile));		
		return model;
	});

	GroupCommand *groupCommand = new GroupCommand(editorState, "Add Mesh", {addNodeCommand, addComponentCommand});
	editorState->ExecuteCommand(groupCommand);		
	wxString path;
	wxFileName::SplitPath(openFileDialog.GetPath(), &path, nullptr, nullptr, wxPATH_NATIVE);
	lastPath = path;
}

void MainFrameImpl::AddStaticModelClicked(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		static std::string lastPath = "";
		wxFileDialog openFileDialog(this, "Open", lastPath, "", "Mesh files  (*.3ds,*.dae,*.obj,*.fbx)|*.3ds;*.dae;*.obj;*.fbx", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (openFileDialog.ShowModal() == wxID_CANCEL)
			return;

		std::string sfile = openFileDialog.GetPath();
		
		auto addComponentCommand = new AddComponentCommand(editorState, "Add StaticModel",  node, [sfile](Engine *engine, Node *node) -> Component *
		{
			StaticModel *model = node->CreateComponent<StaticModel>(engine->GetResourceCache()->GetResource<Mesh>(sfile));			
			return model;
		});
		
		editorState->ExecuteCommand(addComponentCommand);		
		wxString path;
		wxFileName::SplitPath(openFileDialog.GetPath(), &path, nullptr, nullptr, wxPATH_NATIVE);
		lastPath = path;
	}
}

void MainFrameImpl::AddLightClicked(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		auto addComponentCommand = new AddComponentCommand(editorState, "Add Light", node, [](Engine *engine, Node *node) -> Component *
		{
			Light *light = node->CreateComponent<Light>();
			light->SetLightType(LightType::DIRECTIONAL);
			light->SetColor(vec4(1.0f));
			return light;
		});

		editorState->ExecuteCommand(addComponentCommand);
	}
}

void MainFrameImpl::AddLuaScriptClicked(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		auto addComponentCommand = new AddComponentCommand(editorState, "Add LuaScript", node, [](Engine *engine, Node *node) -> Component *
		{
			LuaScript *script = node->CreateComponent<LuaScript>();
			script->SetEnabled(false);
			return script;
		});

		editorState->ExecuteCommand(addComponentCommand);
	}
}

void MainFrameImpl::AddCollisionShapeClicked(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		auto addComponentCommand = new AddComponentCommand(editorState, "Add CollisionShape", node, [](Engine *engine, Node *node) -> Component *
		{
			CollisionShape *collisionShape = node->CreateComponent<CollisionShape>();			
			collisionShape->SetBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
			collisionShape->SetPlane(Plane(vec3(0.0f, 1.0f, 0.0f), 0.0f));
			return collisionShape;
		});

		editorState->ExecuteCommand(addComponentCommand);
	}
}

void MainFrameImpl::AddCharacterControllerClicked(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		auto addComponentCommand = new AddComponentCommand(editorState, "Add CharacterController", node, [](Engine *engine, Node *node) -> Component *
		{
			CharacterController *characterController = node->CreateComponent<CharacterController>();
			characterController->SetRadius(0.5f);
			characterController->SetHeight(1.0f);
			characterController->SetContactOffset(0.1f);
			return characterController;
		});

		editorState->ExecuteCommand(addComponentCommand);
	}
}


void MainFrameImpl::SetScene(FireCube::Scene *scene)
{
	engine = theApp->fcApp.GetEngine();
	this->scene = scene;
	root = scene->GetRootNode();
}

void MainFrameImpl::UndoClicked(wxCommandEvent& event)
{
	editorState->Undo();	
}

void MainFrameImpl::RedoClicked(wxCommandEvent& event)
{
	editorState->Redo();	
}

void MainFrameImpl::SaveClicked(wxCommandEvent& event)
{	
	if (editorState->GetCurrentSceneFile().empty())
	{
		SaveAsClicked(event);
	}
	else
	{
		SceneWriter sceneWriter;
		sceneWriter.Serialize(scene, sceneSettings, editorState->GetCurrentSceneFile());		
	}
}

void MainFrameImpl::SaveAsClicked(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, "Save Scene file", sceneSettings->basePath, "", "Scene files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	SceneWriter sceneWriter;
	sceneWriter.Serialize(scene, sceneSettings, saveFileDialog.GetPath().ToStdString());
	editorState->SetCurrentSceneFile(saveFileDialog.GetPath().ToStdString());
}

void MainFrameImpl::UpdateNode(Node *node)
{
	if (node->GetName().substr(0, 7) != "Editor_")
	{		
		editorState->nodeAdded(editorState, node);
		std::vector<StaticModel *> staticModels;
		node->GetComponents(staticModels);
		for (auto staticModel : staticModels)
		{
			//staticModel->SetCollisionQueryMask(USER_GEOMETRY);
		}

		for (auto component : node->GetComponents())
		{
			editorState->componentAdded(editorState, component);
		}

		for (auto child : node->GetChildren())
		{
			UpdateNode(child);
		}
	}
}

void MainFrameImpl::OpenClicked(wxCommandEvent& event)
{	
	wxFileDialog openFileDialog(this, "Open Scene file", sceneSettings->basePath, "", "Scene files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	wxString path;
	wxFileName::SplitPath(openFileDialog.GetPath(), &path, nullptr, nullptr, wxPATH_NATIVE);

	editorState->SetCurrentSceneFile(openFileDialog.GetPath().ToStdString());

	sceneSettings->basePath = path;
	Filesystem::AddSearchPath(sceneSettings->basePath);

	SceneReader sceneReader(engine);
	sceneReader.ReadSettings(openFileDialog.GetPath().ToStdString());
	sceneSettings->resourcePaths = sceneReader.GetResroucePaths();

	for (const auto &resourcePath : sceneSettings->resourcePaths)
	{
		std::string resourceFullpath = sceneSettings->basePath + (sceneSettings->basePath.back() == '/' || sceneSettings->basePath.back() == '\\' ? "" : "\\") + resourcePath;
		Filesystem::AddSearchPath(resourceFullpath);
	}
	
	editorState->ClearCommands();
	sceneTreeCtrl->DeleteAllItems();
	nodeToTreeItem.clear();
	treeItemToNode.clear();

	scene->GetRootNode()->RemoveAllComponents();
	
	if (sceneReader.Read(*scene, openFileDialog.GetPath().ToStdString()))
	{
		UpdateNode(scene->GetRootNode());
	}	

	editorState->sceneChanged(editorState);
}

void MainFrameImpl::SetBasePathClicked(wxCommandEvent& event)
{
	wxDirDialog dirDialog(this, "Choose base directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	
	if (dirDialog.ShowModal() == wxID_CANCEL)
		return;

	sceneSettings->basePath = dirDialog.GetPath().ToStdString();	
	Filesystem::AddSearchPath(sceneSettings->basePath);
}

void MainFrameImpl::AddResourcePathClicked(wxCommandEvent& event)
{
	wxDirDialog dirDialog(this, "Choose resource directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dirDialog.ShowModal() == wxID_CANCEL)
		return;
	
	sceneSettings->resourcePaths.push_back(dirDialog.GetPath().ToStdString());
	
	Filesystem::AddSearchPath(dirDialog.GetPath().ToStdString());
}

void MainFrameImpl::SelectedNodeChanged(FireCube::Node *node)
{
	if (node)
	{
		sceneTreeCtrl->SelectItem(nodeToTreeItem[node]);		
	}
	else
	{		
		sceneTreeCtrl->UnselectAll();
	}

	UpdateInpsectorPane();
}

void MainFrameImpl::NodeAdded(FireCube::Node *node)
{
	if (node->GetParent() == nullptr)
	{
		auto id = sceneTreeCtrl->AddRoot(node->GetName());
		nodeToTreeItem[node] = id;
		treeItemToNode[id] = node;
	}
	else if (nodeToTreeItem.find(node->GetParent()) != nodeToTreeItem.end())
	{
		wxTreeItemId parentId = nodeToTreeItem[node->GetParent()];
		auto id = sceneTreeCtrl->AppendItem(parentId, node->GetName());
		nodeToTreeItem[node] = id;
		treeItemToNode[id] = node;
	}
	
}

void MainFrameImpl::NodeRemoved(FireCube::Node *node)
{
	if (nodeToTreeItem.find(node) != nodeToTreeItem.end())
	{
		wxTreeItemId id = nodeToTreeItem[node];
		nodeToTreeItem.erase(node);
		treeItemToNode.erase(id);
		sceneTreeCtrl->Delete(id);		
	}
}

void MainFrameImpl::SceneTreeSelectionChanged(wxTreeEvent& event)
{
	auto node = treeItemToNode[event.GetItem()];
	if (node->GetParent() && node != editorState->GetSelectedNode())
	{
		editorState->SetSelectedNode(node);
	}
}

void MainFrameImpl::SceneTreeEndLabelEdit(wxTreeEvent& event)
{
	auto renameNodeCommand = new RenameNodeCommand(editorState, "Rename", treeItemToNode[event.GetItem()], event.GetLabel().ToStdString());
	editorState->ExecuteCommand(renameNodeCommand);
}

void MainFrameImpl::SceneTreeBeginDrag(wxTreeEvent& event)
{
	dragItem = event.GetItem();
	event.Allow();
}
void MainFrameImpl::SceneTreeEndDrag(wxTreeEvent& event)
{
	auto newParent = event.GetItem();
	if (newParent.IsOk() == false)
	{
		newParent = sceneTreeCtrl->GetRootItem();
	}			

	auto oldParent = sceneTreeCtrl->GetItemParent(dragItem);
	if (oldParent == newParent)
		return;

	auto newParentNode = treeItemToNode[newParent];
	auto node = treeItemToNode[dragItem];
	
	auto removeNodeCommand = new RemoveNodeCommand(editorState, "Remove Node", node);
	auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", node, newParentNode);
	auto groupCommand = new GroupCommand(editorState, "Reparent", { removeNodeCommand, addNodeCommand });

	editorState->ExecuteCommand(groupCommand);
}

void MainFrameImpl::NodeRenamed(FireCube::Node *node)
{
	auto itemId = nodeToTreeItem[node];
	sceneTreeCtrl->SetItemText(itemId, node->GetName());
}

void MainFrameImpl::ViewSceneHierarchyClicked(wxCommandEvent& event)
{
	auto &pane = m_mgr.GetPane("sceneHierarchyPane");
	if (event.IsChecked())
	{
		pane.Show();		
	}
	else
	{
		pane.Hide();		
	}

	m_mgr.Update();
}

void MainFrameImpl::ViewInspectorClicked(wxCommandEvent& event)
{
	auto &pane = m_mgr.GetPane("inspectorPane");
	if (event.IsChecked())
	{
		pane.Show();
	}
	else
	{
		pane.Hide();
	}

	m_mgr.Update();
}

void MainFrameImpl::PaneClose(wxAuiManagerEvent& event)
{
	auto pane = event.GetPane();

	if (pane->name == "sceneHierarchyPane")
	{
		viewSceneHierarchyMenuItem->Check(false);
	}

	if (pane->name == "inspectorPane")
	{
		viewInspectorMenuItem->Check(false);
	}
}

void MainFrameImpl::AddComponentPanel(Component *component)
{
	if (component->GetType() == StaticModel::GetTypeStatic())
	{
		auto t = new BaseComponentPanelImpl(componentsList, component);
		t->AddControl(new StaticModelPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (component->GetType() == Light::GetTypeStatic())
	{
		auto t = new BaseComponentPanelImpl(componentsList, component);
		t->AddControl(new LightPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (component->GetType() == LuaScript::GetTypeStatic())
	{
		auto t = new BaseComponentPanelImpl(componentsList, component);
		t->AddControl(new LuaScriptPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (component->GetType() == CollisionShape::GetTypeStatic())
	{
		auto t = new BaseComponentPanelImpl(componentsList, component);
		t->AddControl(new CollisionShapePanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (component->GetType() == CharacterController::GetTypeStatic())
	{
		auto t = new BaseComponentPanelImpl(componentsList, component);
		t->AddControl(new CharacterControllerPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}

	componentsList->FitInside();
	componentsList->Layout();
}

void MainFrameImpl::RemoveComponentPanel(Component *component)
{
	for (auto i = currentComponentPanels.begin(); i != currentComponentPanels.end(); ++i)
	{
		if ((*i)->GetComponent() == component)
		{
			(*i)->Destroy();
			currentComponentPanels.erase(i);
			return;
		}
	}
}

void MainFrameImpl::UpdateInpsectorPane()
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{				
		componentsList->Freeze();
		componentsList->DestroyChildren();
		currentComponentPanels.clear();

		componentsSizer->Add(new NodePropertiesPanelImpl(componentsList), 0, wxALL | wxEXPAND, 1);

		auto components = node->GetComponents();		
		for (auto component : components)
		{
			AddComponentPanel(component);
		}

		if (components.empty())
		{
			componentsList->FitInside();
			componentsList->Layout();
		}
		
		componentsList->Thaw();
	}
	else
	{		
		componentsList->Freeze();
		componentsList->DestroyChildren();
		componentsList->Layout();
		componentsList->Thaw();		
	}
}

void MainFrameImpl::ComponentAdded(Component *component)
{
	if (editorState->GetSelectedNode())
	{
		componentsList->Freeze();
		AddComponentPanel(component);
		componentsList->Thaw();
	}
}

void MainFrameImpl::ComponentRemoved(Component *component)
{
	if (editorState->GetSelectedNode())
	{
		componentsList->Freeze();
		RemoveComponentPanel(component);
		componentsList->Thaw();
	}
}

void MainFrameImpl::UpdateUndoRedoMenu(Command *command)
{
	if (editorState->HasUndo())
	{
		undoMenuItem->SetItemLabel(wxString(wxT("Undo \"")) + editorState->GetCurrentUndoCommand()->GetDescription() + wxT("\"\t") + wxT("Ctrl+Z"));
		undoMenuItem->Enable(true);
	}
	else
	{
		undoMenuItem->Enable(false);
		undoMenuItem->SetItemLabel(wxString(wxT("Undo")) + wxT('\t') + wxT("Ctrl+Z"));
	}
	
	if (editorState->HasRedo())
	{
		redoMenuItem->SetItemLabel(wxString(wxT("Redo \"")) + editorState->GetCurrentRedoCommand()->GetDescription() + wxT("\"\t") + wxT("Ctrl+Y"));
		redoMenuItem->Enable(true);
	}
	else
	{
		redoMenuItem->Enable(false);
		redoMenuItem->SetItemLabel(wxString(wxT("Redo")) + wxT('\t') + wxT("Ctrl+Y"));
	}
}