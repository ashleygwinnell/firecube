#include <FireCube.h>
using namespace FireCube;
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "GLCanvas.h"
#include "mainframe.h"
#include "MainFrameImpl.h"
#include "app.h"
#include "Types.h"
#include "TranslateGizmo.h"

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, nullptr, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name), init(false), theApp((MyApp*)wxTheApp), gridNode(nullptr), gridMaterial(nullptr), gridGeometry(nullptr), currentNode(nullptr)
{
	context = new wxGLContext(this);
	Bind(wxEVT_SIZE, &GLCanvas::OnSize, this);
	Bind(wxEVT_PAINT, &GLCanvas::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &GLCanvas::OnEraseBackground, this);
	Bind(wxEVT_ENTER_WINDOW, &GLCanvas::OnEnterWindow, this);
	Bind(wxEVT_MOTION, &GLCanvas::OnMotion, this);
	Bind(wxEVT_MOUSEWHEEL, &GLCanvas::OnMouseWheel, this);
	Bind(wxEVT_LEFT_UP, &GLCanvas::OnLeftUp, this);
}

GLCanvas::~GLCanvas()
{
	Unbind(wxEVT_SIZE, &GLCanvas::OnSize, this);
	Unbind(wxEVT_PAINT, &GLCanvas::OnPaint, this);
	Unbind(wxEVT_ERASE_BACKGROUND, &GLCanvas::OnEraseBackground, this);
	Unbind(wxEVT_ENTER_WINDOW, &GLCanvas::OnEnterWindow, this);
	Unbind(wxEVT_MOTION, &GLCanvas::OnMotion, this);
	Unbind(wxEVT_MOUSEWHEEL, &GLCanvas::OnMouseWheel, this);
	Unbind(wxEVT_LEFT_UP, &GLCanvas::OnLeftUp, this);
}

void GLCanvas::Init()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	theApp->fcApp.InitializeNoWindow();
	engine = theApp->fcApp.GetEngine();

	theApp->GetProject().Init(engine);

	scene = theApp->GetProject().GetScene();	
	root = scene->GetRootNode();

	Node *cameraNode = root->CreateChild("Camera");
	camera = cameraNode->CreateComponent<OrbitCamera>();
	scene->SetCamera(camera);
	camera->SetDistance(5.0f);
	camera->SetMaxDistance(10000.0f);

	scene->SetFogColor(vec3(0.2f, 0.2f, 0.2f));
	
	Node *lightNode = root->CreateChild("lightNode");
	Light *light = lightNode->CreateComponent<Light>();
	light->SetLightType(LightType::DIRECTIONAL);
	light->SetColor(vec4(1.0f));
	lightNode->Rotate(vec3(PI * 0.7f, 0.2f, 0));

	for (int i = 0; i < 10; ++i)
	{
		Node *testNode = root->CreateChild("TestNode");
		StaticModel *model = testNode->CreateComponent<StaticModel>();
		SharedPtr<Mesh> mesh(new Mesh(engine));
		mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
		mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
		model->CreateFromMesh(mesh);
		model->SetCollisionQueryMask(USER_GEOMETRY);
		testNode->SetTranslation(vec3(RangedRandom(-10, 10), 0.5, RangedRandom(-10, 10)));
	}
	
	translateGizmo = new TranslateGizmo(engine, root);
	//translateGizmo->SetSnapToGrid(true);
	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));

	CreateGrid(1.0f, 100);
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
	
	scene->Render(engine->GetRenderer());

	SwapBuffers();
}

void GLCanvas::OnEnterWindow(wxMouseEvent& event)
{
	lastMousePos = vec2(event.GetPosition().x, event.GetPosition().y);
}

void GLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
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

	if (!init)
	{
		init = true;
		Init();
	}
	
	engine->GetRenderer()->SetWidth(w);
	engine->GetRenderer()->SetHeight(h);	
	engine->GetRenderer()->SetViewport(0, 0, w, h);
}

void GLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}

void GLCanvas::OnMotion(wxMouseEvent& event)
{
	vec2 curpos(event.GetPosition().x, event.GetPosition().y);
	
	if (event.MiddleIsDown())
	{
		if (event.ShiftDown())
			camera->Zoom(-(curpos.y - lastMousePos.y) / 15.0f);
		else
			camera->Zoom(-(curpos.y - lastMousePos.y) / 30.0f);
	}
	else if (event.LeftIsDown() && event.ShiftDown() == false)
	{
		wxPoint mousePos = event.GetPosition();
		wxSize size = this->GetSize();
		Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());

		if (currentOperation == "")
		{				
			bool startTranslation = translateGizmo->CheckOperationStart(scene, currentNode, ray);
			if (startTranslation)
			{							
				currentOperation = "Translate";				
			}		
			else
			{
				currentOperation = "CameraOrbit";				
			}
		}
		
		if (currentOperation == "CameraOrbit")
		{
			camera->RotateX(-(curpos.y - lastMousePos.y) / 60.0f);
			camera->RotateY(-(curpos.x - lastMousePos.x) / 60.0f);
			this->Refresh(false);
		}
		else if (currentOperation == "Translate")
		{			
			translateGizmo->PerformOperation(ray, currentNode);
			this->Refresh(false);
		}
	}

	lastMousePos = curpos;	
}
void GLCanvas::OnMouseWheel(wxMouseEvent& event)
{
	int r = event.GetWheelRotation();
	if (event.ShiftDown())
		camera->Zoom((float)r / 400.0f);
	else
		camera->Zoom((float)r / 150.0f);
	this->Refresh(false);
}

void GLCanvas::OnLeftUp(wxMouseEvent& event)
{
	if (event.ShiftDown() == false)
	{
		wxPoint mousePos = event.GetPosition();
		wxSize size = this->GetSize();
		Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());
		RayQuery query(ray, 10e4);
					
		if (currentOperation == "")
		{			
			scene->IntersectRay(query, USER_GEOMETRY);
			if (query.results.empty() == false)
			{
				auto &result = query.results.front();
				currentNode = result.renderable->GetNode();				
				translateGizmo->SetPosition(currentNode->GetWorldPosition());
				translateGizmo->Show();								
			}
			else
			{
				currentNode = nullptr;
				translateGizmo->Hide();
			}
			this->Refresh(false);
		}
		
		currentOperation = "";
	}
}

void GLCanvas::CreateGrid(float size, unsigned int numberOfCells)
{
	if (!gridNode)
		gridNode = root->CreateChild("GridNode");
	gridNode->RemoveAllComponents();
	gridGeometry = gridNode->CreateComponent<CustomGeometry>();
	for (DWORD i = 0; i < numberOfCells + 1; i++)
	{
		vec3 pos1((float)i * size - size * (float)numberOfCells / 2.0f, 0, -size * (float)numberOfCells / 2.0f);
		vec3 pos2((float)i * size - size * (float)numberOfCells / 2.0f, 0, size * (float)numberOfCells / 2.0f);
		vec3 pos3(-size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		vec3 pos4(size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		gridGeometry->AddVertex(pos1);
		gridGeometry->AddVertex(pos2);
		gridGeometry->AddVertex(pos3);
		gridGeometry->AddVertex(pos4);
	}
	gridGeometry->SetPrimitiveType(PrimitiveType::LINES);
	gridGeometry->SetMaterial(gridMaterial);	
	gridGeometry->UpdateGeometry();
	gridGeometry->SetCollisionQueryMask(AUX_GEOMETRY);
}
