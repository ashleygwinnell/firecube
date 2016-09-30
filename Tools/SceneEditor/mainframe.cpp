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
	
	wxMenuItem* addPlaneMenuItem;
	addPlaneMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Plane") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addPlaneMenuItem );
	
	wxMenuItem* addSphereMenuItem;
	addSphereMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Sphere") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addSphereMenuItem );
	
	wxMenuItem* addParticleEmitterMenuItem;
	addParticleEmitterMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("ParticleEmitter") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( addParticleEmitterMenuItem );
	
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
	
	viewAssetBrowserMenuItem = new wxMenuItem( viewMenu, wxID_ANY, wxString( wxT("Asset Browser") ) , wxEmptyString, wxITEM_CHECK );
	viewMenu->Append( viewAssetBrowserMenuItem );
	
	menuBar->Append( viewMenu, wxT("View") ); 
	
	this->SetMenuBar( menuBar );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel1, wxAuiPaneInfo() .Name( wxT("sceneHierarchyPane") ).Left() .Caption( wxT("Scene") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).Layer( 1 ) );
	
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
	
	editorCanvas = new EditorCanvas(m_panel2, wxID_ANY,wxDefaultPosition,wxSize(1,1));
	bSizer11->Add( editorCanvas, 1, wxEXPAND, 1 );
	
	
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
	this->Connect( addPlaneMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddPlaneClicked ) );
	this->Connect( addSphereMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddSphereClicked ) );
	this->Connect( addParticleEmitterMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddParticleEmitterClicked ) );
	this->Connect( viewSceneHierarchyMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewSceneHierarchyClicked ) );
	this->Connect( viewInspectorMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewInspectorClicked ) );
	this->Connect( viewMaterialEditorMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewMaterialEditorClicked ) );
	this->Connect( viewAssetBrowserMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewAssetBrowserClicked ) );
	sceneTreeCtrl->Connect( wxEVT_KEY_UP, wxKeyEventHandler( MainFrame::SceneTreeKeyUp ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler( MainFrame::SceneTreeBeginDrag ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( MainFrame::SceneTreeEndDrag ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( MainFrame::SceneTreeEndLabelEdit ), NULL, this );
	sceneTreeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( MainFrame::SceneTreeItemMenu ), NULL, this );
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
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddPlaneClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddSphereClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::AddParticleEmitterClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewSceneHierarchyClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewInspectorClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewMaterialEditorClicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewAssetBrowserClicked ) );
	sceneTreeCtrl->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( MainFrame::SceneTreeKeyUp ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler( MainFrame::SceneTreeBeginDrag ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( MainFrame::SceneTreeEndDrag ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( MainFrame::SceneTreeEndLabelEdit ), NULL, this );
	sceneTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( MainFrame::SceneTreeItemMenu ), NULL, this );
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
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->AddGrowableCol( 1 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer7->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	meshFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	fgSizer7->Add( meshFilePicker, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText79 = new wxStaticText( this, wxID_ANY, wxT("Cast Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText79->Wrap( -1 );
	fgSizer7->Add( m_staticText79, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	castShadowCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( castShadowCheckBox, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("Light Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer7->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	lightMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	lightMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &lightMaskText ) );
	
	fgSizer7->Add( lightMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText421 = new wxStaticText( this, wxID_ANY, wxT("Collision Query Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText421->Wrap( -1 );
	fgSizer7->Add( m_staticText421, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	collisionQueryMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	collisionQueryMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &collisionQueryMaskText ) );
	
	fgSizer7->Add( collisionQueryMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer7 );
	this->Layout();
	fgSizer7->Fit( this );
	
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
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer8->AddGrowableCol( 1 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText80 = new wxStaticText( this, wxID_ANY, wxT("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText80->Wrap( -1 );
	fgSizer8->Add( m_staticText80, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer66;
	bSizer66 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("W"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer66->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	widthTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	widthTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	widthTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &widthText ) );
	
	bSizer66->Add( widthTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	wxStaticText* m_staticText21;
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("H"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer66->Add( m_staticText21, 0, wxALIGN_CENTER|wxALL, 5 );
	
	heightTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	heightTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	heightTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &heightText ) );
	
	bSizer66->Add( heightTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	wxStaticText* m_staticText211;
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT("D"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	bSizer66->Add( m_staticText211, 0, wxALIGN_CENTER|wxALL, 5 );
	
	depthTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	depthTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	depthTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &depthText ) );
	
	bSizer66->Add( depthTextCtrl, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	fgSizer8->Add( bSizer66, 1, wxEXPAND, 5 );
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, wxT("Material"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer8->Add( m_staticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	materialFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.xml"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	fgSizer8->Add( materialFilePicker, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText81 = new wxStaticText( this, wxID_ANY, wxT("Cast Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	fgSizer8->Add( m_staticText81, 0, wxALL, 5 );
	
	castShadowCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( castShadowCheckBox, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("Light Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer8->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	lightMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	lightMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &lightMaskText ) );
	
	fgSizer8->Add( lightMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText421 = new wxStaticText( this, wxID_ANY, wxT("Collision Query Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText421->Wrap( -1 );
	fgSizer8->Add( m_staticText421, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	collisionQueryMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	collisionQueryMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &collisionQueryMaskText ) );
	
	fgSizer8->Add( collisionQueryMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
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

SpherePanel::SpherePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->AddGrowableCol( 1 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer9->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	radiusTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	radiusTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &radiusText ) );
	
	fgSizer9->Add( radiusTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText21;
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Columns"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer9->Add( m_staticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	columnsTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	columnsTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &columnsText ) );
	
	fgSizer9->Add( columnsTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText211;
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT("Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer9->Add( m_staticText211, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	ringsTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	ringsTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &ringsText ) );
	
	fgSizer9->Add( ringsTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, wxT("Material"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer9->Add( m_staticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	materialFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.xml"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	fgSizer9->Add( materialFilePicker, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText82 = new wxStaticText( this, wxID_ANY, wxT("Cast Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText82->Wrap( -1 );
	fgSizer9->Add( m_staticText82, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	castShadowCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( castShadowCheckBox, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("Light Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer9->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	lightMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	lightMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &lightMaskText ) );
	
	fgSizer9->Add( lightMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText421 = new wxStaticText( this, wxID_ANY, wxT("Collision Query Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText421->Wrap( -1 );
	fgSizer9->Add( m_staticText421, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	collisionQueryMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	collisionQueryMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &collisionQueryMaskText ) );
	
	fgSizer9->Add( collisionQueryMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer9 );
	this->Layout();
	fgSizer9->Fit( this );
	
	// Connect Events
	radiusTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::RadiusChanged ), NULL, this );
	columnsTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::ColumnsChanged ), NULL, this );
	ringsTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::RingsChanged ), NULL, this );
	materialFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( SpherePanel::MaterialFileChanged ), NULL, this );
	castShadowCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SpherePanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::CollisionQueryMaskChanged ), NULL, this );
}

SpherePanel::~SpherePanel()
{
	// Disconnect Events
	radiusTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::RadiusChanged ), NULL, this );
	columnsTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::ColumnsChanged ), NULL, this );
	ringsTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::RingsChanged ), NULL, this );
	materialFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( SpherePanel::MaterialFileChanged ), NULL, this );
	castShadowCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SpherePanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( SpherePanel::CollisionQueryMaskChanged ), NULL, this );
	
}

PlanePanel::PlanePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->AddGrowableCol( 1 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText83 = new wxStaticText( this, wxID_ANY, wxT("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText83->Wrap( -1 );
	fgSizer10->Add( m_staticText83, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText2;
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer69->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	widthTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	widthTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	widthTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &widthText ) );
	
	bSizer69->Add( widthTextCtrl, 1, wxALL, 5 );
	
	wxStaticText* m_staticText211;
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT("Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	bSizer69->Add( m_staticText211, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	depthTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	depthTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	depthTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &depthText ) );
	
	bSizer69->Add( depthTextCtrl, 1, wxALL, 5 );
	
	
	fgSizer10->Add( bSizer69, 1, wxEXPAND, 5 );
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, wxT("Material"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer10->Add( m_staticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	materialFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.xml"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	fgSizer10->Add( materialFilePicker, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText84 = new wxStaticText( this, wxID_ANY, wxT("Cast Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText84->Wrap( -1 );
	fgSizer10->Add( m_staticText84, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	castShadowCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( castShadowCheckBox, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, wxT("Light Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer10->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	lightMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	lightMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &lightMaskText ) );
	
	fgSizer10->Add( lightMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText421 = new wxStaticText( this, wxID_ANY, wxT("Collision Query Mask"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText421->Wrap( -1 );
	fgSizer10->Add( m_staticText421, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	collisionQueryMaskTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	collisionQueryMaskTextCtrl->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &collisionQueryMaskText ) );
	
	fgSizer10->Add( collisionQueryMaskTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer10 );
	this->Layout();
	fgSizer10->Fit( this );
	
	// Connect Events
	widthTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::WidthChanged ), NULL, this );
	depthTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::DepthChanged ), NULL, this );
	materialFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( PlanePanel::MaterialFileChanged ), NULL, this );
	castShadowCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PlanePanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::CollisionQueryMaskChanged ), NULL, this );
}

PlanePanel::~PlanePanel()
{
	// Disconnect Events
	widthTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::WidthChanged ), NULL, this );
	depthTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::DepthChanged ), NULL, this );
	materialFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( PlanePanel::MaterialFileChanged ), NULL, this );
	castShadowCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PlanePanel::CastShadowChanged ), NULL, this );
	lightMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::LightMaskChanged ), NULL, this );
	collisionQueryMaskTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PlanePanel::CollisionQueryMaskChanged ), NULL, this );
	
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
	bSizer8->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
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
	
	m_staticText86 = new wxStaticText( this, wxID_ANY, wxT("Trigger"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText86->Wrap( -1 );
	fgSizer3->Add( m_staticText86, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	triggerCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( triggerCheckBox, 0, wxALL, 5 );
	
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
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->AddGrowableCol( 1 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer11->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	radiusTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), wxTE_PROCESS_ENTER );
	radiusTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &radiusText ) );
	
	fgSizer11->Add( radiusTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer11->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	heightTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	heightTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &heightText ) );
	
	fgSizer11->Add( heightTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText45 = new wxStaticText( this, wxID_ANY, wxT("Contact Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	fgSizer11->Add( m_staticText45, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	contactOffsetTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer11->Add( contactOffsetTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer11 );
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
	bSizer37 = new wxBoxSizer( wxVERTICAL );
	
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
	
	addParameterButton = new wxButton( this, wxID_ANY, wxT("Add Parameter"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( addParameterButton, 0, wxALL, 5 );
	
	
	bSizer37->Add( bSizer38, 0, wxEXPAND, 5 );
	
	m_splitter3 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( MaterialEditorPanel::m_splitter3OnIdle ), NULL, this );
	
	m_panel13 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxVERTICAL );
	
	propertyGrid = new wxPropertyGrid(m_panel13, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer39->Add( propertyGrid, 1, wxALL|wxEXPAND, 1 );
	
	
	m_panel13->SetSizer( bSizer39 );
	m_panel13->Layout();
	bSizer39->Fit( m_panel13 );
	previewPanel = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	previewPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	
	previewPanel->SetSizer( previewPanelSizer );
	previewPanel->Layout();
	previewPanelSizer->Fit( previewPanel );
	m_splitter3->SplitHorizontally( m_panel13, previewPanel, 0 );
	bSizer37->Add( m_splitter3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer37 );
	this->Layout();
	
	// Connect Events
	newButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::NewButtonClicked ), NULL, this );
	openButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::OpenButtonClicked ), NULL, this );
	saveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::SaveButtonClicked ), NULL, this );
	saveAsButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::SaveAsButtonClicked ), NULL, this );
	pickMaterialButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::PickMaterialButtonClicked ), NULL, this );
	addParameterButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::AddParameterButtonClicked ), NULL, this );
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
	addParameterButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MaterialEditorPanel::AddParameterButtonClicked ), NULL, this );
	propertyGrid->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( MaterialEditorPanel::PropertyGridChanged ), NULL, this );
	
}

