///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
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
	
	glCanvas = new MyGLCanvas(this, wxID_ANY,wxDefaultPosition,wxSize(1,1));
	fgSizer1->Add( glCanvas, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxNB_MULTILINE );
	panel1 = new wxPanel( notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	staticText1 = new wxStaticText( panel1, wxID_ANY, wxT("Vertices:"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText1->Wrap( -1 );
	fgSizer4->Add( staticText1, 0, wxALL, 5 );
	
	textCtrl1 = new wxTextCtrl( panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	textCtrl1->Enable( false );
	
	fgSizer4->Add( textCtrl1, 0, wxALL|wxEXPAND, 1 );
	
	staticText2 = new wxStaticText( panel1, wxID_ANY, wxT("Faces:"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText2->Wrap( -1 );
	fgSizer4->Add( staticText2, 0, wxALL, 5 );
	
	textCtrl2 = new wxTextCtrl( panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	textCtrl2->Enable( false );
	
	fgSizer4->Add( textCtrl2, 0, wxALL|wxEXPAND, 1 );
	
	panel1->SetSizer( fgSizer4 );
	panel1->Layout();
	fgSizer4->Fit( panel1 );
	notebook1->AddPage( panel1, wxT("Info"), false );
	panel2 = new wxPanel( notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_choicebook1 = new wxChoicebook( panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	panel3 = new wxPanel( m_choicebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	panel9 = new wxPanel( panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	checkBox2 = new wxCheckBox( panel9, wxID_ANY, wxT("Normals"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( checkBox2, 0, wxALL, 5 );
	
	checkBox3 = new wxCheckBox( panel9, wxID_ANY, wxT("Cull face"), wxDefaultPosition, wxDefaultSize, 0 );
	checkBox3->SetValue(true); 
	fgSizer8->Add( checkBox3, 0, wxALL, 5 );
	
	checkBox4 = new wxCheckBox( panel9, wxID_ANY, wxT("Tangents"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( checkBox4, 0, wxALL, 5 );
	
	bSizer8->Add( fgSizer8, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer51;
	fgSizer51 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer51->AddGrowableCol( 0 );
	fgSizer51->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer51->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	staticText3 = new wxStaticText( panel9, wxID_ANY, wxT("Normals length:"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText3->Wrap( -1 );
	fgSizer51->Add( staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	textCtrl3 = new wxTextCtrl( panel9, wxID_ANY, wxT("0.1"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer51->Add( textCtrl3, 0, wxALL|wxEXPAND, 5 );
	
	bSizer8->Add( fgSizer51, 0, wxEXPAND, 5 );
	
	panel9->SetSizer( bSizer8 );
	panel9->Layout();
	bSizer8->Fit( panel9 );
	bSizer5->Add( panel9, 0, wxEXPAND | wxALL, 5 );
	
	wxString radioBox1Choices[] = { wxT("Points"), wxT("Lines"), wxT("Solid") };
	int radioBox1NChoices = sizeof( radioBox1Choices ) / sizeof( wxString );
	radioBox1 = new wxRadioBox( panel3, wxID_ANY, wxT("Fill Mode"), wxDefaultPosition, wxDefaultSize, radioBox1NChoices, radioBox1Choices, 1, wxRA_SPECIFY_COLS );
	radioBox1->SetSelection( 2 );
	bSizer5->Add( radioBox1, 0, wxALL|wxEXPAND, 5 );
	
	panel7 = new wxPanel( panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	staticText4 = new wxStaticText( panel7, wxID_ANY, wxT("Background Color:"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText4->Wrap( -1 );
	fgSizer6->Add( staticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	colourPicker1 = new wxColourPickerCtrl( panel7, wxID_ANY, wxColour( 63, 132, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer6->Add( colourPicker1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	panel7->SetSizer( fgSizer6 );
	panel7->Layout();
	fgSizer6->Fit( panel7 );
	bSizer5->Add( panel7, 0, wxALL|wxEXPAND, 5 );
	
	panel8 = new wxPanel( panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer7->AddGrowableCol( 1 );
	fgSizer7->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	staticText6 = new wxStaticText( panel8, wxID_ANY, wxT("Cell size:"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText6->Wrap( -1 );
	fgSizer7->Add( staticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	textCtrl4 = new wxTextCtrl( panel8, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer7->Add( textCtrl4, 0, wxALL|wxEXPAND, 5 );
	
	staticText7 = new wxStaticText( panel8, wxID_ANY, wxT("Number of cells:"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText7->Wrap( -1 );
	fgSizer7->Add( staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	textCtrl5 = new wxTextCtrl( panel8, wxID_ANY, wxT("20"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer7->Add( textCtrl5, 0, wxALL|wxEXPAND, 5 );
	
	panel8->SetSizer( fgSizer7 );
	panel8->Layout();
	fgSizer7->Fit( panel8 );
	bSizer5->Add( panel8, 0, wxEXPAND | wxALL, 5 );
	
	button1 = new wxButton( panel3, wxID_ANY, wxT("Reset camera"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( button1, 0, wxALL, 5 );
	
	panel3->SetSizer( bSizer5 );
	panel3->Layout();
	bSizer5->Fit( panel3 );
	m_choicebook1->AddPage( panel3, wxT("Rendering"), false );
	panel4 = new wxPanel( m_choicebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );
	
	button2 = new wxButton( panel4, wxID_ANY, wxT("Add search path"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer51->Add( button2, 0, wxALL, 5 );
	
	panel4->SetSizer( bSizer51 );
	panel4->Layout();
	bSizer51->Fit( panel4 );
	m_choicebook1->AddPage( panel4, wxT("Environment"), false );
	bSizer3->Add( m_choicebook1, 1, wxEXPAND | wxALL, 5 );
	
	panel2->SetSizer( bSizer3 );
	panel2->Layout();
	bSizer3->Fit( panel2 );
	notebook1->AddPage( panel2, wxT("Settings"), true );
	panel5 = new wxPanel( notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 1, 1, 0, 0 );
	fgSizer5->AddGrowableCol( 0 );
	fgSizer5->AddGrowableRow( 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	propertyGrid1=new wxPropertyGrid(panel5,wxID_ANY);
	fgSizer5->Add( propertyGrid1, 0, wxALL|wxEXPAND, 1 );
	
	panel5->SetSizer( fgSizer5 );
	panel5->Layout();
	fgSizer5->Fit( panel5 );
	notebook1->AddPage( panel5, wxT("Materials"), false );
	
	bSizer2->Add( notebook1, 1, wxEXPAND | wxALL, 5 );
	
	fgSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
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
	
	wxMenuItem* menuItem7;
	menuItem7 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("None") ) , wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem7 );
	
	menuBar->Append( menu2, wxT("Shader") ); 
	
	menu3 = new wxMenu();
	wxMenuItem* menuItem6;
	menuItem6 = new wxMenuItem( menu3, wxID_ANY, wxString( wxT("Sidebar") ) , wxEmptyString, wxITEM_CHECK );
	menu3->Append( menuItem6 );
	menuItem6->Check( true );
	
	wxMenuItem* menuItem5;
	menuItem5 = new wxMenuItem( menu3, wxID_ANY, wxString( wxT("Statusbar") ) , wxEmptyString, wxITEM_CHECK );
	menu3->Append( menuItem5 );
	menuItem5->Check( true );
	
	menuBar->Append( menu3, wxT("View") ); 
	
	this->SetMenuBar( menuBar );
	
	statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	checkBox2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::CheckBox2Clicked ), NULL, this );
	checkBox3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::CheckBox3Clicked ), NULL, this );
	checkBox4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::CheckBox4Clicked ), NULL, this );
	textCtrl3->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::TextCtrl3TextEnter ), NULL, this );
	radioBox1->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( MainFrame::RadioBox1Clicked ), NULL, this );
	colourPicker1->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( MainFrame::ColourPicker1Changed ), NULL, this );
	textCtrl4->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::TextCtrl4TextEnter ), NULL, this );
	textCtrl5->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::TextCtrl5TextEnter ), NULL, this );
	button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::Button1Clicked ), NULL, this );
	button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::Button2Clicked ), NULL, this );
	this->Connect( menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem2Clicked ) );
	this->Connect( menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem1Clicked ) );
	this->Connect( menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem3Clicked ) );
	this->Connect( menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem4Clicked ) );
	this->Connect( menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem7Clicked ) );
	this->Connect( menuItem6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem6Clicked ) );
	this->Connect( menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem5Clicked ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	checkBox2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::CheckBox2Clicked ), NULL, this );
	checkBox3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::CheckBox3Clicked ), NULL, this );
	checkBox4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MainFrame::CheckBox4Clicked ), NULL, this );
	textCtrl3->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::TextCtrl3TextEnter ), NULL, this );
	radioBox1->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( MainFrame::RadioBox1Clicked ), NULL, this );
	colourPicker1->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( MainFrame::ColourPicker1Changed ), NULL, this );
	textCtrl4->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::TextCtrl4TextEnter ), NULL, this );
	textCtrl5->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::TextCtrl5TextEnter ), NULL, this );
	button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::Button1Clicked ), NULL, this );
	button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::Button2Clicked ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem2Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem1Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem3Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem4Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem7Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem6Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem5Clicked ) );
	
}
