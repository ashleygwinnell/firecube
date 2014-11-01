#include <FireCube.h>
using namespace FireCube;
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "GLCanvas.h"
#include "mainframe.h"
#include "MainFrameImpl.h"
#include "app.h"

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

	Node *testNode = root->CreateChild("TestNode");
	StaticModel *model = testNode->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh(new Mesh(engine));
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
	model->CreateFromMesh(mesh);
	model->SetCollisionQueryMask(USER_GEOMETRY);

	testNode = root->CreateChild("TestNode2");
	testNode->Move(vec3(5.0f, 0.0f, 0.0f));
	model = testNode->CreateComponent<StaticModel>();
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	mesh->SetBoundingBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
	model->CreateFromMesh(mesh);
	model->SetCollisionQueryMask(USER_GEOMETRY);

	SharedPtr<Material> material = engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml")->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), material);
	mesh->SetBoundingBox(BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)));
	StaticModel *staticModel;
	Node *node;
	translateGizmo = root->CreateChild("TranslateGizmo");
	node = translateGizmo->CreateChild("XAxis");	
	node->Move(vec3(0.5f, 0.0f, 0.0f));
	node->Rotate(vec3(0.0f, 0.0f, PI * 0.5f));
	staticModel = node->CreateComponent<StaticModel>();	
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), material);
	mesh->SetBoundingBox(BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)));
	node = translateGizmo->CreateChild("YAxis");	
	node->Move(vec3(0.0f, 0.5f, 0.0f));
	staticModel = node->CreateComponent<StaticModel>();	
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(0.0f, 0.0f, 1.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), material);
	mesh->SetBoundingBox(BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)));
	node = translateGizmo->CreateChild("ZAxis");	
	node->Move(vec3(0.0f, 0.0f, -0.5f));
	node->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));
	staticModel = node->CreateComponent<StaticModel>();	
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

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
//Two non-parallel lines which may or may not touch each other have a point on each line which are closest
//to each other. This function finds those two points. If the lines are not parallel, the function
//outputs true, otherwise false.

bool ClosestPointsOnTwoLines(vec3 &closestPointLine1, vec3 linePoint1, vec3 lineVec1, vec3 linePoint2, vec3 lineVec2)
{
	float a = Dot(lineVec1, lineVec1);
	float b = Dot(lineVec1, lineVec2);
	float e = Dot(lineVec2, lineVec2);
	float d = a*e - b*b;
	
	//lines are not parallel
	if (d != 0.0f)
	{
		vec3 r = linePoint1 - linePoint2;
		float c = Dot(lineVec1, r);
		float f = Dot(lineVec2, r);
		float s = (b*f - c*e) / d;
		float t = (a*f - c*b) / d;
		closestPointLine1 = linePoint1 + lineVec1 * s;
		return true;
	}
	else
	{
		return false;
	}
}

void GLCanvas::OnMotion(wxMouseEvent& event)
{
	vec2 curpos(event.GetPosition().x, event.GetPosition().y);
	if (event.LeftIsDown() && event.ShiftDown())
	{
		camera->RotateX(-(curpos.y - lastMousePos.y) / 60.0f);
		camera->RotateY(-(curpos.x - lastMousePos.x) / 60.0f);
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
		if (currentOperation == "")
		{
			wxPoint mousePos = event.GetPosition();
			wxSize size = this->GetSize();
			Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());
			RayQuery query(ray, 10e4);

			scene->IntersectRay(query, GIZMO_GEOMETRY);
			if (query.results.empty() == false)
			{
				auto &result = query.results.front();
				Node *node = result.renderable->GetNode();
				//wxMessageBox(node->GetName());
				currentOperation = node->GetName();
				dragStart = query.ray.origin + query.ray.direction * result.distance;
				startPosition = currentNode->GetTranslation();
			}			
		}
		
		if (currentOperation == "XAxis")
		{
			wxPoint mousePos = event.GetPosition();
			wxSize size = this->GetSize();
			Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());			
			vec3 intersectionPoint;
			if (ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(1, 0, 0), ray.origin, ray.direction))
			{
				vec3 translation(startPosition.x + (intersectionPoint.x - dragStart.x), startPosition.y, startPosition.z);
				currentNode->SetTranslation(translation);
				translateGizmo->SetTranslation(currentNode->GetWorldPosition());
			}						
		}
		else if (currentOperation == "YAxis")
		{
			wxPoint mousePos = event.GetPosition();
			wxSize size = this->GetSize();
			Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());
			vec3 intersectionPoint;
			if (ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(0, 1, 0), ray.origin, ray.direction))
			{
				vec3 translation(startPosition.x, startPosition.y + (intersectionPoint.y - dragStart.y), startPosition.z);
				currentNode->SetTranslation(translation);
				translateGizmo->SetTranslation(currentNode->GetWorldPosition());
			}
		}
		else if (currentOperation == "ZAxis")
		{
			wxPoint mousePos = event.GetPosition();
			wxSize size = this->GetSize();
			Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());
			vec3 intersectionPoint;
			if (ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(0, 0, 1), ray.origin, ray.direction))
			{
				vec3 translation(startPosition.x, startPosition.y, startPosition.z + (intersectionPoint.z - dragStart.z));
				currentNode->SetTranslation(translation);
				translateGizmo->SetTranslation(currentNode->GetWorldPosition());
			}
		}
	}
	lastMousePos = curpos;
	this->Refresh(false);
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

		if (currentNode)
		{
			currentOperation = "";			
		}
		else
		{			
			scene->IntersectRay(query, USER_GEOMETRY);
			if (query.results.empty() == false)
			{
				auto &result = query.results.front();
				currentNode = result.renderable->GetNode();
				translateGizmo->SetTranslation(currentNode->GetWorldPosition());
				std::vector<StaticModel *> components;
				translateGizmo->GetComponents<StaticModel>(components, true);
				for (auto c : components)
				{
					c->SetEnabled(true);
				}
				this->Refresh(false);
			}
		}
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
