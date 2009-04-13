#include <string>
#include <vector>
#include <sstream>
#include <map>
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

using namespace FireCube;
MyMainFrame::MyMainFrame( wxWindow* parent )
:
MainFrame( parent )
{

}
DWORD WINAPI MyThread( LPVOID lpParam ) 
{ 	
	string *code=(string*)lpParam;		
	Application::InitializeLua();
	Application::ExecuteString(*code);
	Application::CloseLua();
	return 0; 
} 

void MyMainFrame::MenuItem1Clicked( wxCommandEvent& event )
{
	static string code="";
	code=textCtrl1->GetValue();
	CreateThread(NULL,0,MyThread,&code,0,NULL);
}
void MyMainFrame::MenuItem2Clicked( wxCommandEvent& event )
{
	currentFile="";
	textCtrl1->SetValue("");
}
void MyMainFrame::MenuItem3Clicked( wxCommandEvent& event )
{	
	wxString file=wxFileSelector(wxT("Open"),0,0,wxT("lua"),wxT("*.lua"));	
	currentFile=file;
	if (currentFile=="")
		return;		
	string allText;
	wxFileInputStream input(currentFile);
	wxTextInputStream text(input);
	while(input.IsOk() && !input.Eof() )
	{
		allText+=text.ReadLine()+ "\n";		
	}
	textCtrl1->SetValue(allText);
}
void MyMainFrame::MenuItem4Clicked( wxCommandEvent& event )
{
	if (currentFile=="")
	{
		wxString file=wxFileSelector(wxT("Open"),0,0,wxT("lua"),wxT("*.lua"),wxFD_SAVE);	
		currentFile=file;
		if (currentFile=="")
			return;
		wxFileOutputStream fo(currentFile);
		wxTextOutputStream to(fo);
		to.WriteString(textCtrl1->GetValue());
	}
	else
	{	
		wxFileOutputStream fo(currentFile);
		wxTextOutputStream to(fo);
		to.WriteString(textCtrl1->GetValue());	
	}
}
void MyMainFrame::MenuItem5Clicked( wxCommandEvent& event )
{
	wxString file=wxFileSelector(wxT("Open"),0,0,wxT("lua"),wxT("*.lua"),wxFD_SAVE);	
	currentFile=file;
	if (currentFile=="")
		return;
	wxFileOutputStream fo(currentFile);
	wxTextOutputStream to(fo);
	to.WriteString(textCtrl1->GetValue());
}
void MyMainFrame::MenuItem6Clicked( wxCommandEvent& event )
{
	Close();
}
