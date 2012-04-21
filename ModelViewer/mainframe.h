///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
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
#include <queue>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
#include <wx/glcanvas.h>
#include "GlCanvas.h"
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>
#include <wx/clrpicker.h>
#include <wx/button.h>
#include <wx/choicebk.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
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
		GLCanvas *glCanvas;
		wxNotebook* notebook1;
		wxPanel* panel1;
		wxStaticText* staticText1;
		wxTextCtrl* textCtrl1;
		wxStaticText* staticText2;
		wxTextCtrl* textCtrl2;
		wxPanel* panel2;
		wxChoicebook* m_choicebook1;
		wxPanel* panel3;
		wxPanel* panel9;
		wxCheckBox* checkBox2;
		wxCheckBox* checkBox3;
		wxCheckBox* checkBox4;
		wxStaticText* staticText3;
		wxTextCtrl* textCtrl3;
		wxRadioBox* radioBox1;
		wxPanel* panel7;
		wxStaticText* staticText4;
		wxColourPickerCtrl* colourPicker1;
		wxPanel* panel8;
		wxStaticText* staticText6;
		wxTextCtrl* textCtrl4;
		wxStaticText* staticText7;
		wxTextCtrl* textCtrl5;
		wxButton* button1;
		wxPanel* panel4;
		wxButton* button2;
		wxPanel* panel5;
		wxPropertyGrid *propertyGrid1;
		wxPanel* panel10;
		wxTreeCtrl* treeCtrl2;
		wxMenuBar* menuBar;
		wxMenu* menu1;
		wxMenu* menu2;
		wxMenu* menu3;
		wxStatusBar* statusBar1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void CheckBox2Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void CheckBox3Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void CheckBox4Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void TextCtrl3TextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void RadioBox1Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ColourPicker1Changed( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void TextCtrl4TextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void TextCtrl5TextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void Button1Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void Button2Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void MenuItem2Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void MenuItem1Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void MenuItem3Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void MenuItem4Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void MenuItem7Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void MenuItem6Clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void MenuItem5Clicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ModelViewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 918,764 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

#endif //__mainframe__
