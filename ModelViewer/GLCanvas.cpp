#include <FireCube.h>
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
	crap = false;
}

GLCanvas::~GLCanvas()
{
	Disconnect(wxEVT_SIZE, wxSizeEventHandler(GLCanvas::OnSize), nullptr, this);
	Disconnect(wxEVT_PAINT, wxPaintEventHandler(GLCanvas::OnPaint), nullptr, this);
	Disconnect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(GLCanvas::OnEraseBackground), nullptr, this);
	Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GLCanvas::OnEnterWindow), nullptr, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::OnMotion), nullptr, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::OnMouseWheel), nullptr, this);
	crap = true;
}
void GLCanvas::Init()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	theApp->fcApp.InitializeNoWindow();
	engine = theApp->fcApp.GetEngine();
	renderingParameters.bgColor = vec4(0.249f, 0.521f, 1.0f, 1.0f);
	renderingParameters.renderingMode = GL_FILL;
	renderingParameters.cullFaceEnabled = true;
	renderingParameters.renderNormals = false;
	renderingParameters.renderTangents = false;    

//	renderingParameters.grid = GeometryPtr(new Geometry);    
//	renderingParameters.gridNode = GeometryNodePtr(new GeometryNode);
//	MaterialPtr material(new Material);
//	renderingParameters.grid->SetMaterial(material);
//	material->SetDiffuseColor(vec3(1, 1, 1));
//	renderingParameters.gridNode->SetGeometry(renderingParameters.grid);
//	renderingParameters.gridNode->SetLighting(false);
	
/*	renderingParameters.gridMaterial = MaterialPtr(new Material);
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
	renderingParameters.bitangentsMaterial->SetSpecularColor(vec3(0,0,0));*/
   
	theApp->GetDocument().CreateRootNode(engine);
	Node *root = theApp->GetDocument().GetRoot();
	scene = theApp->GetDocument().GetScene();

	mat4 mat;
	int w, h;
	GetClientSize(&w, &h);
	mat.GeneratePerspective(90.0f,(float) w / (float) h, 0.1f, 1000.0f);
	renderingParameters.camera = root->CreateComponent<NodeObserverCamera>();
	renderingParameters.camera->SetDistance(5.0f);
	renderingParameters.camera->SetMaxDistance(10000.0f);
	//renderingParameters.camera->SetProjectionMatrix(mat);

	
	//scene.SetRootNodeAndCamera(root, renderingParameters.camera);
	scene->SetCamera(renderingParameters.camera);
	
	theApp->GetDocument().CreateGrid(2, 20);

	renderingParameters.camera->SetTarget(root);
	theApp->LoadDocument("../Assets/Models/teapot2.3ds");

	Node *lightNode = root->CreateChild("Light");
	Light *l = lightNode->CreateComponent<Light>();
	l->SetLightType(DIRECTIONAL);
	l->SetDiffuseColor(vec4(0.7f, 0.7f, 0.7f, 1.0f));
	l->SetSpecularColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));	
	lightNode->Rotate(vec3((float)PI / 4.0f, (float)PI / 4.0f, 0));
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
	
	scene->SetFogColor(renderingParameters.bgColor.ToVec3()); // TODO change bgColor to vec3
	//engine->GetRenderer()->Clear(renderingParameters.bgColor, 1.0f);
		
	if (renderingParameters.cullFaceEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, renderingParameters.renderingMode);
		
	scene->Render(engine->GetRenderer());
	//Renderer::Render(theApp->GetDocument().GetRoot());
	/*if (renderingParameters.renderNormals)
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
	}*/
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
	
	engine->GetRenderer()->SetWidth(w);
	engine->GetRenderer()->SetHeight(h);
	renderingParameters.camera->SetPerspectiveProjectionParameters(90.0f, (float)w / (float)h, 0.1f, 1000.0f);

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
	//renderingParameters.camera->SetRotation(rotation);
}

vec3 GLCanvas::GetRotation()
{
	//return renderingParameters.camera->GetRotation();
	return vec3();
}
void GLCanvas::SetDistance(float v)
{
	renderingParameters.camera->SetDistance(v);
}
float GLCanvas::GetDistance() const
{
	return renderingParameters.camera->GetDistance();
}