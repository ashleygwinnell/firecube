///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
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
	wxMenuItem* newMenuItem;
	newMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( newMenuItem );
	
	wxMenuItem* openMenuItem;
	openMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( openMenuItem );
	
	wxMenuItem* saveMenuItem;
	saveMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( saveMenuItem );
	
	wxMenuItem* saveAsMenuItem;
	saveAsMenuItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( wxT("Save As") ) + wxT('\t') + wxT("Ctrl+Shift+S"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( saveAsMenuItem );
	
	fileMenu->AppendSeparator();
	
	wxMenuItem* exitMenuItem;
	exitMenuItem = new wxMenuItem( fileMenu, wxID_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("Ctrl+X"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( exitMenuItem );
	
	menuBar->Append( fileMenu, wxT("File") ); 
	
	editMenu = new wxMenu();
	undoMenuItem = new wxMenuItem( editMenu, wxID_ANY, wxString( wxT("Undo") ) + wxT('\t') + wxT("Ctrl+Z"), wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( undoMenuItem );
	undoMenuItem->Enable( false );
	
	redoMenuItem = new wxMenuItem( editMenu, wxID_ANY, wxString( wxT("Redo") ) + wxT('\t') + wxT("Ctrl+Y"), wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( redoMenuItem );
	redoMenuItem->Enable( false );
	
	menuBar->Append( editMenu, wxT("Edit") ); 
	
	addMenu = new wxMenu();
	wxMenuItem* addNodeMenuItem;
	addNodeMenuItem = new wxMenuItem( addMenu, wxID_ANY, wxString( wxT("Node") ) , wxEmptyString, wxITEM_NORMAL );
	addMenu->Append( addNodeMenuItem );
	
	wxMenuItem* addMeshMenuItem;
	addMeshMenuItem = new wxMenuItem( addMenu, wxID_ANY, wxString( wxT("Mesh") ) , wxEmptyString, wxITEM_NORMAL );
	addMenu->Append( addMeshMenuItem );
	
	m_menu1 = new wxMenu();
	wxMenuItem* m_menu1Item = new wxMenuItem( addMenu, wxID_ANY, wxT("Component"), wxEmptyString, wxITEM_NORMAL, m_menu1 );
	wxMenuItem* addStaticModelMenuItem;
	addStaticModelMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("StaticModel") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addStaticModelMenuItem );
	
	wxMenuItem* addLightMenuItem;
	addLightMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Light") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addLightMenuItem );
	
	wxMenuItem* addLuaScriptMenuItem;
	addLuaScriptMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("LuaScript") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addLuaScriptMenuItem );
	
	wxMenuItem* addCollisionShapeMenuItem;
	addCollisionShapeMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("CollisionShape") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addCollisionShapeMenuItem );
	
	wxMenuItem* addCharacterControllerMenuItem;
	addCharacterControllerMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("CharacterController") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addCharacterControllerMenuItem );
	
	wxMenuItem* addBoxMenuItem;
	addBoxMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Box") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addBoxMenuItem );
	
	wxMenuItem* addRigidBodyMenuItem;
	addRigidBodyMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Rigid Body") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addRigidBodyMenuItem );
	
	addMenu->Append( m_menu1Item );
	
	menuBar->Append( addMenu, wxT("Add") ); 
	
	viewMenu = new wxMenu();
	viewSceneHierarchyMenuItem = new wxMenuItem( viewMenu, wxID_ANY, wxString( wxT("Scene Hierarchy") ) , wxEmptyString, wxITEM_CHECK );
	viewMenu->Append( viewSceneHierarchyMenuItem );
	viewSceneHierarchyMenuItem->Check( true );
	
	viewInspectorMenuItem = new wxMenuItem( viewMenu, wxID_ANY, wxString( wxT("Inspector") ) , wxEmptyString, wxITEM_CHECK );
	viewMenu->Append( viewInspectorMenuItem );
	viewInspectorMenuItem->Check( true );
	
	viewMaterialEditorMenuItem = new wxMenuItem( viewMenu, wxID_ANY, wxString( wxT("Material Editor") ) , wxEmptyString, wxITEM_CHECK );
	viewMenu->Append( viewMaterialEditorMenuItem );
	
	menuBar->Append( viewMenu, wxT("View") ); 
	
	this->SetMenuBar( menuBar );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel1, wxAuiPaneInfo() .Name( wxT("sceneHierarchyPane") ).Left() .Caption( wxT("Scene") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ) );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	sceneTreeCtrl = new wxTreeCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_NO_LINES|wxTR_TWIST_BUTTONS );
	sceneTreeCtrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	bSizer2->Add( sceneTreeCtrl, 1, wxALL|wxEXPAND, 1 );
	
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel2, wxAuiPaneInfo() .Name( wxT("canvasPane") ).Left() .CaptionVisible( false ).CloseButton( false ).PinButton( true ).Float().FloatingPosition( wxPoint( 628,556 ) ).Resizable().FloatingSize( wxSize( 42,59 ) ).CentrePane() );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	glCanvas = new GLCanvas(m_panel2, wxID_ANY,wxDefaultPosition,wxSize(1,1));
	bSizer11->Add( glCanvas, 1, wxEXPAND, 1 );
	
	
	m_panel2->SetSizer( bSizer11 );
	m_panel2->Layout();
	bSizer11->Fit( m_panel2 );
	inspectorPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 400,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( inspectorPanel, wxAuiPaneInfo() .Name( wxT("inspectorPane") ).Right() .Caption( wxT("Inspector") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 42,59 ) ).Layer( 1 ) );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
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
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Connect( newMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::NewClicked ) );
	this->Connect( openMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Connect( saveMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Connect( saveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveAsClicked ) );
	this->Connect( exitMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExitClicked ) );
	this->Connect( undoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Connect( redoMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
	this->Connect( addNodeMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddNodeClicked ) );
	this->Connect( addMeshMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddMeshClicked ) );
	this->Connect( addStaticModelMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddStaticModelClicked ) );
	this->Connect( addLightMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddLightClicked ) );
	this->Connect( addLuaScriptMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddLuaScriptClicked ) );
	this->Connect( addCollisionShapeMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddCollisionShapeClicked ) );
	this->Connect( addCharacterControllerMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddCharacterControllerClicked ) );
	this->Connect( addBoxMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddBoxClicked ) );
	this->Connect( addRigidBodyMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddRigidBodyClicked ) );
	this->Connect( viewSceneHierarchyMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewSceneHierarchyClicked ) );
	this->Connect( viewInspectorMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewInspectorClicked ) );
	this->Connect( viewMaterialEditorMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewMaterialEditorClicked ) );
	sceneTreeCtrl->Connect( wxEVT_KEY_UP, wxKeyEventHandler( MainFrame::SceneTreeKeyUp ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler( MainFrame::SceneTreeBeginDrag ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( MainFrame::SceneTreeEndDrag ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( MainFrame::SceneTreeEndLabelEdit ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::SceneTreeSelectionChanged ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler( MainFrame::PaneClose ) );
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::NewClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OpenClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::SaveAsClicked ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnExitClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::UndoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::RedoClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddNodeClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddMeshClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddStaticModelClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddLightClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddLuaScriptClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddCollisionShapeClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddCharacterControllerClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddBoxClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddRigidBodyClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewSceneHierarchyClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewInspectorClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewMaterialEditorClicked ) );
	sceneTreeCtrl->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( MainFrame::SceneTreeKeyUp ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler( MainFrame::SceneTreeBeginDrag ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( MainFrame::SceneTreeEndDrag ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( MainFrame::SceneTreeEndLabelEdit ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::SceneTreeSelectionChanged ), NULL, this );
	
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
	
	// Connect Events
	removeComponent->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseComponentPanel::RemoveComponentClicked ), NULL, this );
}

BaseComponentPanel::~BaseComponentPanel()
{
	// Disconnect Events
	removeComponent->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseComponentPanel::RemoveComponentClicked ), NULL, this );
	
}

