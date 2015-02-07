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
#include "RotateGizmo.h"
#include "ScaleGizmo.h"
#include "MainFrameImpl.h"

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, nullptr, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name), 
	init(false), theApp((MyApp*)wxTheApp), gridNode(nullptr), gridMaterial(nullptr), gridGeometry(nullptr), currentNode(nullptr), currentOperation(Operation::NONE)
{
	context = new wxGLContext(this);
	Bind(wxEVT_SIZE, &GLCanvas::OnSize, this);
	Bind(wxEVT_PAINT, &GLCanvas::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &GLCanvas::OnEraseBackground, this);
	Bind(wxEVT_ENTER_WINDOW, &GLCanvas::OnEnterWindow, this);
	Bind(wxEVT_MOTION, &GLCanvas::OnMotion, this);
	Bind(wxEVT_MOUSEWHEEL, &GLCanvas::OnMouseWheel, this);
	Bind(wxEVT_LEFT_UP, &GLCanvas::OnLeftUp, this);
	Bind(wxEVT_KEY_UP, &GLCanvas::OnKeyUp, this);
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
	Unbind(wxEVT_KEY_UP, &GLCanvas::OnKeyUp, this);
}

void GLCanvas::Init()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	Filesystem::AddSearchPath("../Samples/SceneEditor");
	
	theApp->fcApp.InitializeNoWindow();	
	engine = theApp->fcApp.GetEngine();
	engine->GetRenderer()->SetCurrentRenderPath(engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/ForwardWithOverlay.xml"));

	theApp->GetProject().Init(engine);	

	scene = theApp->GetProject().GetScene();	
	root = scene->GetRootNode();

	((MainFrameImpl *) this->GetParent())->SetScene(scene);

	cameraTarget = root->CreateChild("CameraTarget");
	Node *cameraNode = cameraTarget->CreateChild("Camera");
	camera = cameraNode->CreateComponent<OrbitCamera>();
	scene->SetCamera(camera);
	camera->SetDistance(5.0f);
	camera->SetMaxDistance(10000.0f);

	scene->SetFogColor(vec3(0.2f, 0.2f, 0.2f));
	
	Node *lightNode = cameraNode->CreateChild("lightNode");
	Light *light = lightNode->CreateComponent<Light>();
	light->SetLightType(LightType::DIRECTIONAL);
	light->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	
	Node *testNode = root->CreateChild("TestNode");
	StaticModel *model = testNode->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh(new Mesh(engine));
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
	model->CreateFromMesh(mesh);
	model->SetCollisionQueryMask(USER_GEOMETRY);

	translateGizmo = new TranslateGizmo(engine, root);
	rotateGizmo = new RotateGizmo(engine, root);
	scaleGizmo = new ScaleGizmo(engine, root);
	transformGizmo = translateGizmo.Get();
	
	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));

	CreateGrid(10.0f, 100);	
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
	
	if (event.LeftIsDown() && event.ShiftDown() == true)
	{
		camera->RotateX(-(curpos.y - lastMousePos.y) / 60.0f);
		camera->RotateY(-(curpos.x - lastMousePos.x) / 60.0f);
		this->Refresh(false);
	}
	else if (event.MiddleIsDown())
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

		if (currentOperation == Operation::NONE)
		{							
			bool startTransaform = transformGizmo->CheckOperationStart(scene, currentNode, ray, vec2(mousePos.x, mousePos.y));
			if (startTransaform)
			{							
				currentOperation = Operation::OBJECT_TRANSFORM;
			}							
		}		
		
		if (currentOperation == Operation::OBJECT_TRANSFORM)
		{						
			transformGizmo->PerformOperation(ray, vec2(mousePos.x, mousePos.y), currentNode);
			UpdateGizmo();
			this->Refresh(false);
		}		
	}

	lastMousePos = curpos;	
}
void GLCanvas::OnMouseWheel(wxMouseEvent& event)
{
	int r = event.GetWheelRotation();
	if (event.ShiftDown())
	{
		camera->Zoom((float)r / 400.0f);
		UpdateGizmo();
	}
	else
	{
		camera->Zoom((float)r / 150.0f);
		UpdateGizmo();
	}
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
					
		if (currentOperation == Operation::NONE)
		{			
			scene->IntersectRay(query, USER_GEOMETRY);
			if (query.results.empty() == false)
			{
				auto &result = query.results.front();
				currentNode = result.renderable->GetNode();								
				UpdateGizmo();
				transformGizmo->Show();
			}
			else
			{
				currentNode = nullptr;				
				transformGizmo->Hide();
			}
			this->Refresh(false);
		}
		
		currentOperation = Operation::NONE;
	}
}

void GLCanvas::OnKeyUp(wxKeyEvent& event)
{
	if (event.GetKeyCode() == 'Q')
	{		
		if (transformGizmo != translateGizmo.Get())
		{
			transformGizmo->Hide();
			transformGizmo = translateGizmo.Get();
			if (currentNode)
			{
				UpdateGizmo();
				transformGizmo->Show();
			}
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == 'W')
	{	
		if (transformGizmo != rotateGizmo.Get())
		{
			transformGizmo->Hide();
			transformGizmo = rotateGizmo.Get();
			if (currentNode)
			{
				UpdateGizmo();
				transformGizmo->Show();
			}
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == 'E')
	{
		if (transformGizmo != scaleGizmo.Get())
		{
			transformGizmo->Hide();
			transformGizmo = scaleGizmo.Get();
			if (currentNode)
			{
				UpdateGizmo();
				transformGizmo->Show();
			}
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == WXK_ESCAPE)
	{
		if (transformGizmo)
			transformGizmo->Hide();
		currentNode = nullptr;
		currentOperation = Operation::NONE;		
		this->Refresh(false);
	}
	else if (event.GetKeyCode() == 'G')
	{
		if (currentNode)
		{		
			std::vector<StaticModel *> models;
			currentNode->GetComponents<StaticModel>(models, true);
			if (models.empty() == false)
			{
				BoundingBox bbox;
				for (auto &m : models)
					bbox.Expand(m->GetWorldBoundingBox());
				vec3 newPos = currentNode->GetWorldPosition();
				newPos.y += -bbox.GetMin().y;
				currentNode->SetTranslation(newPos);
				UpdateGizmo();
				this->Refresh(false);
			}
		}		
	}
	else if (event.GetKeyCode() == 'Z')
	{
		if (currentNode)
		{
			cameraTarget->SetTranslation(currentNode->GetWorldPosition());
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == WXK_DELETE)
	{
		if (currentNode)
		{
			if (transformGizmo)
				transformGizmo->Hide();
			
			currentNode->GetParent()->RemoveChild(currentNode);
			currentNode = nullptr;
			this->Refresh(false);
		}
	}
}

void GLCanvas::UpdateGizmo()
{
	if (transformGizmo && currentNode)
	{
		transformGizmo->SetPosition(currentNode->GetWorldPosition());
		transformGizmo->SetScale((camera->GetNode()->GetWorldPosition() - currentNode->GetWorldPosition()).Length() * 0.1f);
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
