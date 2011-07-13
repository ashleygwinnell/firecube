#include <FireCube.h>
using namespace std;

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "Document.h"
#include "GLCanvas.h"
#include "mainframe.h"
#include "MyMainFrame.h"
#include "main.h"

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id,
                       const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE , name ), init(false), theApp((MyApp*)wxTheApp)
{
	Connect(wxEVT_SIZE, wxSizeEventHandler(GLCanvas::OnSize));
	Connect(wxEVT_PAINT, wxPaintEventHandler(GLCanvas::OnPaint));
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(GLCanvas::OnEraseBackground));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GLCanvas::OnEnterWindow));
	Connect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::OnMotion));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::OnMouseWheel));
}

GLCanvas::~GLCanvas()
{
	Disconnect(wxEVT_SIZE, wxSizeEventHandler(GLCanvas::OnSize), NULL, this);
	Disconnect(wxEVT_PAINT, wxPaintEventHandler(GLCanvas::OnPaint), NULL, this);
	Disconnect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(GLCanvas::OnEraseBackground), NULL, this);
	Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(GLCanvas::OnEnterWindow), NULL, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::OnMotion), NULL, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::OnMouseWheel), NULL, this);
}
void GLCanvas::Init()
{
    FireCube::Filesystem::AddSearchPath("../Assets/Textures");
    theApp->fcApp.InitializeNoWindow();

    renderingParameters.bgColor = FireCube::vec4(0.249f, 0.521f, 1.0f, 1.0f);
    renderingParameters.renderingMode = GL_FILL;
    renderingParameters.cullFaceEnabled = true;
    renderingParameters.renderNormals = false;
    renderingParameters.renderTangents = false;    

    theApp->GetApplicationParameters().vshader = FireCube::Renderer::GetShaderManager().Create("plainColor.vert");
    theApp->GetApplicationParameters().fshader = FireCube::Renderer::GetShaderManager().Create("plainColor.frag");
    theApp->GetApplicationParameters().program = FireCube::ProgramPtr(new FireCube::Program);
    theApp->GetApplicationParameters().program->Create(theApp->GetApplicationParameters().vshader, theApp->GetApplicationParameters().fshader);

   
    FireCube::ShaderPtr nvShader(new FireCube::Shader);
    FireCube::ShaderPtr nfShader(new FireCube::Shader);
    nvShader->Create(FireCube::VERTEX_SHADER, "void main() \
								  {	\
									gl_Position = ftransform(); \
								  } ");

    nfShader->Create(FireCube::FRAGMENT_SHADER, "uniform vec4 color; \
									void main() \
									{ \
									gl_FragColor = color;  \
									} ");
    renderingParameters.plainColorProgram = FireCube::ProgramPtr(new FireCube::Program);
    renderingParameters.plainColorProgram->Create(nvShader, nfShader);
    
    theApp->LoadDocument("../Assets/Models/teapot2.3ds");
    CreateGrid(2, 20);
	FireCube::mat4 mat;
	int w, h;
	GetClientSize(&w, &h);
	mat.GeneratePerspective(90.0f,(float) w / (float) h, 0.1f, 1000.0f);
	renderingParameters.camera = FireCube::NodeObserverCameraPtr(new FireCube::NodeObserverCamera);
	renderingParameters.camera->SetDistance(5.0f);
	renderingParameters.camera->SetMaxDistance(10000.0f);
	renderingParameters.camera->SetProjectionMatrix(mat);
}
void GLCanvas::Render()
{    
    wxPaintDC dc(this);
    if (!GetContext()) return;
    SetCurrent();
    if (!init)
    {
        init = true;
        Init();
    }
    FireCube::Renderer::Clear(renderingParameters.bgColor, 1.0f);
    /*FireCube::vec3 dir;
	dir.FromAngles(renderingParameters.rot.x, renderingParameters.rot.y);
	dir*=renderingParameters.rot.z;
	FireCube::Renderer::GetCamera()->SetPosition(dir);
	FireCube::Renderer::GetCamera()->LookAt(FireCube::vec3(0.0f, 0.0f, 0.0f), FireCube::vec3(0.0f, 1.0f, 0.0f));*/
	FireCube::Renderer::SetCamera(renderingParameters.camera);
	renderingParameters.camera->SetTarget(theApp->GetDocument().GetRoot());
    FireCube::Renderer::SetModelViewMatrix(FireCube::Renderer::GetCamera()->GetViewMatrix());
    RenderGrid();
    if (renderingParameters.cullFaceEnabled)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, renderingParameters.renderingMode);
    
    FireCube::Renderer::Render(theApp->GetDocument().GetRoot());
    if (renderingParameters.renderNormals)
    {
        FireCube::Renderer::UseProgram(renderingParameters.plainColorProgram);
        renderingParameters.plainColorProgram->SetUniform("color", FireCube::vec4(0, 0, 1, 1));
        theApp->GetDocument().GetNormalRenderingBuffer()->SetVertexStream(3);
        FireCube::Renderer::RenderStream(FireCube::LINES, theApp->GetDocument().GetNormalRenderingBufferSize());
    }
    if (renderingParameters.renderTangents)
    {
        FireCube::Renderer::UseProgram(renderingParameters.plainColorProgram);
        renderingParameters.plainColorProgram->SetUniform("color", FireCube::vec4(1, 0, 0, 1));
        theApp->GetDocument().GetTangentRenderingBuffer()->SetVertexStream(3);
        FireCube::Renderer::RenderStream(FireCube::LINES, theApp->GetDocument().GetNormalRenderingBufferSize());
        
        renderingParameters.plainColorProgram->SetUniform("color", FireCube::vec4(0, 1, 0, 1));
        theApp->GetDocument().GetBitangentRenderingBuffer()->SetVertexStream(3);
        FireCube::Renderer::RenderStream(FireCube::LINES, theApp->GetDocument().GetNormalRenderingBufferSize());
    }
    SwapBuffers();
}

