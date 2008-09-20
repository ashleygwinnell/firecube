///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __mainframe__
#define __mainframe__

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
#include <wx/glcanvas.h>
#include "MyGlCanvas.h"
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		MyGLCanvas *glCanvas;
		wxMenuBar* menuBar;
		wxMenu* menu1;
		wxMenu* menu2;
		wxMenu* menu3;
		wxMenu* menu14;
		wxStatusBar* m_statusBar1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void MenuItem2Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem1Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem3Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem4Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem5Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem6Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem7Clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void MenuItem8Clicked( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 618,596 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

#endif //__mainframe__