StaticModelPanel::StaticModelPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer8->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	meshFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	bSizer8->Add( meshFilePicker, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer8, 1, wxEXPAND, 5 );
	
	castShadowCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Cast Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( castShadowCheckBox, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("Light Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	bSizer26->Add( m_staticText42, 0, wxALIGN_CENTER|wxALL, 5 );
	
	lightMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	lightMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &lightMaskText ) );
	
	bSizer26->Add( lightMaskTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer26, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer261;
	bSizer261 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText421 = new wxStaticText( this, wxID_ANY, wxT("Collision Query Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText421->Wrap( -1 );
	bSizer261->Add( m_staticText421, 0, wxALIGN_CENTER|wxALL, 5 );
	
	collisionQueryMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	collisionQueryMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &collisionQueryMaskText ) );
	
	bSizer261->Add( collisionQueryMaskTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer261, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer24 );
	this->Layout();
	bSizer24->Fit( this );
	
	// Connect Events
	meshFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( StaticModelPanel::FileChanged ), NULL, this );
	castShadowCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( StaticModelPanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( StaticModelPanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( StaticModelPanel::CollisionQueryMaskChanged ), NULL, this );
}

StaticModelPanel::~StaticModelPanel()
{
	// Disconnect Events
	meshFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( StaticModelPanel::FileChanged ), NULL, this );
	castShadowCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( StaticModelPanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( StaticModelPanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( StaticModelPanel::CollisionQueryMaskChanged ), NULL, this );
	
}

