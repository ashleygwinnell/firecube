///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mainframe.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	glCanvas = new GLCanvas(this, wxID_ANY,wxDefaultPosition,wxSize(1,1));
	fgSizer1->Add( glCanvas, 0, wxEXPAND, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_button1, 0, 0, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	glCanvas->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainFrame::OnLeftDown ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::MyButtonClicked ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	glCanvas->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainFrame::OnLeftDown ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::MyButtonClicked ), NULL, this );
	
}
