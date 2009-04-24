///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __mainframe__
#define __mainframe__

#include <wx/wxScintilla.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/richtext/richtextctrl.h>
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
		wxScintilla *wxScintilla1;
		wxMenuBar* menubar1;
		wxMenu* menu2;
		wxMenu* menu1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void MenuItem2Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem3Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem4Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem5Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem6Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem1Clicked( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxRichTextCtrl* richText1;
		wxMenuItem* menuItem1;
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("LuaRunner"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 864,730 ), long style = wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

#endif //__mainframe__