BoxPanel::BoxPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer8->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	widthTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	widthTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &widthText ) );
	
	bSizer8->Add( widthTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText21;
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer81->Add( m_staticText21, 0, wxALIGN_CENTER|wxALL, 5 );
	
	heightTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	heightTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &heightText ) );
	
	bSizer81->Add( heightTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer81, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer811;
	bSizer811 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText211;
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT("Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	bSizer811->Add( m_staticText211, 0, wxALIGN_CENTER|wxALL, 5 );
	
	depthTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	depthTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &depthText ) );
	
	bSizer811->Add( depthTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer811, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, wxT("Material"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	bSizer39->Add( m_staticText48, 0, wxALIGN_CENTER|wxALL, 5 );
	
	materialFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.xml"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	bSizer39->Add( materialFilePicker, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer39, 1, wxEXPAND, 5 );
	
	castShadowCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Cast Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( castShadowCheckBox, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("Light Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	bSizer26->Add( m_staticText42, 0, wxALIGN_CENTER|wxALL, 5 );
	
	lightMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	lightMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &lightMaskText ) );
	
	bSizer26->Add( lightMaskTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer26, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer261;
	bSizer261 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText421 = new wxStaticText( this, wxID_ANY, wxT("Collision Query Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText421->Wrap( -1 );
	bSizer261->Add( m_staticText421, 0, wxALIGN_CENTER|wxALL, 5 );
	
	collisionQueryMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	collisionQueryMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &collisionQueryMaskText ) );
	
	bSizer261->Add( collisionQueryMaskTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer261, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer24 );
	this->Layout();
	bSizer24->Fit( this );
	
	// Connect Events
	widthTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::WidthChanged ), NULL, this );
	heightTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::HeightChanged ), NULL, this );
	depthTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::DepthChanged ), NULL, this );
	materialFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BoxPanel::MaterialFileChanged ), NULL, this );
	castShadowCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BoxPanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::CollisionQueryMaskChanged ), NULL, this );
}

BoxPanel::~BoxPanel()
{
	// Disconnect Events
	widthTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::WidthChanged ), NULL, this );
	heightTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::HeightChanged ), NULL, this );
	depthTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::DepthChanged ), NULL, this );
	materialFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BoxPanel::MaterialFileChanged ), NULL, this );
	castShadowCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BoxPanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( BoxPanel::CollisionQueryMaskChanged ), NULL, this );
	
}

RigidBodyPanel::RigidBodyPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Mass"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer8->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	massTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	massTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &massText ) );
	
	bSizer8->Add( massTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer24->Add( bSizer8, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer24 );
	this->Layout();
	bSizer24->Fit( this );
	
	// Connect Events
	massTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( RigidBodyPanel::MassChanged ), NULL, this );
}

