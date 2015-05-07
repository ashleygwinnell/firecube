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
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_button1, 0, 0, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_button2, 0, 0, 5 );
	
	m_button3 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_button3, 0, 0, 5 );
	
	
	fgSizer1->Add( bSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	menuBar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* openMenuItem;
	openMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( openMenuItem );
	
	wxMenuItem* saveMenuItem;
	saveMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( saveMenuItem );
	
	menuBar->Append( fileMenu, wxT("File") ); 
	
	editMenu = new wxMenu();
	wxMenuItem* undoMenuItem;
	undoMenuItem = new wxMenuItem( editMenu, wxID_ANY, wxString( wxT("Undo") ) + wxT('\t') + wxT("CTRL-Z"), wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( undoMenuItem );
	
	wxMenuItem* redoMenuItem;
	redoMenuItem = new wxMenuItem( editMenu, wxID_ANY, wxString( wxT("Redo") ) + wxT('\t') + wxT("CTRL-Y"), wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( redoMenuItem );
	
	menuBar->Append( editMenu, wxT("Edit") ); 
	
	this->SetMenuBar( menuBar );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::MyButtonClicked ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::LoadMeshClicked ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::SetBasePathClicked ), NULL, this );
	this->Connect( openMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Connect( saveMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Connect( undoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Connect( redoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::MyButtonClicked ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::LoadMeshClicked ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::SetBasePathClicked ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
	
}
