///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
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
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	glCanvas = new MyGLCanvas(this, wxID_ANY,wxDefaultPosition,wxSize(2000,2000));
	fgSizer1->Add( glCanvas, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	menuBar = new wxMenuBar( 0 );
	menu1 = new wxMenu();
	wxMenuItem* menuItem2;
	menuItem2 = new wxMenuItem( menu1, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	menu1->Append( menuItem2 );
	
	wxMenuItem* menuItem1;
	menuItem1 = new wxMenuItem( menu1, wxID_ANY, wxString( wxT("E&xit") ) , wxT("Close the application."), wxITEM_NORMAL );
	menu1->Append( menuItem1 );
	
	menuBar->Append( menu1, wxT("&File") );
	
	menu2 = new wxMenu();
	wxMenuItem* menuItem3;
	menuItem3 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("Open vertex shader") ) , wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem3 );
	
	wxMenuItem* menuItem4;
	menuItem4 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("Open pixel shader") ) , wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem4 );
	
	menuBar->Append( menu2, wxT("Shader") );
	
	menu3 = new wxMenu();
	wxMenuItem* menuItem5;
	menuItem5 = new wxMenuItem( menu3, wxID_ANY, wxString( wxT("Change background color") ) , wxEmptyString, wxITEM_NORMAL );
	menu3->Append( menuItem5 );
	
	menu14 = new wxMenu();
	wxMenuItem* menuItem9;
	menuItem9 = new wxMenuItem( menu14, wxID_ANY, wxString( wxT("Normals") ) , wxEmptyString, wxITEM_CHECK );
	menu14->Append( menuItem9 );
	
	menu14->AppendSeparator();
	
	wxMenuItem* menuItem6;
	menuItem6 = new wxMenuItem( menu14, wxID_ANY, wxString( wxT("Points") ) , wxEmptyString, wxITEM_RADIO );
	menu14->Append( menuItem6 );
	
	wxMenuItem* menuItem7;
	menuItem7 = new wxMenuItem( menu14, wxID_ANY, wxString( wxT("Lines") ) , wxEmptyString, wxITEM_RADIO );
	menu14->Append( menuItem7 );
	
	wxMenuItem* menuItem8;
	menuItem8 = new wxMenuItem( menu14, wxID_ANY, wxString( wxT("Solid") ) , wxEmptyString, wxITEM_RADIO );
	menu14->Append( menuItem8 );
	menuItem8->Check( true );
	
	menu3->Append( -1, wxT("Rendering"), menu14 );
	
	menuBar->Append( menu3, wxT("Options") );
	
	this->SetMenuBar( menuBar );
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem2Clicked ) );
	this->Connect( menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem1Clicked ) );
	this->Connect( menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem3Clicked ) );
	this->Connect( menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem4Clicked ) );
	this->Connect( menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem5Clicked ) );
	this->Connect( menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem9Clicked ) );
	this->Connect( menuItem6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem6Clicked ) );
	this->Connect( menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem7Clicked ) );
	this->Connect( menuItem8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem8Clicked ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem2Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem1Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem3Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem4Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem5Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem9Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem6Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem7Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem8Clicked ) );
}