RigidBodyPanel::~RigidBodyPanel()
{
	// Disconnect Events
	massTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( RigidBodyPanel::MassChanged ), NULL, this );
	
}

NodePropertiesPanel::NodePropertiesPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
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
	
	
	bSizer22->Add( fgSizer2, 1, wxEXPAND, 0 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer22->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer22 );
	this->Layout();
	
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

LightPanel::LightPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer13->Add( m_staticText16, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString lightTypeChoiceChoices[] = { wxT("Directional"), wxT("Point"), wxT("Spot") };
	int lightTypeChoiceNChoices = sizeof( lightTypeChoiceChoices ) / sizeof( wxString );
	lightTypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, lightTypeChoiceNChoices, lightTypeChoiceChoices, 0 );
	lightTypeChoice->SetSelection( 0 );
	bSizer13->Add( lightTypeChoice, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer13, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer14->Add( m_staticText17, 0, wxALIGN_CENTER|wxALL, 5 );
	
	lightColorPicker = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer14->Add( lightColorPicker, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer14, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText41 = new wxStaticText( this, wxID_ANY, wxT("Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	bSizer25->Add( m_staticText41, 0, wxALIGN_CENTER|wxALL, 5 );
	
	maskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	maskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &maskText ) );
	
	bSizer25->Add( maskTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer25, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	castShadowCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Cast Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( castShadowCheckBox, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer15, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("Intensity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer21->Add( m_staticText19, 0, wxALIGN_CENTER|wxALL, 5 );
	
	shadowIntensityTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	shadowIntensityTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &shadowIntensityText ) );
	
	bSizer21->Add( shadowIntensityTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer2->Add( bSizer21, 1, wxEXPAND, 5 );
	
	rangePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText21 = new wxStaticText( rangePanel, wxID_ANY, wxT("Range"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText21->Wrap( -1 );
	bSizer16->Add( m_staticText21, 0, wxALIGN_CENTER|wxALL, 5 );
	
	rangeTextCtrl = new wxTextCtrl( rangePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	rangeTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &rangeText ) );
	
	bSizer16->Add( rangeTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	rangePanel->SetSizer( bSizer16 );
	rangePanel->Layout();
	bSizer16->Fit( rangePanel );
	fgSizer2->Add( rangePanel, 1, wxEXPAND, 5 );
	
	spotCutoffPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText211 = new wxStaticText( spotCutoffPanel, wxID_ANY, wxT("Spot Cutoff"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText211->Wrap( -1 );
	bSizer161->Add( m_staticText211, 0, wxALIGN_CENTER|wxALL, 5 );
	
	spotCutoffTextCtrl = new wxTextCtrl( spotCutoffPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	spotCutoffTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &spotCutoffText ) );
	
	bSizer161->Add( spotCutoffTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	spotCutoffPanel->SetSizer( bSizer161 );
	spotCutoffPanel->Layout();
	bSizer161->Fit( spotCutoffPanel );
	fgSizer2->Add( spotCutoffPanel, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	fgSizer2->Fit( this );
	
	// Connect Events
	lightTypeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( LightPanel::LightTypeChanged ), NULL, this );
	lightColorPicker->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( LightPanel::LightColorChanged ), NULL, this );
	maskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::MaskChanged ), NULL, this );
	castShadowCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LightPanel::CastShadowChanged ), NULL, this );
	shadowIntensityTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::ShadowIntensityChanged ), NULL, this );
	rangeTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::RangeChanged ), NULL, this );
	spotCutoffTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::SpotCutoffChanged ), NULL, this );
}

LightPanel::~LightPanel()
{
	// Disconnect Events
	lightTypeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( LightPanel::LightTypeChanged ), NULL, this );
	lightColorPicker->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( LightPanel::LightColorChanged ), NULL, this );
	maskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::MaskChanged ), NULL, this );
	castShadowCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LightPanel::CastShadowChanged ), NULL, this );
	shadowIntensityTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::ShadowIntensityChanged ), NULL, this );
	rangeTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::RangeChanged ), NULL, this );
	spotCutoffTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LightPanel::SpotCutoffChanged ), NULL, this );
	
}

