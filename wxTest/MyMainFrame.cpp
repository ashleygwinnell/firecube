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

#include "wx/wx.h"
#include "wx/colordlg.h"
#include <wx/glcanvas.h>
#include "MyGLCanvas.h"
#include "MyMainFrame.h"
#include "main.h"

MyMainFrame::MyMainFrame( wxWindow* parent )
:
MainFrame( parent )
{

}
void MyMainFrame::MenuItem1Clicked( wxCommandEvent& event )
{
	((MyApp*)wxTheApp)->frame->Close();	
}
void MyMainFrame::MenuItem2Clicked( wxCommandEvent& event )
{
	static FireCube::Program pr;
	wxString file=wxFileSelector(wxT("Open"),0,0,wxT("3ds"),wxT("*.3ds"));
	FireCubeApp *app= &(((MyApp*)wxTheApp)->fireCubeApp);
	std::string sfile=file;
	if (sfile=="")
		return;
	string::size_type d=sfile.find_last_of("\\");
	if (d!=string::npos)
	{
		string path=sfile.substr(0,d+1);
		wxSetWorkingDirectory(path.c_str());
	}
	glCanvas->model=glCanvas->mm.Create(sfile);	
	pr.Create(glCanvas->vshader,glCanvas->fshader);
	glCanvas->model->SetProgram(pr);
}
void MyMainFrame::MenuItem3Clicked( wxCommandEvent& event )
{
	static FireCube::Program pr;
	wxString file=wxFileSelector(wxT("Open"),0,0,wxT("vshader"),wxT("*.vshader"));
	FireCubeApp *app= &(((MyApp*)wxTheApp)->fireCubeApp);
	std::string sfile=file;
	if (sfile=="")
		return;
	glCanvas->vshader=FireCube::Application::GetContext().shaderManager->Create(sfile);
	pr.Create(glCanvas->vshader,glCanvas->fshader);
	glCanvas->model->SetProgram(pr);
}
void MyMainFrame::MenuItem4Clicked( wxCommandEvent& event )
{
	static FireCube::Program pr;
	wxString file=wxFileSelector(wxT("Open"),0,0,wxT("fshader"),wxT("*.fshader"));
	FireCubeApp *app= &(((MyApp*)wxTheApp)->fireCubeApp);
	std::string sfile=file;
	if (sfile=="")
		return;
	glCanvas->fshader=FireCube::Application::GetContext().shaderManager->Create(sfile);
	pr.Create(glCanvas->vshader,glCanvas->fshader);
	glCanvas->model->SetProgram(pr);
}
void MyMainFrame::MenuItem5Clicked( wxCommandEvent& event )
{
	wxColor color=wxGetColourFromUser();
	if (color.IsOk())
	{	
		glCanvas->bgColor=FireCube::vec4(color.Red()/255.0f,color.Green()/255.0f,color.Blue()/255.0f,1.0f);
		glCanvas->Refresh();
	}
}
void MyMainFrame::MenuItem6Clicked( wxCommandEvent& event )
{
	glCanvas->renderingMode=GL_POINT;
	glCanvas->Refresh();
}
void MyMainFrame::MenuItem7Clicked( wxCommandEvent& event )
{
	glCanvas->renderingMode=GL_LINE;
	glCanvas->Refresh();
}
void MyMainFrame::MenuItem8Clicked( wxCommandEvent& event )
{
	glCanvas->renderingMode=GL_FILL;
	glCanvas->Refresh();
}