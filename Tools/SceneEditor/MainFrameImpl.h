#pragma once

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include "mainframe.h"
#include "FireCube.h"
#include "Descriptors/NodeDescriptor.h"

class MyApp;
class SceneSettings;
class BaseComponentPanelImpl;
class Command;
class ComponentDescriptor;

/** Implementing MainFrame */
class MainFrameImpl : public MainFrame, public FireCube::Object
{
	FIRECUBE_OBJECT(MainFrameImpl)
	protected:
		// Handlers for MainFrame events.		
		virtual void UndoClicked(wxCommandEvent& event);
		virtual void RedoClicked(wxCommandEvent& event);
		virtual void SaveClicked(wxCommandEvent& event);
		virtual void SaveAsClicked(wxCommandEvent& event);
		virtual void OpenClicked(wxCommandEvent& event);
		virtual void NewClicked(wxCommandEvent& event);		
		virtual void AddMeshClicked(wxCommandEvent& event);		
		virtual void SceneTreeSelectionChanged(wxTreeEvent& event);
		virtual void SceneTreeEndLabelEdit(wxTreeEvent& event);
		virtual void ViewSceneHierarchyClicked(wxCommandEvent& event);
		virtual void ViewInspectorClicked(wxCommandEvent& event);
		virtual void SceneTreeBeginDrag(wxTreeEvent& event);
		virtual void SceneTreeEndDrag(wxTreeEvent& event);
		virtual void PaneClose(wxAuiManagerEvent& event);		
		virtual void AddStaticModelClicked(wxCommandEvent& event);
		virtual void AddLightClicked(wxCommandEvent& event);
		virtual void AddLuaScriptClicked(wxCommandEvent& event);
		virtual void AddCollisionShapeClicked(wxCommandEvent& event);
		virtual void AddCharacterControllerClicked(wxCommandEvent& event);
		virtual void AddBoxClicked(wxCommandEvent& event);
		virtual void AddNodeClicked(wxCommandEvent& event);
		virtual void SceneTreeKeyUp(wxKeyEvent& event);
		void ComponentAdded(ComponentDescriptor *componentDesc);
		void ComponentRemoved(ComponentDescriptor *componentDesc);
		void AddComponentPanel(ComponentDescriptor *componentDesc);
		void RemoveComponentPanel(ComponentDescriptor *componentDesc);
		void SelectedNodeChanged(NodeDescriptor *nodeDesc);
		void NodeAdded(NodeDescriptor *nodeDesc);
		void NodeRemoved(NodeDescriptor *nodeDesc);
		void NodeRenamed(NodeDescriptor *nodeDesc);
		void UpdateInpsectorPane();
		void UpdateNode(NodeDescriptor *nodeDesc);
		void UpdateUndoRedoMenu(Command *command);		
		void SetAllPanelsVisibility(bool visible);
		void LoadSettingsFile();
		void WriteSettingsFile();
		void RecentFileClicked(wxCommandEvent& event);		
		virtual void OnClose(wxCloseEvent& event);
		void OpenSceneFile(const std::string &filename);
		virtual void OnExitClicked(wxCommandEvent& event);
		void Reset();
		void NewSceneCreated();

		MyApp *theApp;
		FireCube::Engine *engine;
		FireCube::Scene *scene;
		FireCube::Node *root;
		NodeDescriptor rootDesc;
		EditorState *editorState;
		SceneSettings *sceneSettings;

		std::map<NodeDescriptor *, wxTreeItemId> nodeToTreeItem;
		std::map<wxTreeItemId, NodeDescriptor *> treeItemToNode;
		
		wxTreeItemId dragItem;		
		std::vector<BaseComponentPanelImpl *> currentComponentPanels;
		std::vector<std::string> recentSceneFiles;
		std::vector<wxMenuItem *> recentSceneFilesMenuItems;
	public:
		/** Constructor */
		MainFrameImpl( wxWindow* parent );	

		void SetScene(FireCube::Scene *scene);
	
};
