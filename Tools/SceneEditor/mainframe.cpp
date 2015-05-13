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
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	menuBar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* openMenuItem;
	openMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( openMenuItem );
	
	wxMenuItem* saveMenuItem;
	saveMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( saveMenuItem );
	
	wxMenuItem* saveAsMenuItem;
	saveAsMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("MyMenuItem") ) + wxT('\t') + wxT("Ctrl+Shift+S"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( saveAsMenuItem );
	
	menuBar->Append( fileMenu, wxT("File") ); 
	
	editMenu = new wxMenu();
	wxMenuItem* undoMenuItem;
	undoMenuItem = new wxMenuItem( editMenu, wxID_ANY, wxString( wxT("Undo") ) + wxT('\t') + wxT("Ctrl+Z"), wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( undoMenuItem );
	
	wxMenuItem* redoMenuItem;
	redoMenuItem = new wxMenuItem( editMenu, wxID_ANY, wxString( wxT("Redo") ) + wxT('\t') + wxT("Ctrl+Y"), wxEmptyString, wxITEM_NORMAL );
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
	
	viewMenu = new wxMenu();
	viewSceneHierarchyMenuItem = new wxMenuItem( viewMenu, wxID_ANY, wxString( wxT("Scene Hierarchy") ) , wxEmptyString, wxITEM_CHECK );
	viewMenu->Append( viewSceneHierarchyMenuItem );
	viewSceneHierarchyMenuItem->Check( true );
	
	viewInspectorMenuItem = new wxMenuItem( viewMenu, wxID_ANY, wxString( wxT("Inspector") ) , wxEmptyString, wxITEM_CHECK );
	viewMenu->Append( viewInspectorMenuItem );
	viewInspectorMenuItem->Check( true );
	
	menuBar->Append( viewMenu, wxT("View") ); 
	
	this->SetMenuBar( menuBar );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel1, wxAuiPaneInfo() .Name( wxT("sceneHierarchyPane") ).Left() .Caption( wxT("Scene") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).DockFixed( false ) );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	sceneTreeCtrl = new wxTreeCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_NO_LINES|wxTR_TWIST_BUTTONS );
	sceneTreeCtrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	bSizer2->Add( sceneTreeCtrl, 1, wxALL|wxEXPAND, 1 );
	
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel2, wxAuiPaneInfo() .Name( wxT("canvasPane") ).Left() .CaptionVisible( false ).CloseButton( false ).PinButton( true ).Float().FloatingPosition( wxPoint( 628,556 ) ).Resizable().FloatingSize( wxSize( 42,59 ) ).DockFixed( false ).CentrePane() );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	glCanvas = new GLCanvas(m_panel2, wxID_ANY,wxDefaultPosition,wxSize(1,1));
	bSizer11->Add( glCanvas, 1, wxEXPAND, 1 );
	
	
	m_panel2->SetSizer( bSizer11 );
	m_panel2->Layout();
	bSizer11->Fit( m_panel2 );
	inspectorPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 400,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( inspectorPanel, wxAuiPaneInfo() .Name( wxT("inspectorPane") ).Right() .Caption( wxT("Inspector") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 42,59 ) ).DockFixed( false ).Layer( 1 ) );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_button1 = new wxButton( inspectorPanel, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL, 5 );
	
	componentsList = new wxScrolledWindow( inspectorPanel, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxHSCROLL|wxVSCROLL );
	componentsList->SetScrollRate( 5, 5 );
	componentsSizer = new wxBoxSizer( wxVERTICAL );
	
	
	componentsList->SetSizer( componentsSizer );
	componentsList->Layout();
	componentsSizer->Fit( componentsList );
	bSizer3->Add( componentsList, 1, wxEXPAND | wxALL, 5 );
	
	
	inspectorPanel->SetSizer( bSizer3 );
	inspectorPanel->Layout();
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler( MainFrame::PaneClose ) );
	this->Connect( openMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Connect( saveMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Connect( saveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveAsClicked ) );
	this->Connect( undoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Connect( redoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
	this->Connect( setBasePathMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SetBasePathClicked ) );
	this->Connect( addResourcePathMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddResourcePathClicked ) );
	this->Connect( addMeshMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddMeshClicked ) );
	this->Connect( viewSceneHierarchyMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewSceneHierarchyClicked ) );
	this->Connect( viewInspectorMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewInspectorClicked ) );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler( MainFrame::SceneTreeBeginDrag ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( MainFrame::SceneTreeEndDrag ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( MainFrame::SceneTreeEndLabelEdit ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::SceneTreeSelectionChanged ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::TestClicked ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler( MainFrame::PaneClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveAsClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SetBasePathClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddResourcePathClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddMeshClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewSceneHierarchyClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewInspectorClicked ) );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler( MainFrame::SceneTreeBeginDrag ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( MainFrame::SceneTreeEndDrag ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( MainFrame::SceneTreeEndLabelEdit ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::SceneTreeSelectionChanged ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::TestClicked ), NULL, this );
	
	m_mgr.UnInit();
	
}

BaseComponentPanel::BaseComponentPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	componentTypeLabel = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	componentTypeLabel->Wrap( -1 );
	bSizer6->Add( componentTypeLabel, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	removeComponent = new wxButton( this, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( removeComponent, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer5->Add( bSizer6, 0, wxEXPAND, 5 );
	
	customComponentsSizer = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer5->Add( customComponentsSizer, 1, wxALL|wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer5->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );
}

BaseComponentPanel::~BaseComponentPanel()
{
}

StaticModelPanel::StaticModelPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer8->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	meshFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer8->Add( meshFilePicker, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	this->SetSizer( bSizer8 );
	this->Layout();
	bSizer8->Fit( this );
	
	// Connect Events
	meshFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( StaticModelPanel::FileChanged ), NULL, this );
}

StaticModelPanel::~StaticModelPanel()
{
	// Disconnect Events
	meshFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( StaticModelPanel::FileChanged ), NULL, this );
	
}

NodePropertiesPanel::NodePropertiesPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 0 );
	fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer2->Add( m_staticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	nameTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer2->Add( nameTextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer2->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer9->Add( m_staticText4, 0, wxALIGN_CENTER|wxALL, 5 );
	
	positionXTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	positionXTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodePositionXText ) );
	
	bSizer9->Add( positionXTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer9->Add( m_staticText5, 0, wxALIGN_CENTER|wxALL, 5 );
	
	positionYTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	positionYTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodePositionYText ) );
	
	bSizer9->Add( positionYTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer9->Add( m_staticText6, 0, wxALIGN_CENTER|wxALL, 5 );
	
	positionZTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	positionZTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodePositionZText ) );
	
	bSizer9->Add( positionZTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer9, 1, wxEXPAND, 5 );
	
	m_staticText31 = new wxStaticText( this, wxID_ANY, wxT("Rotation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer2->Add( m_staticText31, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText41 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	bSizer91->Add( m_staticText41, 0, wxALIGN_CENTER|wxALL, 5 );
	
	rotationXTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	rotationXTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodeRotationXText ) );
	
	bSizer91->Add( rotationXTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	bSizer91->Add( m_staticText51, 0, wxALIGN_CENTER|wxALL, 5 );
	
	rotationYTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	rotationYTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodeRotationYText ) );
	
	bSizer91->Add( rotationYTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText61 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	bSizer91->Add( m_staticText61, 0, wxALIGN_CENTER|wxALL, 5 );
	
	rotationZTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	rotationZTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodeRotationZText ) );
	
	bSizer91->Add( rotationZTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer91, 0, wxEXPAND, 5 );
	
	m_staticText32 = new wxStaticText( this, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer2->Add( m_staticText32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer92;
	bSizer92 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	bSizer92->Add( m_staticText42, 0, wxALIGN_CENTER|wxALL, 5 );
	
	scaleXTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	scaleXTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodeScaleXText ) );
	
	bSizer92->Add( scaleXTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText52 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText52->Wrap( -1 );
	bSizer92->Add( m_staticText52, 0, wxALIGN_CENTER|wxALL, 5 );
	
	scaleYTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	scaleYTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodeScaleYText ) );
	
	bSizer92->Add( scaleYTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText62 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	bSizer92->Add( m_staticText62, 0, wxALIGN_CENTER|wxALL, 5 );
	
	scaleZTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	scaleZTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &nodeScaleZText ) );
	
	bSizer92->Add( scaleZTextCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer92, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	fgSizer2->Fit( this );
	
	// Connect Events
	nameTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::NameChanged ), NULL, this );
	positionXTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::PositionXChanged ), NULL, this );
	positionYTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::PositionYChanged ), NULL, this );
	positionZTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::PositionZChanged ), NULL, this );
	rotationXTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::RotationXChanged ), NULL, this );
	rotationYTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::RotationYChanged ), NULL, this );
	rotationZTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::RotationZChanged ), NULL, this );
	scaleXTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::ScaleXChanged ), NULL, this );
	scaleYTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::ScaleYChanged ), NULL, this );
	scaleZTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::ScaleZChanged ), NULL, this );
}

NodePropertiesPanel::~NodePropertiesPanel()
{
	// Disconnect Events
	nameTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::NameChanged ), NULL, this );
	positionXTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::PositionXChanged ), NULL, this );
	positionYTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::PositionYChanged ), NULL, this );
	positionZTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::PositionZChanged ), NULL, this );
	rotationXTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::RotationXChanged ), NULL, this );
	rotationYTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::RotationYChanged ), NULL, this );
	rotationZTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::RotationZChanged ), NULL, this );
	scaleXTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::ScaleXChanged ), NULL, this );
	scaleYTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::ScaleYChanged ), NULL, this );
	scaleZTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( NodePropertiesPanel::ScaleZChanged ), NULL, this );
	
}
