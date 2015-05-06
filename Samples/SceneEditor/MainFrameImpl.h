#pragma once

/**
@file
Subclass of MainFrame, which is generated by wxFormBuilder.
*/

#include "mainframe.h"
#include "FireCube.h"

class MyApp;

/** Implementing MainFrame */
class MainFrameImpl : public MainFrame
{
	protected:
		// Handlers for MainFrame events.
		void MyButtonClicked( wxCommandEvent& event );
		void LoadMeshClicked(wxCommandEvent& event);
		virtual void UndoClicked(wxCommandEvent& event);
		virtual void RedoClicked(wxCommandEvent& event);
		virtual void SaveClicked(wxCommandEvent& event);
		virtual void OpenClicked(wxCommandEvent& event);

		MyApp *theApp;
		FireCube::Engine *engine;
		FireCube::Scene *scene;
		FireCube::Node *root;
		EditorState *editorState;
	public:
		/** Constructor */
		MainFrameImpl( wxWindow* parent );	

		void SetScene(FireCube::Scene *scene);
	
};
