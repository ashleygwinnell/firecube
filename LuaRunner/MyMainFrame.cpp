#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "mainframe.h"
#include "MyMainFrame.h"
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include "main.h"

using namespace FireCube;

bool MyMainFrame::running=false;

MyMainFrame::MyMainFrame( wxWindow* parent )
:
MainFrame( parent )
{
	this->Connect(wxScintilla1->GetId(),wxEVT_SCI_MARGINCLICK,wxCommandEventHandler(MyMainFrame::Scintilla1MarginClicked));	
	this->Connect(wxScintilla1->GetId(),wxEVT_SCI_CHARADDED,wxCommandEventHandler(MyMainFrame::Scintilla1CharAdded));	
	this->Connect(wxScintilla1->GetId(),wxEVT_SCI_UPDATEUI,wxCommandEventHandler(MyMainFrame::Scintilla1UpdateUI));	
	this->Connect(wxScintilla1->GetId(),wxEVT_SCI_MODIFIED,wxCommandEventHandler(MyMainFrame::Scintilla1Modified));	
	wxScintilla1->StyleSetForeground (wxSCI_LUA_WORD, wxColour (_T("BLUE")));
	wxScintilla1->StyleSetForeground (wxSCI_LUA_WORD5, wxColour (_T("PURPLE")));	
	wxScintilla1->StyleSetForeground (wxSCI_LUA_WORD6, wxColour (_T("PURPLE")));	
	wxScintilla1->StyleSetForeground (wxSCI_LUA_LITERALSTRING, wxColour (_T("BROWN")));
	wxScintilla1->StyleSetForeground (wxSCI_LUA_STRING, wxColour (_T("BROWN")));	
	wxScintilla1->StyleSetForeground (wxSCI_LUA_NUMBER, wxColour (_T("SEA GREEN")));	
	wxScintilla1->StyleSetForeground (wxSCI_LUA_IDENTIFIER, wxColour (_T("GRAY")));
	wxScintilla1->StyleSetForeground (wxSCI_LUA_COMMENT, wxColour (0,128,0));
	wxScintilla1->StyleSetForeground (wxSCI_LUA_COMMENTDOC, wxColour (0,128,0));
	wxScintilla1->StyleSetForeground (wxSCI_LUA_COMMENTLINE, wxColour (0,128,0));
	wxScintilla1->StyleSetForeground (wxSCI_LUA_FUNCTIONCALL, wxColour (wxT("BROWN")));
	wxScintilla1->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Courier New") ));
	wxScintilla1->SetKeyWords(0,"and break do else elseif end false for function if in local nil not or repeat return then true until while");
	wxScintilla1->SetKeyWords(4,"mat4 mat3 vec4 vec3 vec2 Timer Material Edge Face Mesh Object Model Texture FrameBuffer Image Buffer Program \
								 Shader Font FontManager TextureManager ShaderManager ModelManager Application RenderMode BufferType TextureFilter \
								 ShaderType EventType Key Renderer Logger vector_vec4 vector_vec3 vector_vec2 vector_float vector_int vector_uint vector_Face \
								 vector_Mesh vector_Buffer vector_vector_vec2 vector_Object vector_Material");
	wxScintilla1->SetKeyWords(5,"POINTS LINES TRIANGLES TRIANGLE_STRIP QUADS LINE_LOOP TRIANGLE_FAN STATIC DYNAMIC STREAM NEAREST LINEAR MIPMAP VERTEX_SHADER FRAGMENT_SHADER MAX_TEXTURES \
								 MOUSE_MOVE MOUSE_CLICK KEY_PRESSED KEY_DOWN KEY_UP KEY_UP_ARROW KEY_DOWN_ARROW KEY_LEFT_ARROW KEY_RIGHT_ARROW KEY_SPACE");
	wxScintilla1->SetModEventMask(wxSCI_MOD_INSERTTEXT);
}
bool outputCallback(string &str,bool out)
{
	MyApp *app=(MyApp*)wxTheApp;	
	app->frame->richText1->AppendText(str);
	return true;
}
DWORD WINAPI MyThread( LPVOID lpParam ) 
{ 	
	string *code=(string*)lpParam;		
	Application::InitializeLua();
	Application::SetOutputCallback(&outputCallback);
	Application::ExecuteString(*code);
	Application::CloseLua();
	MyMainFrame::running=false;
	MyApp *app=(MyApp*)wxTheApp;	
	app->frame->menuItem1->Enable(true);
	return 0; 
} 

