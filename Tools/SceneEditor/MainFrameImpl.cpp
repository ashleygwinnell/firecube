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
#include "BaseComponentPanelImpl.h"
#include "StaticModelPanelImpl.h"
#include "LightPanelImpl.h"
#include "NodePropertiesPanelImpl.h"
#include "LuaScriptPanelImpl.h"
#include "CollisionShapePanelImpl.h"
#include "CharacterControllerPanelImpl.h"
#include "AssetUtils.h"
#include "SceneReader.h"
#include "Descriptors/ComponentDescriptor.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "Descriptors/LightDescriptor.h"
#include "Descriptors/CollisionShapeDescriptor.h"
#include "Descriptors/CharacterControllerDescriptor.h"
#include "Descriptors/LuaScriptDescriptor.h"

using namespace FireCube;

MainFrameImpl::MainFrameImpl(wxWindow* parent) : MainFrame(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), theApp((MyApp*)wxTheApp), editorState(theApp->GetEditorState())										 
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
	SetAllPanelsVisibility(false);
}

void MainFrameImpl::AddNodeClicked(wxCommandEvent& event)
{	
	auto nodeDesc = new NodeDescriptor("Node");
	auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", nodeDesc, &rootDesc);
	editorState->ExecuteCommand(addNodeCommand);
	this->glCanvas->SetFocus();	
}

void MainFrameImpl::AddMeshClicked(wxCommandEvent& event)
{
	static std::string lastPath = "";
	wxFileDialog openFileDialog(this, "Open", lastPath, "", "Mesh files  (*.3ds,*.dae,*.obj,*.fbx)|*.3ds;*.dae;*.obj;*.fbx", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	
	std::string sfile = openFileDialog.GetPath().ToStdString();

	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), sfile))
	{
		sfile = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), sfile);		
	}
	else
	{
		AssetUtils::ImportMesh(engine, sfile);
		sfile = "Models" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);		
	}

	auto nodeDesc = new NodeDescriptor("Node");
	auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", nodeDesc, &rootDesc);
	auto staticModelDescriptor = new StaticModelDescriptor();
	staticModelDescriptor->SetMeshFilename(sfile, engine);

	auto addComponentCommand = new AddComponentCommand(editorState, "Add Component", nodeDesc, staticModelDescriptor, engine);

	GroupCommand *groupCommand = new GroupCommand(editorState, "Add Mesh", { addNodeCommand, addComponentCommand });
	editorState->ExecuteCommand(groupCommand);

	wxString path;
	wxFileName::SplitPath(openFileDialog.GetPath(), &path, nullptr, nullptr, wxPATH_NATIVE);
	lastPath = path;
}

void MainFrameImpl::AddStaticModelClicked(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{	
		auto staticModelDescriptor = new StaticModelDescriptor();
		auto addComponentCommand = new AddComponentCommand(editorState, "Add StaticModel", nodeDesc, staticModelDescriptor, engine);
		editorState->ExecuteCommand(addComponentCommand);				
	}
}

void MainFrameImpl::AddLightClicked(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{
		auto lightDescriptor = new LightDescriptor();
		lightDescriptor->SetLightType(LightType::DIRECTIONAL);
		lightDescriptor->SetColor(vec4(1.0f));		
		auto addComponentCommand = new AddComponentCommand(editorState, "Add Light", nodeDesc, lightDescriptor, engine);
		editorState->ExecuteCommand(addComponentCommand);
	}
}

void MainFrameImpl::AddLuaScriptClicked(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{
		auto luaScriptDescriptor = new LuaScriptDescriptor();
		auto addComponentCommand = new AddComponentCommand(editorState, "Add LuaScript", nodeDesc, luaScriptDescriptor, engine);

		editorState->ExecuteCommand(addComponentCommand);
	}
}

void MainFrameImpl::AddCollisionShapeClicked(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{
		auto collisionShapeDescriptor = new CollisionShapeDescriptor();
		collisionShapeDescriptor->SetBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
		collisionShapeDescriptor->SetPlane(Plane(vec3(0.0f, 1.0f, 0.0f), 0.0f));
		auto addComponentCommand = new AddComponentCommand(editorState, "Add CollisionShape", nodeDesc, collisionShapeDescriptor, engine);

		editorState->ExecuteCommand(addComponentCommand);
	}
}