AssetBrowserPanel::AssetBrowserPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxHORIZONTAL );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( AssetBrowserPanel::m_splitter1OnIdle ), NULL, this );
	
	m_panel8 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer58;
	bSizer58 = new wxBoxSizer( wxVERTICAL );
	
	directoryTreeCtrl = new wxTreeCtrl( m_panel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer58->Add( directoryTreeCtrl, 1, wxALL|wxEXPAND, 0 );
	
	
	m_panel8->SetSizer( bSizer58 );
	m_panel8->Layout();
	bSizer58->Fit( m_panel8 );
	m_panel9 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer59;
	bSizer59 = new wxBoxSizer( wxHORIZONTAL );
	
	splitter2 = new wxSplitterWindow( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	splitter2->SetSashGravity( 1 );
	splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( AssetBrowserPanel::splitter2OnIdle ), NULL, this );
	splitter2->SetMinimumPaneSize( 150 );
	
	m_panel10 = new wxPanel( splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer581;
	bSizer581 = new wxBoxSizer( wxVERTICAL );
	
	fileListCtrl = new wxListCtrl( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON|wxLC_SINGLE_SEL );
	bSizer581->Add( fileListCtrl, 1, wxALL|wxEXPAND, 0 );
	
	
	m_panel10->SetSizer( bSizer581 );
	m_panel10->Layout();
	bSizer581->Fit( m_panel10 );
	previewPanel = new wxPanel( splitter2, wxID_ANY, wxDefaultPosition, wxSize( 300,-1 ), wxTAB_TRAVERSAL );
	previewPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	
	previewPanel->SetSizer( previewPanelSizer );
	previewPanel->Layout();
	splitter2->SplitVertically( m_panel10, previewPanel, 2000 );
	bSizer59->Add( splitter2, 1, wxEXPAND, 5 );
	
	
	m_panel9->SetSizer( bSizer59 );
	m_panel9->Layout();
	bSizer59->Fit( m_panel9 );
	m_splitter1->SplitVertically( m_panel8, m_panel9, 150 );
	bSizer37->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer37 );
	this->Layout();
	
	// Connect Events
	directoryTreeCtrl->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( AssetBrowserPanel::DirectoryTreeSelectionChanged ), NULL, this );
	fileListCtrl->Connect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( AssetBrowserPanel::FileListBeginDrag ), NULL, this );
	fileListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( AssetBrowserPanel::FileListItemActivated ), NULL, this );
	fileListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AssetBrowserPanel::FileListItemSelected ), NULL, this );
	fileListCtrl->Connect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( AssetBrowserPanel::FileListKeyDown ), NULL, this );
	fileListCtrl->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( AssetBrowserPanel::FileListRightUp ), NULL, this );
}

