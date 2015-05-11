#pragma once

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include "mainframe.h"
#include "FireCube.h"

class MyApp;
class SceneSettings;

/** Implementing MainFrame */
class MainFrameImpl : public MainFrame, public FireCube::Object
{
	OBJECT(MainFrameImpl)
	protected:
		// Handlers for MainFrame events.		
		virtual void UndoClicked(wxCommandEvent& event);
		virtual void RedoClicked(wxCommandEvent& event);
		virtual void SaveClicked(wxCommandEvent& event);
		virtual void SaveAsClicked(wxCommandEvent& event);
		virtual void OpenClicked(wxCommandEvent& event);
		virtual void SetBasePathClicked(wxCommandEvent& event);		
		virtual void AddMeshClicked(wxCommandEvent& event);
		virtual void AddResourcePathClicked(wxCommandEvent& event);
		virtual void SceneTreeSelectionChanged(wxTreeEvent& event);
		virtual void SceneTreeEndLabelEdit(wxTreeEvent& event);
		virtual void ViewSceneHierarchyClicked(wxCommandEvent& event);
		virtual void ViewComponentsClicked(wxCommandEvent& event);
		virtual void SceneTreeBeginDrag(wxTreeEvent& event);
		virtual void SceneTreeEndDrag(wxTreeEvent& event);
		virtual void PaneClose(wxAuiManagerEvent& event);
		virtual void TestClicked(wxCommandEvent& event);
		void SelectedNodeChanged(FireCube::Node *node);
		void NodeAdded(FireCube::Node *node);
		void NodeRemoved(FireCube::Node *node);
		void NodeRenamed(FireCube::Node *node);

		MyApp *theApp;
		FireCube::Engine *engine;
		FireCube::Scene *scene;
		FireCube::Node *root;
		EditorState *editorState;
		SceneSettings *sceneSettings;

		std::map<FireCube::Node *, wxTreeItemId> nodeToTreeItem;
		std::map<wxTreeItemId, FireCube::Node *> treeItemToNode;
		
		wxTreeItemId dragItem;
	public:
		/** Constructor */
		MainFrameImpl( wxWindow* parent );	

		void SetScene(FireCube::Scene *scene);
	
};