void MainFrameImpl::AddCharacterControllerClicked(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{
		auto characterControllerDescriptor = new CharacterControllerDescriptor();
		characterControllerDescriptor->SetRadius(0.5f);
		characterControllerDescriptor->SetHeight(1.0f);
		characterControllerDescriptor->SetContactOffset(0.1f);
		auto addComponentCommand = new AddComponentCommand(editorState, "Add CharacterController", nodeDesc, characterControllerDescriptor, engine);

		editorState->ExecuteCommand(addComponentCommand);
	}
}


void MainFrameImpl::SetScene(FireCube::Scene *scene)
{
	engine = theApp->fcApp.GetEngine();
	this->scene = scene;
	root = scene->GetRootNode();
	rootDesc.SetNode(root);
	editorState->GetNodeMap()[root] = &rootDesc;
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
		sceneWriter.Serialize(&rootDesc, editorState->GetCurrentSceneFile());		
	}
}

void MainFrameImpl::SaveAsClicked(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, "Save Scene file", Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scenes", "", "Scene files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	SceneWriter sceneWriter;
	sceneWriter.Serialize(&rootDesc, saveFileDialog.GetPath().ToStdString());
	editorState->SetCurrentSceneFile(saveFileDialog.GetPath().ToStdString());
}

void MainFrameImpl::UpdateNode(NodeDescriptor *nodeDesc)
{
	editorState->nodeAdded(editorState, nodeDesc);
	editorState->GetNodeMap()[nodeDesc->GetNode()] = nodeDesc;

	for (auto child : nodeDesc->GetChildren())
	{
		UpdateNode(child);
	}
}