LuaScriptPanel::LuaScriptPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Script"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer8->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	scriptFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	bSizer8->Add( scriptFilePicker, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer20->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText21;
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Object"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer81->Add( m_staticText21, 0, wxALIGN_CENTER|wxALL, 5 );
	
	objectNameTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer81->Add( objectNameTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer20->Add( bSizer81, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxHORIZONTAL );
	
	addProperty = new wxButton( this, wxID_ANY, wxT("Add Property"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( addProperty, 0, wxALIGN_CENTER|wxALL, 5 );
	
	removeProperty = new wxButton( this, wxID_ANY, wxT("Remove Property"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( removeProperty, 0, wxALIGN_CENTER|wxALL, 5 );
	
	renameProperty = new wxButton( this, wxID_ANY, wxT("Rename Property"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( renameProperty, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer20->Add( bSizer30, 1, wxEXPAND, 5 );
	
	propertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), wxPG_DEFAULT_STYLE|wxPG_HIDE_MARGIN);
	bSizer20->Add( propertyGrid, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer20 );
	this->Layout();
	bSizer20->Fit( this );
	
	// Connect Events
	scriptFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LuaScriptPanel::FileChanged ), NULL, this );
	objectNameTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LuaScriptPanel::ObjectNameChanged ), NULL, this );
	addProperty->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LuaScriptPanel::AddPropertyClicked ), NULL, this );
	removeProperty->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LuaScriptPanel::RemovePropertyClicked ), NULL, this );
	renameProperty->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LuaScriptPanel::RenamePropertyClicked ), NULL, this );
	propertyGrid->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( LuaScriptPanel::PropertyGridChanged ), NULL, this );
}

LuaScriptPanel::~LuaScriptPanel()
{
	// Disconnect Events
	scriptFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( LuaScriptPanel::FileChanged ), NULL, this );
	objectNameTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LuaScriptPanel::ObjectNameChanged ), NULL, this );
	addProperty->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LuaScriptPanel::AddPropertyClicked ), NULL, this );
	removeProperty->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LuaScriptPanel::RemovePropertyClicked ), NULL, this );
	renameProperty->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LuaScriptPanel::RenamePropertyClicked ), NULL, this );
	propertyGrid->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( LuaScriptPanel::PropertyGridChanged ), NULL, this );
	
}

