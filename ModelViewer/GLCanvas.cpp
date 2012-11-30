#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "Document.h"
#include "GLCanvas.h"
#include "mainframe.h"
#include "MyMainFrame.h"
#include "main.h"

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id,
					   const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, nullptr, pos, size, style | wxFULL_REPAINT_ON_RESIZE , name ), init(false), theApp((MyApp*)wxTheApp)
{	
	context = new wxGLContext(this);
	Connect(wxEVT_SIZE, wxSizeEventHandler(GLCanvas::OnSize));
	Connect(wxEVT_PAINT, wxPaintEventHandler(GLCanvas::OnPaint));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(GLCanvas::OnEraseBackground));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GLCanvas::OnEnterWindow));
	Connect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::OnMotion));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::OnMouseWheel));
}

GLCanvas::~GLCanvas()
{
	Disconnect(wxEVT_SIZE, wxSizeEventHandler(GLCanvas::OnSize), nullptr, this);
	Disconnect(wxEVT_PAINT, wxPaintEventHandler(GLCanvas::OnPaint), nullptr, this);
	Disconnect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(GLCanvas::OnEraseBackground), nullptr, this);
	Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GLCanvas::OnEnterWindow), nullptr, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::OnMotion), nullptr, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::OnMouseWheel), nullptr, this);
}
void GLCanvas::Init()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	theApp->fcApp.InitializeNoWindow();

	renderingParameters.bgColor = vec4(0.249f, 0.521f, 1.0f, 1.0f);
	renderingParameters.renderingMode = GL_FILL;
	renderingParameters.cullFaceEnabled = true;
	renderingParameters.renderNormals = false;
	renderingParameters.renderTangents = false;    

	renderingParameters.grid = GeometryPtr(new Geometry);    
	renderingParameters.gridNode = GeometryNodePtr(new GeometryNode);
	MaterialPtr material(new Material);
	renderingParameters.grid->SetMaterial(material);
	material->SetDiffuseColor(vec3(1, 1, 1));
	renderingParameters.gridNode->SetGeometry(renderingParameters.grid);
	renderingParameters.gridNode->SetLighting(false);
	
	renderingParameters.gridMaterial = MaterialPtr(new Material);
	renderingParameters.gridMaterial->SetAmbientColor(vec3(0,0,0));
	renderingParameters.gridMaterial->SetDiffuseColor(vec3(1,1,1));
	renderingParameters.gridMaterial->SetSpecularColor(vec3(0,0,0));

	renderingParameters.normalsMaterial = MaterialPtr(new Material);
	renderingParameters.normalsMaterial->SetAmbientColor(vec3(0,0,0));
	renderingParameters.normalsMaterial->SetDiffuseColor(vec3(0,0,1));
	renderingParameters.normalsMaterial->SetSpecularColor(vec3(0,0,0));

	renderingParameters.tangentsMaterial = MaterialPtr(new Material);
	renderingParameters.tangentsMaterial->SetAmbientColor(vec3(0,0,0));
	renderingParameters.tangentsMaterial->SetDiffuseColor(vec3(1,0,0));
	renderingParameters.tangentsMaterial->SetSpecularColor(vec3(0,0,0));

	renderingParameters.bitangentsMaterial = MaterialPtr(new Material);
	renderingParameters.bitangentsMaterial->SetAmbientColor(vec3(0,0,0));
	renderingParameters.bitangentsMaterial->SetDiffuseColor(vec3(0,1,0));
	renderingParameters.bitangentsMaterial->SetSpecularColor(vec3(0,0,0));
   
	ShaderProperties shaderProperties;	
	renderingParameters.plainColorProgram = Renderer::GetTechnique("default")->GenerateProgram(shaderProperties);    
	
	theApp->LoadDocument("../Assets/Models/teapot2.3ds");
	CreateGrid(2, 20);
	mat4 mat;
	int w, h;
	GetClientSize(&w, &h);
	mat.GeneratePerspective(90.0f,(float) w / (float) h, 0.1f, 1000.0f);
	renderingParameters.camera = NodeObserverCameraPtr(new NodeObserverCamera);
	renderingParameters.camera->SetDistance(5.0f);
	renderingParameters.camera->SetMaxDistance(10000.0f);
	renderingParameters.camera->SetProjectionMatrix(mat);
}
void GLCanvas::Render()
{    
	wxPaintDC dc(this);
	if (!SetCurrent(*context))
		return;
	if (!init)
	{
		init = true;
		Init();
	}
	Renderer::Clear(renderingParameters.bgColor, 1.0f);
	
	Renderer::UseCamera(renderingParameters.camera);
	renderingParameters.camera->SetTarget(theApp->GetDocument().GetRoot());
	
	RenderGrid();
	if (renderingParameters.cullFaceEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, renderingParameters.renderingMode);
	
	Renderer::Render(theApp->GetDocument().GetRoot());
	if (renderingParameters.renderNormals)
	{
		Renderer::UseProgram(renderingParameters.plainColorProgram);
		Renderer::UseMaterial(renderingParameters.plainColorProgram, renderingParameters.normalsMaterial);
		theApp->GetDocument().GetNormalRenderingBuffer()->SetVertexAttribute(0, 3, 0, 0);
		Renderer::RenderStream(LINES, theApp->GetDocument().GetNormalRenderingBufferSize());
	}
	if (renderingParameters.renderTangents)
	{
		Renderer::UseProgram(renderingParameters.plainColorProgram);        
		Renderer::UseMaterial(renderingParameters.plainColorProgram, renderingParameters.tangentsMaterial);
		theApp->GetDocument().GetTangentRenderingBuffer()->SetVertexAttribute(0, 3, 0, 0);
		Renderer::RenderStream(LINES, theApp->GetDocument().GetNormalRenderingBufferSize());
				
		Renderer::UseMaterial(renderingParameters.plainColorProgram, renderingParameters.bitangentsMaterial);
		theApp->GetDocument().GetBitangentRenderingBuffer()->SetVertexAttribute(0, 3, 0, 0);
		Renderer::RenderStream(LINES, theApp->GetDocument().GetNormalRenderingBufferSize());
	}
	SwapBuffers();
}

