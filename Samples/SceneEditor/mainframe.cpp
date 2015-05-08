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
	fgSizer1 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	sceneTreeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer1->Add( sceneTreeCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer1->Add( bSizer1, 1, wxEXPAND, 5 );
	
	glCanvas = new GLCanvas(this, wxID_ANY,wxDefaultPosition,wxSize(1,1));
	fgSizer1->Add( glCanvas, 0, wxEXPAND, 5 );
	
	
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
	
	optionsMenu = new wxMenu();
	wxMenuItem* setBasePathMenuItem;
	setBasePathMenuItem = new wxMenuItem( optionsMenu, wxID_ANY, wxString( wxT("Set Base Dir") ) , wxEmptyString, wxITEM_NORMAL );
	optionsMenu->Append( setBasePathMenuItem );
	
	wxMenuItem* addResourcePathMenuItem;
	addResourcePathMenuItem = new wxMenuItem( optionsMenu, wxID_ANY, wxString( wxT("Add Resource Path") ) , wxEmptyString, wxITEM_NORMAL );
	optionsMenu->Append( addResourcePathMenuItem );
	
	menuBar->Append( optionsMenu, wxT("Options") ); 
	
	addMenu = new wxMenu();
	wxMenuItem* addMeshMenuItem;
	addMeshMenuItem = new wxMenuItem( addMenu, wxID_ANY, wxString( wxT("Mesh") ) , wxEmptyString, wxITEM_NORMAL );
	addMenu->Append( addMeshMenuItem );
	
	menuBar->Append( addMenu, wxT("Add") ); 
	
	this->SetMenuBar( menuBar );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::SceneTreeSelectionChanged ), NULL, this );
	this->Connect( openMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Connect( saveMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Connect( undoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Connect( redoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
	this->Connect( setBasePathMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SetBasePathClicked ) );
	this->Connect( addResourcePathMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddResourcePathClicked ) );
	this->Connect( addMeshMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddMeshClicked ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::SceneTreeSelectionChanged ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SetBasePathClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddResourcePathClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddMeshClicked ) );
	
}