void MainFrameImpl::OpenClicked(wxCommandEvent& event)
{	
	wxFileDialog openFileDialog(this, "Open Scene file", "", "", "Scene files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	SetAllPanelsVisibility(true);

	std::string path = openFileDialog.GetPath().ToStdString();
	
	editorState->SetCurrentSceneFile(path);
	
	Filesystem::SetAssetsFolder(Filesystem::GetDirectoryName(Filesystem::GetDirectoryName(path)));

	::SceneReader sceneReader(engine, editorState);
	
	editorState->ClearCommands();
	sceneTreeCtrl->DeleteAllItems();
	nodeToTreeItem.clear();
	treeItemToNode.clear();	
	editorState->GetNodeMap().clear();

	scene->GetRootNode()->RemoveAllComponents();
	rootDesc.RemoveAllComponents();
	
	if (sceneReader.Read(&rootDesc, openFileDialog.GetPath().ToStdString()))
	{
		UpdateNode(&rootDesc);
	}	

	editorState->sceneChanged(editorState);	
}

void MainFrameImpl::NewClicked(wxCommandEvent& event)
{
	wxDirDialog dirDialog(this, "Choose root assets directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dirDialog.ShowModal() == wxID_CANCEL)
		return;

	std::string path = dirDialog.GetPath().ToStdString();
	Filesystem::SetAssetsFolder(path);

	Filesystem::CreateFolder(Filesystem::RemoveLastSeparator(path) + Filesystem::PATH_SEPARATOR + "Scenes");

	SetAllPanelsVisibility(true);
}

void MainFrameImpl::SelectedNodeChanged(NodeDescriptor *nodeDesc)
{
	if (nodeDesc)
	{
		sceneTreeCtrl->SelectItem(nodeToTreeItem[nodeDesc]);
	}
	else
	{		
		sceneTreeCtrl->UnselectAll();
	}

	UpdateInpsectorPane();
}

void MainFrameImpl::NodeAdded(NodeDescriptor *nodeDesc)
{
	if (nodeDesc->GetParent() == nullptr)
	{
		auto id = sceneTreeCtrl->AddRoot(nodeDesc->GetName());
		nodeToTreeItem[nodeDesc] = id;
		treeItemToNode[id] = nodeDesc;
	}
	else if (nodeToTreeItem.find(nodeDesc->GetParent()) != nodeToTreeItem.end())
	{
		wxTreeItemId parentId = nodeToTreeItem[nodeDesc->GetParent()];
		auto id = sceneTreeCtrl->AppendItem(parentId, nodeDesc->GetName());
		nodeToTreeItem[nodeDesc] = id;
		treeItemToNode[id] = nodeDesc;
	}
	
}

void MainFrameImpl::NodeRemoved(NodeDescriptor *nodeDesc)
{
	if (nodeToTreeItem.find(nodeDesc) != nodeToTreeItem.end())
	{
		wxTreeItemId id = nodeToTreeItem[nodeDesc];
		nodeToTreeItem.erase(nodeDesc);
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

void MainFrameImpl::NodeRenamed(NodeDescriptor *nodeDesc)
{
	auto itemId = nodeToTreeItem[nodeDesc];
	sceneTreeCtrl->SetItemText(itemId, nodeDesc->GetNode()->GetName());
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

void MainFrameImpl::AddComponentPanel(ComponentDescriptor *componentDesc)
{
	if (componentDesc->GetType() == ComponentType::STATIC_MODEL)
	{
		auto t = new BaseComponentPanelImpl(componentsList, componentDesc);
		t->AddControl(new StaticModelPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (componentDesc->GetType() == ComponentType::LIGHT)
	{
		auto t = new BaseComponentPanelImpl(componentsList, componentDesc);
		t->AddControl(new LightPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (componentDesc->GetType() == ComponentType::LUA_SCRIPT)
	{
		auto t = new BaseComponentPanelImpl(componentsList, componentDesc);
		t->AddControl(new LuaScriptPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (componentDesc->GetType() == ComponentType::COLLISION_SHAPE)
	{
		auto t = new BaseComponentPanelImpl(componentsList, componentDesc);
		t->AddControl(new CollisionShapePanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}
	else if (componentDesc->GetType() == ComponentType::CHARACTER_CONTROLLER)
	{
		auto t = new BaseComponentPanelImpl(componentsList, componentDesc);
		t->AddControl(new CharacterControllerPanelImpl(t));

		componentsSizer->Add(t, 0, wxALL | wxEXPAND, 1);

		currentComponentPanels.push_back(t);
	}

	componentsList->FitInside();
	componentsList->Layout();
}

void MainFrameImpl::RemoveComponentPanel(ComponentDescriptor *componentDesc)
{
	for (auto i = currentComponentPanels.begin(); i != currentComponentPanels.end(); ++i)
	{
		if ((*i)->GetComponent() == componentDesc)
		{
			(*i)->Destroy();
			currentComponentPanels.erase(i);
			return;
		}
	}
}

void MainFrameImpl::UpdateInpsectorPane()
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{					
		componentsList->Freeze();
		componentsList->DestroyChildren();
		currentComponentPanels.clear();

		componentsSizer->Add(new NodePropertiesPanelImpl(componentsList), 0, wxALL | wxEXPAND, 1);

		auto components = nodeDesc->GetComponents();
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

void MainFrameImpl::ComponentAdded(ComponentDescriptor *componentDesc)
{
	if (editorState->GetSelectedNode())
	{
		componentsList->Freeze();
		AddComponentPanel(componentDesc);
		componentsList->Thaw();
	}
}

void MainFrameImpl::ComponentRemoved(ComponentDescriptor *componentDesc)
{
	if (editorState->GetSelectedNode())
	{
		componentsList->Freeze();
		RemoveComponentPanel(componentDesc);
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

void MainFrameImpl::SetAllPanelsVisibility(bool visible)
{
	auto &canvasPane = m_mgr.GetPane("canvasPane");
	if (visible)
	{
		canvasPane.Show();
	}
	else
	{
		canvasPane.Hide();
	}

	auto &sceneHierarchyPane = m_mgr.GetPane("sceneHierarchyPane");
	if (visible)
	{
		sceneHierarchyPane.Show();
	}
	else
	{
		sceneHierarchyPane.Hide();
	}

	auto &inspectorPane = m_mgr.GetPane("inspectorPane");
	if (visible)
	{
		inspectorPane.Show();
	}
	else
	{
		inspectorPane.Hide();
	}	

	m_mgr.Update();
}