void GLCanvas::OnEnterWindow( wxMouseEvent& event )
{
	lastpos = vec2(event.GetPosition().x, event.GetPosition().y);
}

void GLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	Render();
}
void GLCanvas::OnSize(wxSizeEvent& event)
{
	if (!IsShownOnScreen())
		return;
	
	int w, h;
	GetClientSize(&w, &h);
	if (!SetCurrent(*context))
		return;
	glViewport(0, 0, (GLint) w, (GLint) h);
	if (!init)
	{
		init = true;
		Init();
	}
	mat4 mat;
	mat.GeneratePerspective(90.0f,(float) w / (float) h, 0.1f, 1000.0f);
	renderingParameters.camera->SetProjectionMatrix(mat);

}

void GLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}
void GLCanvas::OnMotion(wxMouseEvent& event)
{
	vec2 curpos(event.GetPosition().x, event.GetPosition().y);
	if (event.LeftIsDown())
	{
		renderingParameters.camera->RotateX(-(curpos.y - lastpos.y) / 60.0f);
		renderingParameters.camera->RotateY(-(curpos.x - lastpos.x) / 60.0f);
	}
	if (event.MiddleIsDown())
	{
		if (event.ShiftDown())
			renderingParameters.camera->Zoom(-(curpos.y - lastpos.y) / 15.0f);
		else
			renderingParameters.camera->Zoom(-(curpos.y - lastpos.y) / 30.0f);
	}
	lastpos = curpos;
	this->Refresh(false);
}
void GLCanvas::OnMouseWheel(wxMouseEvent& event)
{
	int r = event.GetWheelRotation();
	if (event.ShiftDown())
		renderingParameters.camera->Zoom((float)r / 400.0f);
	else
		renderingParameters.camera->Zoom((float)r / 150.0f);
	this->Refresh(false);
}
void GLCanvas::CreateGrid(float size, DWORD numberOfCells)
{	 
	renderingParameters.grid->GetVertices().clear();
	for (DWORD i = 0; i < numberOfCells + 1; i++)
	{
		vec3 pos1((float)i * size - size * (float)numberOfCells / 2.0f, 0, -size * (float)numberOfCells / 2.0f);
		vec3 pos2((float)i * size - size * (float)numberOfCells / 2.0f, 0, size * (float)numberOfCells / 2.0f);
		vec3 pos3(-size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		vec3 pos4(size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		renderingParameters.grid->GetVertices().push_back(pos1);
		renderingParameters.grid->GetVertices().push_back(pos2);
		renderingParameters.grid->GetVertices().push_back(pos3);
		renderingParameters.grid->GetVertices().push_back(pos4);
	}
	renderingParameters.grid->SetPrimitiveType(LINES);
	renderingParameters.grid->SetPrimitiveCount(renderingParameters.grid->GetVertices().size() / 2);    
	renderingParameters.grid->SetVertexCount(renderingParameters.grid->GetVertices().size());    
	renderingParameters.grid->UpdateBuffers();	
}
void GLCanvas::RenderGrid()
{            
	Renderer::Render(renderingParameters.gridNode);
}

void GLCanvas::SetBackgroundColor(vec4 color)
{
	renderingParameters.bgColor = color;
}

vec4 GLCanvas::GetBackgroundColor()
{
	return renderingParameters.bgColor;
}

void GLCanvas::SetRenderingMode(GLenum renderingMode)
{
	renderingParameters.renderingMode = renderingMode;
}

void GLCanvas::SetCullFace(bool enabled)
{
	renderingParameters.cullFaceEnabled = enabled;
}

bool GLCanvas::GetCullFace()
{
	return renderingParameters.cullFaceEnabled;
}

void GLCanvas::SetRenderingNormals(bool enabled)
{
	renderingParameters.renderNormals = enabled;
}

bool GLCanvas::GetRenderingNormals()
{
	return renderingParameters.renderNormals;
}

void GLCanvas::SetRenderingTangents(bool enabled)
{
	renderingParameters.renderTangents = enabled;
}

bool GLCanvas::GetRenderingTangents()
{
	return renderingParameters.renderTangents;
}

void GLCanvas::SetRotation(vec3 rotation)
{
	renderingParameters.camera->SetRotation(rotation);
}

vec3 GLCanvas::GetRotation()
{
	return renderingParameters.camera->GetRotation();
}
void GLCanvas::SetDistance(float v)
{
	renderingParameters.camera->SetDistance(v);
}
float GLCanvas::GetDistance() const
{
	return renderingParameters.camera->GetDistance();
}