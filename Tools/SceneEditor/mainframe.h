///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <queue>
#include <FireCube.h>
#include <wx/glcanvas.h>
#include "GlCanvas.h"
#include <wx/button.h>
#include <wx/scrolwin.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/filepicker.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* menuBar;
		wxMenu* fileMenu;
		wxMenu* editMenu;
		wxMenu* optionsMenu;
		wxMenu* addMenu;
		wxMenu* viewMenu;
		wxMenuItem* viewSceneHierarchyMenuItem;
		wxMenuItem* viewComponentsMenuItem;
		wxPanel* m_panel1;
		wxTreeCtrl* sceneTreeCtrl;
		wxPanel* m_panel2;
		GLCanvas *glCanvas;
		wxPanel* componentsPanel;
		wxButton* m_button1;
		wxScrolledWindow* componentsList;
		wxBoxSizer* componentsSizer;
		
		// Virtual event handlers, overide them in your derived class
		virtual void PaneClose( wxAuiManagerEvent& event ) { event.Skip(); }
		virtual void OpenClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveAsClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void UndoClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void RedoClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SetBasePathClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddResourcePathClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddMeshClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ViewSceneHierarchyClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ViewComponentsClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SceneTreeBeginDrag( wxTreeEvent& event ) { event.Skip(); }
		virtual void SceneTreeEndDrag( wxTreeEvent& event ) { event.Skip(); }
		virtual void SceneTreeEndLabelEdit( wxTreeEvent& event ) { event.Skip(); }
		virtual void SceneTreeSelectionChanged( wxTreeEvent& event ) { event.Skip(); }
		virtual void TestClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Scene Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 876,616 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class BaseComponentPanel
///////////////////////////////////////////////////////////////////////////////
class BaseComponentPanel : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* componentTypeLabel;
		wxBoxSizer* customComponentsSizer;
		wxStaticLine* m_staticline1;
	
	public:
		wxButton* removeComponent;
		
		BaseComponentPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~BaseComponentPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class StaticModelPanel
///////////////////////////////////////////////////////////////////////////////
class StaticModelPanel : public wxPanel 
{
	private:
	
	protected:
		wxFilePickerCtrl* meshFilePicker;
		
		// Virtual event handlers, overide them in your derived class
		virtual void FileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		
	
	public:
		
		StaticModelPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL ); 
		~StaticModelPanel();
	
};

#endif //__MAINFRAME_H__
