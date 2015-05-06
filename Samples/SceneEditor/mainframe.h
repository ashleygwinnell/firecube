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
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <queue>
#include <FireCube.h>
#include <wx/glcanvas.h>
#include "GlCanvas.h"
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		GLCanvas *glCanvas;
		wxButton* m_button1;
		wxButton* m_button2;
		wxMenuBar* menuBar;
		wxMenu* fileMenu;
		wxMenu* editMenu;
		
		// Virtual event handlers, overide them in your derived class
		virtual void MyButtonClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void LoadMeshClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OpenClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void UndoClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void RedoClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Scene Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 876,616 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		
		~MainFrame();
	
};

#endif //__MAINFRAME_H__
