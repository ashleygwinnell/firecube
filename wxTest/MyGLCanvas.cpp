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
#include "MyGLCanvas.h"
#include "mainframe.h"
#include "MyMainFrame.h"
#include "main.h"

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
EVT_SIZE(MyGLCanvas::OnSize)
EVT_PAINT(MyGLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(MyGLCanvas::OnEraseBackground)
EVT_ENTER_WINDOW( MyGLCanvas::OnEnterWindow )
EVT_MOTION( MyGLCanvas::OnMotion )
EVT_MOUSEWHEEL( MyGLCanvas::OnMouseWheel )
END_EVENT_TABLE()

MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id,
						   const wxPoint& pos, const wxSize& size, long style, const wxString& name)
						   : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name ), init(false)
{
	
}

MyGLCanvas::MyGLCanvas(wxWindow *parent, const MyGLCanvas *other,
						   wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
						   const wxString& name )
						   : wxGLCanvas(parent, other->GetContext(), id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name), init(false)
{


}

MyGLCanvas::~MyGLCanvas()
{
}

void MyGLCanvas::Render()
{	
	FireCubeApp *app=&(((MyApp*)wxTheApp)->fireCubeApp);
	static FireCube::Program pr;
	wxPaintDC dc(this);
	if (!GetContext()) return;
	SetCurrent();
	if (!init)
	{		
		init=true;		
		app->InitializeNoWindow();
		model=mm.Create("teapot2.3ds");				
		vshader=FireCube::Application::GetContext().shaderManager->Create("v.vshader");
		fshader=FireCube::Application::GetContext().shaderManager->Create("p.fshader");
		pr.Create(vshader,fshader);
		model->SetProgram(pr);
		rot=FireCube::vec3(0,0,-5);
		font=FireCube::Application::GetContext().fontManager->Create("c:\\windows\\fonts\\arial.ttf:18");
		bgColor=FireCube::vec4(0.249f,0.521f,1.0f,1.0f);
		renderingMode=GL_FILL;
	}	
	glPolygonMode(GL_FRONT_AND_BACK,renderingMode);
	renderer.Clear(bgColor,1.0f);
	FireCube::mat4 mat;
	mat.Identity();
	renderer.SetPerspectiveProjection(90.0f,0.1f,100.0f);		
	mat.Translate(FireCube::vec3(0,0,rot.z));
	mat.RotateX(rot.x);
	mat.RotateY(rot.y);	
	renderer.SetModelViewMatrix(mat);
	renderer.Render(model);	

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	renderer.SetModelViewMatrix(FireCube::mat4());
	renderer.SetOrthographicProjection();
	ostringstream oss,oss2,oss3;
	oss << glGetString(GL_RENDERER);
	renderer.RenderText(font,FireCube::vec2(0,0),oss.str());	
	oss2 << "Num vertices:" << model->object[0].vertex.size();
	renderer.RenderText(font,FireCube::vec2(0,20),oss2.str());	
	oss3 << "Num faces:" << model->object[0].face.size();
	renderer.RenderText(font,FireCube::vec2(0,40),oss3.str());
	SwapBuffers();
}

void MyGLCanvas::OnEnterWindow( wxMouseEvent& event )
{
	SetFocus();
	lastpos=FireCube::vec2(event.GetPosition().x,event.GetPosition().y);
}

void MyGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	Render();
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
{
	if (!IsShownOnScreen())
		return;
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);

	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int w, h;
	GetClientSize(&w, &h);
	if (GetContext())
	{		
		SetCurrent();
		glViewport(0, 0, (GLint) w, (GLint) h);
	}
	
}

void MyGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}
void MyGLCanvas::OnMotion(wxMouseEvent& event)
{
	FireCube::vec2 curpos(event.GetPosition().x,event.GetPosition().y);
	if (event.LeftIsDown())	
	{		
		rot.x-=(curpos.y-lastpos.y)/20.0f;
		rot.y-=(curpos.x-lastpos.x)/20.0f;
	}
	lastpos=curpos;
	this->Refresh(false);
}
void MyGLCanvas::OnMouseWheel(wxMouseEvent& event)
{
	int r=event.GetWheelRotation();
	rot.z+=(float)r/150.0f;
	this->Refresh(false);	
}