void GLCanvas::OnEnterWindow( wxMouseEvent& event )
{
    lastpos = FireCube::vec2(event.GetPosition().x, event.GetPosition().y);
}

void GLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    Render();
}
void GLCanvas::OnSize(wxSizeEvent& event)
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
	FireCube::mat4 mat;
	mat.GeneratePerspective(90.0f,(float) w / (float) h, 0.1f, 1000.0f);
	renderingParameters.camera->SetProjectionMatrix(mat);

}

void GLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing.
}
void GLCanvas::OnMotion(wxMouseEvent& event)
{
    FireCube::vec2 curpos(event.GetPosition().x, event.GetPosition().y);
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
    vector<FireCube::vec3> vertices;
    vector<DWORD> indices;
    for (DWORD i = 0; i < numberOfCells + 1; i++)
    {
        FireCube::vec3 pos1((float)i * size - size * (float)numberOfCells / 2.0f, 0, -size * (float)numberOfCells / 2.0f);
        FireCube::vec3 pos2((float)i * size - size * (float)numberOfCells / 2.0f, 0, size * (float)numberOfCells / 2.0f);
        FireCube::vec3 pos3(-size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
        FireCube::vec3 pos4(size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
        vertices.push_back(pos1);
        vertices.push_back(pos2);
        vertices.push_back(pos3);
        vertices.push_back(pos4);
    }
    renderingParameters.gridCount = vertices.size();
    renderingParameters.gridVertex = FireCube::BufferPtr(new FireCube::Buffer);
    renderingParameters.gridVertex->Create();
    renderingParameters.gridVertex->LoadData(&vertices[0], vertices.size()*sizeof(FireCube::vec3), FireCube::STATIC);
}
void GLCanvas::RenderGrid()
{    
    glDisable(GL_BLEND);

    FireCube::Renderer::UseProgram(renderingParameters.plainColorProgram);
    renderingParameters.plainColorProgram->SetUniform("color", FireCube::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    renderingParameters.gridVertex->SetVertexStream(3);
    FireCube::Renderer::DisableNormalStream();
    for (DWORD i = 0; i < FireCube::MAX_TEXTURES; i++)
        FireCube::Renderer::DisableTexCoordStream(i);
    FireCube::Renderer::RenderStream(FireCube::LINES, renderingParameters.gridCount);
}

void GLCanvas::SetBackgroundColor(FireCube::vec4 color)
{
	renderingParameters.bgColor = color;
}

FireCube::vec4 GLCanvas::GetBackgroundColor()
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

void GLCanvas::SetRotation(FireCube::vec3 rotation)
{
	renderingParameters.camera->SetRotation(rotation);
}

FireCube::vec3 GLCanvas::GetRotation()
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