AssetBrowserPanel::~AssetBrowserPanel()
{
	// Disconnect Events
	directoryTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( AssetBrowserPanel::DirectoryTreeSelectionChanged ), NULL, this );
	fileListCtrl->Disconnect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( AssetBrowserPanel::FileListBeginDrag ), NULL, this );
	fileListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( AssetBrowserPanel::FileListItemActivated ), NULL, this );
	fileListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AssetBrowserPanel::FileListItemSelected ), NULL, this );
	fileListCtrl->Disconnect( wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler( AssetBrowserPanel::FileListKeyDown ), NULL, this );
	fileListCtrl->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( AssetBrowserPanel::FileListRightUp ), NULL, this );
	
}

TexturePreviewPanel::TexturePreviewPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );
	
	texturePreviewBitmap = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( texturePreviewBitmap, 1, wxALL|wxEXPAND, 0 );
	
	
	this->SetSizer( bSizer38 );
	this->Layout();
	
	// Connect Events
	texturePreviewBitmap->Connect( wxEVT_SIZE, wxSizeEventHandler( TexturePreviewPanel::TexturePreviewBitmapResize ), NULL, this );
}

TexturePreviewPanel::~TexturePreviewPanel()
{
	// Disconnect Events
	texturePreviewBitmap->Disconnect( wxEVT_SIZE, wxSizeEventHandler( TexturePreviewPanel::TexturePreviewBitmapResize ), NULL, this );
	
}