void MyMainFrame::MenuItem1Clicked( wxCommandEvent& event )
{
	if (!running)
	{	
		menuItem1->Enable(false);
		running=true;
		richText1->Clear();
		static string code="";
		if (currentFile!="")
			wxScintilla1->SaveFile(currentFile);		
		code=wxScintilla1->GetText();
		CreateThread(NULL,0,MyThread,&code,0,NULL);
	}
}
void MyMainFrame::MenuItem2Clicked( wxCommandEvent& event )
{	
	currentFile="";	
	wxScintilla1->ClearAll();
}
void MyMainFrame::MenuItem3Clicked( wxCommandEvent& event )
{	
	wxString file=wxFileSelector(wxT("Open"),0,0,wxT(""),wxT("Lua Files(*.lua)|*.lua|All Files(*.*)|*.*"));		
	currentFile=file;
	if (currentFile=="")
		return;
	wxScintilla1->LoadFile(currentFile);
	string path=currentFile.substr(0,currentFile.find_last_of('\\'));
	wxSetWorkingDirectory(path);
}
void MyMainFrame::MenuItem4Clicked( wxCommandEvent& event )
{
	if (currentFile=="")
	{
		wxString file=wxFileSelector(wxT("Open"),0,0,wxT("lua"),wxT("*.lua"),wxFD_SAVE);	
		currentFile=file;
		if (currentFile=="")
			return;
		wxScintilla1->SaveFile(currentFile);
	}
	else
	{	
		wxScintilla1->SaveFile(currentFile);
	}
}
void MyMainFrame::MenuItem5Clicked( wxCommandEvent& event )
{
	wxString file=wxFileSelector(wxT("Open"),0,0,wxT("lua"),wxT("*.lua"),wxFD_SAVE);	
	currentFile=file;
	if (currentFile=="")
		return;
	wxScintilla1->SaveFile(currentFile);
}
void MyMainFrame::MenuItem6Clicked( wxCommandEvent& event )
{
	Close();
}
void MyMainFrame::Scintilla1MarginClicked( wxCommandEvent& event )
{
	wxScintillaEvent *evt=(wxScintillaEvent *)&event;
	int lineNum=wxScintilla1->LineFromPosition(evt->GetPosition());
	if (evt->GetMargin()==1)
	{
		ostringstream oss;
		
		oss << wxScintilla1->GetLineState(lineNum);
		//wxMessageBox(oss.str());
	}
	
	if (evt->GetMargin()==1)				
		wxScintilla1->ToggleFold(lineNum);
}
void MyMainFrame::Scintilla1Modified( wxCommandEvent& event )
{
	wxScintillaEvent *evt=(wxScintillaEvent *)&event;
	int lineNum=wxScintilla1->LineFromPosition(evt->GetPosition());	
	lastChar=wxScintilla1->GetCharAt(wxScintilla1->GetCurrentPos());
	if (lastChar==13)	
		ln=lineNum;	
}
void MyMainFrame::Scintilla1CharAdded( wxCommandEvent& event )
{
	wxScintillaEvent *evt=(wxScintillaEvent *)&event;
	int lineNum=wxScintilla1->LineFromPosition(evt->GetPosition());	
}
void MyMainFrame::Scintilla1UpdateUI( wxCommandEvent& event )
{
	wxScintillaEvent *evt=(wxScintillaEvent *)&event;
	int lineNum=wxScintilla1->LineFromPosition(evt->GetPosition());
	if (lastChar==13)
	{						
		wxScintilla1->SetLineIndentation(ln+1,wxScintilla1->GetLineIndentation(ln));
		int i=wxScintilla1->GetLineEndPosition(ln+1);
		wxScintilla1->SetSelection(i,i);
		lastChar=0;
	}
}