CollisionShapePanel::CollisionShapePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer3->Add( m_staticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString shapeTypeChoiceChoices[] = { wxT("Box"), wxT("Plane"), wxT("Mesh"), wxT("Sphere") };
	int shapeTypeChoiceNChoices = sizeof( shapeTypeChoiceChoices ) / sizeof( wxString );
	shapeTypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, shapeTypeChoiceNChoices, shapeTypeChoiceChoices, 0 );
	shapeTypeChoice->SetSelection( 0 );
	fgSizer3->Add( shapeTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	triggerCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Trigger"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( triggerCheckBox, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	planeStaticText = new wxStaticText( this, wxID_ANY, wxT("Plane"), wxDefaultPosition, wxDefaultSize, 0 );
	planeStaticText->Wrap( -1 );
	fgSizer3->Add( planeStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	planePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText27 = new wxStaticText( planePanel, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer4->Add( m_staticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	planeXTextCtrl = new wxTextCtrl( planePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	planeXTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &planeXText ) );
	
	fgSizer4->Add( planeXTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText28 = new wxStaticText( planePanel, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer4->Add( m_staticText28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	planeYTextCtrl = new wxTextCtrl( planePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	planeYTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &planeYText ) );
	
	fgSizer4->Add( planeYTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText29 = new wxStaticText( planePanel, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer4->Add( m_staticText29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	planeZTextCtrl = new wxTextCtrl( planePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	planeZTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &planeZText ) );
	
	fgSizer4->Add( planeZTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText30 = new wxStaticText( planePanel, wxID_ANY, wxT("W"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer4->Add( m_staticText30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	planeWTextCtrl = new wxTextCtrl( planePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	planeWTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &planeWText ) );
	
	fgSizer4->Add( planeWTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	planePanel->SetSizer( fgSizer4 );
	planePanel->Layout();
	fgSizer4->Fit( planePanel );
	fgSizer3->Add( planePanel, 1, wxEXPAND, 5 );
	
	boxStaticText = new wxStaticText( this, wxID_ANY, wxT("Box"), wxDefaultPosition, wxDefaultSize, 0 );
	boxStaticText->Wrap( -1 );
	fgSizer3->Add( boxStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	boxPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText271 = new wxStaticText( boxPanel, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText271->Wrap( -1 );
	fgSizer5->Add( m_staticText271, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxMinXTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	bboxMinXTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxMinXText ) );
	
	fgSizer5->Add( bboxMinXTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText281 = new wxStaticText( boxPanel, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText281->Wrap( -1 );
	fgSizer5->Add( m_staticText281, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxMinYTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	bboxMinYTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxMinYText ) );
	
	fgSizer5->Add( bboxMinYTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText292 = new wxStaticText( boxPanel, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText292->Wrap( -1 );
	fgSizer5->Add( m_staticText292, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxMinZTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	bboxMinZTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxMinZText ) );
	
	fgSizer5->Add( bboxMinZTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText272 = new wxStaticText( boxPanel, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText272->Wrap( -1 );
	fgSizer5->Add( m_staticText272, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxMaxXTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	bboxMaxXTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxMaxXText ) );
	
	fgSizer5->Add( bboxMaxXTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText282 = new wxStaticText( boxPanel, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText282->Wrap( -1 );
	fgSizer5->Add( m_staticText282, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxMaxYTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	bboxMaxYTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxMaxYText ) );
	
	fgSizer5->Add( bboxMaxYTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText293 = new wxStaticText( boxPanel, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText293->Wrap( -1 );
	fgSizer5->Add( m_staticText293, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxMaxZTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	bboxMaxZTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxMaxZText ) );
	
	fgSizer5->Add( bboxMaxZTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	boxPanel->SetSizer( fgSizer5 );
	boxPanel->Layout();
	fgSizer5->Fit( boxPanel );
	fgSizer3->Add( boxPanel, 1, wxEXPAND | wxALL, 5 );
	
	meshStaticText = new wxStaticText( this, wxID_ANY, wxT("Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	meshStaticText->Wrap( -1 );
	fgSizer3->Add( meshStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	meshFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	fgSizer3->Add( meshFilePicker, 0, wxALL|wxEXPAND, 5 );
	
	sphereStaticText = new wxStaticText( this, wxID_ANY, wxT("Sphere"), wxDefaultPosition, wxDefaultSize, 0 );
	sphereStaticText->Wrap( -1 );
	fgSizer3->Add( sphereStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	radiusTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	radiusTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &radiusText ) );
	
	fgSizer3->Add( radiusTextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	fgSizer3->Fit( this );
	
	// Connect Events
	shapeTypeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CollisionShapePanel::ShapeTypeChanged ), NULL, this );
	triggerCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CollisionShapePanel::TriggerChanged ), NULL, this );
	planeXTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneXChanged ), NULL, this );
	planeYTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneYChanged ), NULL, this );
	planeZTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneZChanged ), NULL, this );
	planeWTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneWChanged ), NULL, this );
	bboxMinXTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMinXChanged ), NULL, this );
	bboxMinYTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMinYChanged ), NULL, this );
	bboxMinZTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMinZChanged ), NULL, this );
	bboxMaxXTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMaxXChanged ), NULL, this );
	bboxMaxYTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMaxYChanged ), NULL, this );
	bboxMaxZTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMaxZChanged ), NULL, this );
	meshFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( CollisionShapePanel::MeshFileChanged ), NULL, this );
	radiusTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::RadiusChanged ), NULL, this );
}