ScriptEditorPanel::ScriptEditorPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );
	
	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	saveTool = m_toolBar1->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_FILE_SAVE, wxART_OTHER ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolBar1->Realize(); 
	
	bSizer37->Add( m_toolBar1, 0, wxEXPAND, 5 );
	
	sourceText = new wxStyledTextCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	sourceText->SetUseTabs( true );
	sourceText->SetTabWidth( 4 );
	sourceText->SetIndent( 4 );
	sourceText->SetTabIndents( true );
	sourceText->SetBackSpaceUnIndents( true );
	sourceText->SetViewEOL( false );
	sourceText->SetViewWhiteSpace( false );
	sourceText->SetMarginWidth( 2, 0 );
	sourceText->SetIndentationGuides( true );
	sourceText->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	sourceText->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	sourceText->SetMarginWidth( 1, 16);
	sourceText->SetMarginSensitive( 1, true );
	sourceText->SetProperty( wxT("fold"), wxT("1") );
	sourceText->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	sourceText->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	sourceText->SetMarginWidth( 0, sourceText->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	sourceText->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	sourceText->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	sourceText->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	sourceText->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	sourceText->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	sourceText->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	sourceText->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	sourceText->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	sourceText->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	sourceText->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	sourceText->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	sourceText->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	sourceText->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	sourceText->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	sourceText->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	sourceText->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	sourceText->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	bSizer37->Add( sourceText, 1, wxEXPAND | wxALL, 0 );
	
	
	this->SetSizer( bSizer37 );
	this->Layout();
	
	// Connect Events
	this->Connect( saveTool->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptEditorPanel::SaveClicked ) );
	sourceText->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ScriptEditorPanel::OnKeyDown ), NULL, this );
}

