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
	fgSizer1 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxScintilla1=new wxScintilla(this,wxID_ANY);
	wxScintilla1->SetLexer(wxSCI_LEX_LUA);
	wxScintilla1->SetUseTabs( true );
	wxScintilla1->SetTabWidth( 4 );
	wxScintilla1->SetIndent( 4 );
	wxScintilla1->SetTabIndents( true );
	wxScintilla1->SetBackSpaceUnIndents( true );
	wxScintilla1->SetViewEOL( false );
	wxScintilla1->SetViewWhiteSpace( false );
	wxScintilla1->SetMarginWidth( 2, 0 );
	wxScintilla1->SetIndentationGuides( true );
	wxScintilla1->SetMarginType( 1, wxSCI_MARGIN_SYMBOL );
	wxScintilla1->SetMarginMask( 1, wxSCI_MASK_FOLDERS );
	wxScintilla1->SetMarginWidth( 1, 16);
	wxScintilla1->SetMarginSensitive( 1, true );
	wxScintilla1->SetProperty( wxT("fold"), wxT("1") );
	wxScintilla1->SetFoldFlags( wxSCI_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSCI_FOLDFLAG_LINEAFTER_CONTRACTED );
	wxScintilla1->SetMarginType( 0, wxSCI_MARGIN_NUMBER );
	wxScintilla1->SetMarginWidth( 0, wxScintilla1->TextWidth( wxSCI_STYLE_LINENUMBER, wxT("_99999") ) );
	wxScintilla1->MarkerDefine( wxSCI_MARKNUM_FOLDER, wxSCI_MARK_BOXPLUS );
	wxScintilla1->MarkerSetBackground( wxSCI_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	wxScintilla1->MarkerSetForeground( wxSCI_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	wxScintilla1->MarkerDefine( wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_BOXMINUS );
	wxScintilla1->MarkerSetBackground( wxSCI_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	wxScintilla1->MarkerSetForeground( wxSCI_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	wxScintilla1->MarkerDefine( wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_EMPTY );
	wxScintilla1->MarkerDefine( wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_BOXPLUS );
	wxScintilla1->MarkerSetBackground( wxSCI_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	wxScintilla1->MarkerSetForeground( wxSCI_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	wxScintilla1->MarkerDefine( wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_BOXMINUS );
	wxScintilla1->MarkerSetBackground( wxSCI_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	wxScintilla1->MarkerSetForeground( wxSCI_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	wxScintilla1->MarkerDefine( wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_EMPTY );
	wxScintilla1->MarkerDefine( wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_EMPTY );
	wxScintilla1->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	wxScintilla1->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	
	wxFont font( 10, wxMODERN, wxNORMAL, wxNORMAL );
	wxScintilla1->StyleSetFont( wxSCI_STYLE_DEFAULT, font );
	//wxScintilla1->StyleClearAll();
	
	fgSizer1->Add( wxScintilla1, 0, wxALL|wxEXPAND, 5 );
	
	richText1 = new wxRichTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	richText1->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Courier New") ) );
	richText1->SetMinSize( wxSize( -1,150 ) );
	
	fgSizer1->Add( richText1, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	menubar1 = new wxMenuBar( 0 );
	menu2 = new wxMenu();
	wxMenuItem* menuItem2;
	menuItem2 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem2 );
	
	wxMenuItem* menuItem3;
	menuItem3 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem3 );
	
	wxMenuItem* menuItem4;
	menuItem4 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem4 );
	
	wxMenuItem* menuItem5;
	menuItem5 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("Save As") ) , wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem5 );
	
	menu2->AppendSeparator();
	
	wxMenuItem* menuItem6;
	menuItem6 = new wxMenuItem( menu2, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	menu2->Append( menuItem6 );
	
	menubar1->Append( menu2, wxT("File") );
	
	menu1 = new wxMenu();
	menuItem1 = new wxMenuItem( menu1, wxID_ANY, wxString( wxT("Execute") ) + wxT('\t') + wxT("Ctrl+F5"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( menuItem1 );
	
	menubar1->Append( menu1, wxT("Build") );
	
	this->SetMenuBar( menubar1 );
	
	
	// Connect Events
	this->Connect( menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem2Clicked ) );
	this->Connect( menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem3Clicked ) );
	this->Connect( menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem4Clicked ) );
	this->Connect( menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem5Clicked ) );
	this->Connect( menuItem6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem6Clicked ) );
	this->Connect( menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem1Clicked ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem2Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem3Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem4Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem5Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem6Clicked ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::MenuItem1Clicked ) );
}