CollisionShapePanel::~CollisionShapePanel()
{
	// Disconnect Events
	shapeTypeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CollisionShapePanel::ShapeTypeChanged ), NULL, this );
	triggerCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CollisionShapePanel::TriggerChanged ), NULL, this );
	planeXTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneXChanged ), NULL, this );
	planeYTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneYChanged ), NULL, this );
	planeZTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneZChanged ), NULL, this );
	planeWTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::PlaneWChanged ), NULL, this );
	bboxMinXTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMinXChanged ), NULL, this );
	bboxMinYTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMinYChanged ), NULL, this );
	bboxMinZTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMinZChanged ), NULL, this );
	bboxMaxXTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMaxXChanged ), NULL, this );
	bboxMaxYTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMaxYChanged ), NULL, this );
	bboxMaxZTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::BBoxMaxZChanged ), NULL, this );
	meshFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( CollisionShapePanel::MeshFileChanged ), NULL, this );
	radiusTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CollisionShapePanel::RadiusChanged ), NULL, this );
	
}

CharacterControllerPanel::CharacterControllerPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer28->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	radiusTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	radiusTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &radiusText ) );
	
	bSizer28->Add( radiusTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer22->Add( bSizer28, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	bSizer29->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	heightTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	heightTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &heightText ) );
	
	bSizer29->Add( heightTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer22->Add( bSizer29, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText45 = new wxStaticText( this, wxID_ANY, wxT("Contact Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	bSizer30->Add( m_staticText45, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	contactOffsetTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer30->Add( contactOffsetTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer22->Add( bSizer30, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer22 );
	this->Layout();
	
	// Connect Events
	radiusTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CharacterControllerPanel::RadiusChanged ), NULL, this );
	heightTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CharacterControllerPanel::HeightChanged ), NULL, this );
	contactOffsetTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CharacterControllerPanel::ContactOffsetChanged ), NULL, this );
}

CharacterControllerPanel::~CharacterControllerPanel()
{
	// Disconnect Events
	radiusTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CharacterControllerPanel::RadiusChanged ), NULL, this );
	heightTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CharacterControllerPanel::HeightChanged ), NULL, this );
	contactOffsetTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( CharacterControllerPanel::ContactOffsetChanged ), NULL, this );
	
}

MaterialEditorPanel::MaterialEditorPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxHORIZONTAL );
	
	newButton = new wxButton( this, wxID_ANY, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( newButton, 0, wxALL, 5 );
	
	openButton = new wxButton( this, wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( openButton, 0, wxALL, 5 );
	
	saveButton = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( saveButton, 0, wxALL, 5 );
	
	saveAsButton = new wxButton( this, wxID_ANY, wxT("Save As"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( saveAsButton, 0, wxALL, 5 );
	
	pickMaterialButton = new wxButton( this, wxID_ANY, wxT("Pick"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( pickMaterialButton, 0, wxALL, 5 );
	
	
	bSizer37->Add( bSizer38, 0, wxEXPAND, 5 );
	
	propertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer37->Add( propertyGrid, 1, wxALL|wxEXPAND, 1 );
	
	
	this->SetSizer( bSizer37 );
	this->Layout();
	
	// Connect Events
	newButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::NewButtonClicked ), NULL, this );
	openButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::OpenButtonClicked ), NULL, this );
	saveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::SaveButtonClicked ), NULL, this );
	saveAsButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::SaveAsButtonClicked ), NULL, this );
	pickMaterialButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::PickMaterialButtonClicked ), NULL, this );
	propertyGrid->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( MaterialEditorPanel::PropertyGridChanged ), NULL, this );
}

MaterialEditorPanel::~MaterialEditorPanel()
{
	// Disconnect Events
	newButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::NewButtonClicked ), NULL, this );
	openButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::OpenButtonClicked ), NULL, this );
	saveButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::SaveButtonClicked ), NULL, this );
	saveAsButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::SaveAsButtonClicked ), NULL, this );
	pickMaterialButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::PickMaterialButtonClicked ), NULL, this );
	propertyGrid->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( MaterialEditorPanel::PropertyGridChanged ), NULL, this );
	
}