ScriptEditorPanel::~ScriptEditorPanel()
{
	// Disconnect Events
	this->Disconnect( saveTool->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ScriptEditorPanel::SaveClicked ) );
	sourceText->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ScriptEditorPanel::OnKeyDown ), NULL, this );
	
}

ParticleEmitterPanel::ParticleEmitterPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer3->Add( m_staticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString shapeTypeChoiceChoices[] = { wxT("Box"), wxT("Sphere") };
	int shapeTypeChoiceNChoices = sizeof( shapeTypeChoiceChoices ) / sizeof( wxString );
	shapeTypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, shapeTypeChoiceNChoices, shapeTypeChoiceChoices, 0 );
	shapeTypeChoice->SetSelection( 0 );
	fgSizer3->Add( shapeTypeChoice, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	boxStaticText = new wxStaticText( this, wxID_ANY, wxT("Box"), wxDefaultPosition, wxDefaultSize, 0 );
	boxStaticText->Wrap( -1 );
	fgSizer3->Add( boxStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	boxPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText271 = new wxStaticText( boxPanel, wxID_ANY, wxT("W"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText271->Wrap( -1 );
	bSizer63->Add( m_staticText271, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxWidthTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	bboxWidthTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	bboxWidthTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxWidthText ) );
	
	bSizer63->Add( bboxWidthTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText281 = new wxStaticText( boxPanel, wxID_ANY, wxT("H"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText281->Wrap( -1 );
	bSizer63->Add( m_staticText281, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxHeightTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	bboxHeightTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	bboxHeightTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxHeightText ) );
	
	bSizer63->Add( bboxHeightTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText292 = new wxStaticText( boxPanel, wxID_ANY, wxT("D"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText292->Wrap( -1 );
	bSizer63->Add( m_staticText292, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bboxDepthTextCtrl = new wxTextCtrl( boxPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	bboxDepthTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	bboxDepthTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &bboxDepthText ) );
	
	bSizer63->Add( bboxDepthTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	boxPanel->SetSizer( bSizer63 );
	boxPanel->Layout();
	bSizer63->Fit( boxPanel );
	fgSizer3->Add( boxPanel, 1, wxEXPAND | wxALL, 5 );
	
	sphereStaticText = new wxStaticText( this, wxID_ANY, wxT("Sphere"), wxDefaultPosition, wxDefaultSize, 0 );
	sphereStaticText->Wrap( -1 );
	fgSizer3->Add( sphereStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	radiusTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	radiusTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &radiusText ) );
	
	fgSizer3->Add( radiusTextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText75 = new wxStaticText( this, wxID_ANY, wxT("Particle Count"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer3->Add( m_staticText75, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	numberOfParticlesTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	numberOfParticlesTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &numberOfParticlesText ) );
	
	fgSizer3->Add( numberOfParticlesTextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText77 = new wxStaticText( this, wxID_ANY, wxT("Emission Rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText77->Wrap( -1 );
	fgSizer3->Add( m_staticText77, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	emissionRateTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	emissionRateTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &emissionRateText ) );
	
	fgSizer3->Add( emissionRateTextCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText771 = new wxStaticText( this, wxID_ANY, wxT("Prewarm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText771->Wrap( -1 );
	fgSizer3->Add( m_staticText771, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	prewarmCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( prewarmCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText81 = new wxStaticText( this, wxID_ANY, wxT("Life Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	fgSizer3->Add( m_staticText81, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText72 = new wxStaticText( this, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	bSizer61->Add( m_staticText72, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	minLifeTimeTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	minLifeTimeTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	minLifeTimeTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &minLifeTimeText ) );
	
	bSizer61->Add( minLifeTimeTextCtrl, 1, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( this, wxID_ANY, wxT("Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	bSizer61->Add( m_staticText73, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	maxLifeTimeTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	maxLifeTimeTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	maxLifeTimeTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &maxLifeTimeText ) );
	
	bSizer61->Add( maxLifeTimeTextCtrl, 1, wxALL, 5 );
	
	
	fgSizer3->Add( bSizer61, 0, wxEXPAND, 5 );
	
	m_staticText74 = new wxStaticText( this, wxID_ANY, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	fgSizer3->Add( m_staticText74, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer62;
	bSizer62 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText751 = new wxStaticText( this, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText751->Wrap( -1 );
	bSizer62->Add( m_staticText751, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	minSpeedTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	minSpeedTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	minSpeedTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &minSpeedText ) );
	
	bSizer62->Add( minSpeedTextCtrl, 1, wxALL, 5 );
	
	m_staticText76 = new wxStaticText( this, wxID_ANY, wxT("Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	bSizer62->Add( m_staticText76, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	maxSpeedTextCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	maxSpeedTextCtrl->SetMinSize( wxSize( 1,-1 ) );
	
	maxSpeedTextCtrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &maxSpeedText ) );
	
	bSizer62->Add( maxSpeedTextCtrl, 1, wxALL, 5 );
	
	
	fgSizer3->Add( bSizer62, 1, wxEXPAND, 5 );
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, wxT("Material"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer3->Add( m_staticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	materialFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.xml"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL );
	fgSizer3->Add( materialFilePicker, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText85 = new wxStaticText( this, wxID_ANY, wxT("Simulation Space"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText85->Wrap( -1 );
	fgSizer3->Add( m_staticText85, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString simulationSpaceChoiceChoices[] = { wxT("Local"), wxT("World") };
	int simulationSpaceChoiceNChoices = sizeof( simulationSpaceChoiceChoices ) / sizeof( wxString );
	simulationSpaceChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, simulationSpaceChoiceNChoices, simulationSpaceChoiceChoices, 0 );
	simulationSpaceChoice->SetSelection( 0 );
	fgSizer3->Add( simulationSpaceChoice, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	fgSizer3->Fit( this );
	
	// Connect Events
	shapeTypeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ParticleEmitterPanel::ShapeTypeChanged ), NULL, this );
	bboxWidthTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::BBoxWidthChanged ), NULL, this );
	bboxHeightTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::BBoxHeightChanged ), NULL, this );
	bboxDepthTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::BBoxDepthChanged ), NULL, this );
	radiusTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::RadiusChanged ), NULL, this );
	numberOfParticlesTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::NumberOfParticlesChanged ), NULL, this );
	emissionRateTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::EmissionRateChanged ), NULL, this );
	prewarmCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ParticleEmitterPanel::PrewarmChanged ), NULL, this );
	minLifeTimeTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MinLifeTimeChanged ), NULL, this );
	maxLifeTimeTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MaxLifeTimeChanged ), NULL, this );
	minSpeedTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MinSpeedChanged ), NULL, this );
	maxSpeedTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MaxSpeedChanged ), NULL, this );
	materialFilePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ParticleEmitterPanel::MaterialFileChanged ), NULL, this );
	simulationSpaceChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ParticleEmitterPanel::SimulationSpaceChanged ), NULL, this );
}

ParticleEmitterPanel::~ParticleEmitterPanel()
{
	// Disconnect Events
	shapeTypeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ParticleEmitterPanel::ShapeTypeChanged ), NULL, this );
	bboxWidthTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::BBoxWidthChanged ), NULL, this );
	bboxHeightTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::BBoxHeightChanged ), NULL, this );
	bboxDepthTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::BBoxDepthChanged ), NULL, this );
	radiusTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::RadiusChanged ), NULL, this );
	numberOfParticlesTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::NumberOfParticlesChanged ), NULL, this );
	emissionRateTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::EmissionRateChanged ), NULL, this );
	prewarmCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ParticleEmitterPanel::PrewarmChanged ), NULL, this );
	minLifeTimeTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MinLifeTimeChanged ), NULL, this );
	maxLifeTimeTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MaxLifeTimeChanged ), NULL, this );
	minSpeedTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MinSpeedChanged ), NULL, this );
	maxSpeedTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEmitterPanel::MaxSpeedChanged ), NULL, this );
	materialFilePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ParticleEmitterPanel::MaterialFileChanged ), NULL, this );
	simulationSpaceChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ParticleEmitterPanel::SimulationSpaceChanged ), NULL, this );
	
}
