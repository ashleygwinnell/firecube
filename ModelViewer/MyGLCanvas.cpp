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
void MyGLCanvas::Init()
{
	FireCubeApp *fcApp=&(((MyApp*)wxTheApp)->fireCubeApp);
	MyApp *app=(MyApp*)wxTheApp;
	FireCube::Application::AddSearchPath("../Media/Textures");	
	fcApp->InitializeNoWindow();

	rot=FireCube::vec3(0,0,-5);
	lookat=FireCube::vec3(0,0,0);
	fcApp->font=FireCube::Renderer::GetFontManager()->Create("c:\\windows\\fonts\\arial.ttf",18);
	bgColor=FireCube::vec4(0.249f,0.521f,1.0f,1.0f);
	renderingMode=GL_FILL;
	cullFaceEnabled=true;
	renderNormals=false;

	fcApp->vshader=FireCube::Renderer::GetShaderManager()->Create("plainColor.vshader");
	fcApp->fshader=FireCube::Renderer::GetShaderManager()->Create("plainColor.fshader");
	fcApp->program=FireCube::Program(new FireCube::ProgramResource);
	fcApp->program->Create(fcApp->vshader,fcApp->fshader);
	
	fcApp->normalRenderingBuffer=FireCube::Buffer(new FireCube::BufferResource);
	fcApp->normalRenderingBuffer->Create();
	fcApp->plainColorProgram=FireCube::Program(new FireCube::ProgramResource);
	FireCube::Shader nvShader(new FireCube::ShaderResource);
	FireCube::Shader nfShader(new FireCube::ShaderResource);
	nvShader->Create(FireCube::VERTEX_SHADER,"void main() \
								  {	\
									gl_Position = ftransform(); \
								  } ");

	nfShader->Create(FireCube::FRAGMENT_SHADER,"uniform vec4 color; \
									void main() \
									{ \
									gl_FragColor = color;  \
									} ");
	fcApp->plainColorProgram->Create(nvShader,nfShader);		
	normalsLength=0.1f;
	fcApp->LoadModel("../Media/Models/teapot2.3ds");
	CreateGrid(2,20);
}
void MyGLCanvas::Render()
{	
	FireCubeApp *fcApp=&(((MyApp*)wxTheApp)->fireCubeApp);
	wxPaintDC dc(this);
	if (!GetContext()) return;
	SetCurrent();
	if (!init)
	{		
		init=true;
		Init();
	}	
	FireCube::Renderer::Clear(bgColor,1.0f);
	FireCube::mat4 mat;
	mat.Identity();
	FireCube::Renderer::SetPerspectiveProjection(90.0f,0.1f,1000.0f);		
	mat.Translate(lookat);
	mat.Translate(FireCube::vec3(0,0,rot.z));
	mat.RotateX(rot.x);
	mat.RotateY(rot.y);	
	FireCube::Renderer::SetModelViewMatrix(mat);
	RenderGrid();
	if (cullFaceEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK,renderingMode);

	FireCube::Renderer::Render(fcApp->model);	
	if (renderNormals)
	{
		FireCube::Renderer::UseProgram(fcApp->plainColorProgram);
		fcApp->plainColorProgram->SetUniform("color",FireCube::vec4(1,1,1,1));
		fcApp->normalRenderingBuffer->SetVertexStream(3);
		FireCube::Renderer::RenderStream(FireCube::LINES,fcApp->normalRenderingBufferSize);
	}
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	FireCube::Renderer::SetModelViewMatrix(FireCube::mat4());
	FireCube::Renderer::SetOrthographicProjection();
	ostringstream oss,oss2,oss3;
	oss << glGetString(GL_RENDERER);
	FireCube::Renderer::RenderText(fcApp->font,FireCube::vec2(0,0),FireCube::vec4(1,1,1,1),oss.str());	
	SwapBuffers();
}

void MyGLCanvas::OnEnterWindow( wxMouseEvent& event )
{	
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
		rot.x-=(curpos.y-lastpos.y)/60.0f;
		rot.y-=(curpos.x-lastpos.x)/60.0f;
	}
	if (event.RightIsDown())	
	{		
		lookat.x+=(curpos.x-lastpos.x)/30.0f;
		lookat.y-=(curpos.y-lastpos.y)/30.0f;
	}
	if (event.MiddleIsDown())
	{
		if (event.ShiftDown())
			rot.z-=(curpos.y-lastpos.y)/15.0f;
		else
			rot.z-=(curpos.y-lastpos.y)/30.0f;
	}
	lastpos=curpos;
	this->Refresh(false);
}
void MyGLCanvas::OnMouseWheel(wxMouseEvent& event)
{	
	int r=event.GetWheelRotation();
	if (event.ShiftDown())
		rot.z+=(float)r/400.0f;
	else	
		rot.z+=(float)r/150.0f;
	this->Refresh(false);	
}
void MyGLCanvas::CreateGrid(float size,DWORD numberOfCells)
{	
	vector<FireCube::vec3> vertices;
	vector<DWORD> indices;
	for (DWORD j=0;j<numberOfCells+1;j++)
		for (DWORD i=0;i<numberOfCells+1;i++)
		{
			FireCube::vec3 pos((float)i*size-size*(float)numberOfCells/2.0f,0,(float)j*size-size*(float)numberOfCells/2.0f);
			vertices.push_back(pos);
		}
	for (DWORD j=0;j<numberOfCells;j++)
	{
		for (DWORD i=0;i<numberOfCells;i++)
		{
			indices.push_back(j*(numberOfCells+1)+i);			
			indices.push_back(j*(numberOfCells+1)+i+1);

			indices.push_back(j*(numberOfCells+1)+i+1);				
			indices.push_back((j+1)*(numberOfCells+1)+i+1);

			indices.push_back((j+1)*(numberOfCells+1)+i+1);
			indices.push_back((j+1)*(numberOfCells+1)+i);

			indices.push_back((j+1)*(numberOfCells+1)+i);
			indices.push_back(j*(numberOfCells+1)+i);			
		}
	}
	gridCount=indices.size();	
	gridVertex=FireCube::Buffer(new FireCube::BufferResource);
	gridIndices=FireCube::Buffer(new FireCube::BufferResource);
	gridVertex->Create();
	gridIndices->Create();
	gridVertex->LoadData(&vertices[0],vertices.size()*sizeof(FireCube::vec3),FireCube::STATIC);
	gridIndices->LoadIndexData(&indices[0],indices.size(),FireCube::STATIC);
}
void MyGLCanvas::RenderGrid()
{
	FireCubeApp *fcApp=&(((MyApp*)wxTheApp)->fireCubeApp);
	glDisable(GL_BLEND);
	
	FireCube::Renderer::UseProgram(fcApp->plainColorProgram);
	fcApp->plainColorProgram->SetUniform("color",FireCube::vec4(0.7f,0.7f,0.7f,1.0f));
	gridVertex->SetVertexStream(3);
	gridIndices->SetIndexStream();
	FireCube::Renderer::DisableNormalStream();
	for (DWORD i=0;i<FireCube::MAX_TEXTURES;i++)
		FireCube::Renderer::DisableTexCoordStream(i);
	FireCube::Renderer::RenderIndexStream(FireCube::LINES,gridCount);
}