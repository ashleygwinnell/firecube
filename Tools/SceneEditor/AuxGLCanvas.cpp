
#include "AuxGLCanvas.h"
#include "app.h"

using namespace FireCube;

AuxGLCanvas::AuxGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, nullptr, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()),
	init(false), theApp((MyApp*)wxTheApp), gridNode(nullptr), gridMaterial(nullptr), gridGeometry(nullptr)
{
	context = theApp->GetMainContext(this);
	Bind(wxEVT_SIZE, &AuxGLCanvas::OnSize, this);
	Bind(wxEVT_PAINT, &AuxGLCanvas::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &AuxGLCanvas::OnEraseBackground, this);
	Bind(wxEVT_ENTER_WINDOW, &AuxGLCanvas::OnEnterWindow, this);
	Bind(wxEVT_MOTION, &AuxGLCanvas::OnMotion, this);
	Bind(wxEVT_MOUSEWHEEL, &AuxGLCanvas::OnMouseWheel, this);
	Bind(wxEVT_LEFT_UP, &AuxGLCanvas::OnLeftUp, this);
	Bind(wxEVT_LEFT_DOWN, &AuxGLCanvas::OnLeftDown, this);
	Bind(wxEVT_KEY_UP, &AuxGLCanvas::OnKeyUp, this);	
}

AuxGLCanvas::~AuxGLCanvas()
{
	Unbind(wxEVT_SIZE, &AuxGLCanvas::OnSize, this);
	Unbind(wxEVT_PAINT, &AuxGLCanvas::OnPaint, this);
	Unbind(wxEVT_ERASE_BACKGROUND, &AuxGLCanvas::OnEraseBackground, this);
	Unbind(wxEVT_ENTER_WINDOW, &AuxGLCanvas::OnEnterWindow, this);
	Unbind(wxEVT_MOTION, &AuxGLCanvas::OnMotion, this);
	Unbind(wxEVT_MOUSEWHEEL, &AuxGLCanvas::OnMouseWheel, this);
	Unbind(wxEVT_LEFT_UP, &AuxGLCanvas::OnLeftUp, this);
	Unbind(wxEVT_LEFT_DOWN, &AuxGLCanvas::OnLeftDown, this);
	Unbind(wxEVT_KEY_UP, &AuxGLCanvas::OnKeyUp, this);
}

void AuxGLCanvas::Init()
{	
	theApp->InitEngine();

	editorState = theApp->GetEditorState();				
				
	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE_NAME, vec3(1.0f));
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));	

	scene = new Scene(engine);
	root = scene->GetRootNode();

	CreateGrid(0.5f, 10);

	cameraTarget = root->CreateChild();
	Node *cameraNode = cameraTarget->CreateChild("Camera");
	camera = cameraNode->CreateComponent<OrbitCamera>();
	camera->SetFarPlane(20.0f);
	camera->SetDistance(2.0f);
	camera->SetMaxDistance(20.0f);
	camera->RotateX(-(float) M_PI * 0.25f);	

	scene->SetFogColor(vec3(0.2f, 0.2f, 0.2f));
	
	sceneView = new SceneView(engine, scene, camera);

	constructSceneCallback(this);
}

void AuxGLCanvas::Render()
{
	wxPaintDC dc(this);
	if (!SetCurrent(*context))
		return;
	if (!init)
	{
		init = true;
		Init();
	}

	int w, h;
	GetClientSize(&w, &h);

	engine->GetRenderer()->SetWidth(w);
	engine->GetRenderer()->SetHeight(h);
	engine->GetRenderer()->SetViewport(0, 0, w, h);
	
	engine->GetRenderer()->RenderSceneView(sceneView);

	SwapBuffers();
}

void AuxGLCanvas::OnEnterWindow(wxMouseEvent& event)
{
	if (!this->HasFocus())
	{
		this->SetFocus();
	}

	lastMousePos = vec2(event.GetPosition().x, event.GetPosition().y);
}

void AuxGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	Render();
}
void AuxGLCanvas::OnSize(wxSizeEvent& event)
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

void AuxGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}

void AuxGLCanvas::OnLeftDown(wxMouseEvent& event)
{
	vec2 curpos(event.GetPosition().x, event.GetPosition().y);	

	lastMousePos = curpos;
}

void AuxGLCanvas::OnMotion(wxMouseEvent& event)
{
	vec2 curpos(event.GetPosition().x, event.GetPosition().y);	

	/*camera->RotateX(5.0f / 60.0f);
	camera->RotateY(5.0f / 60.0f);
	this->Refresh(false);*/

	if (event.LeftIsDown())
	{
		camera->RotateX(-(curpos.y - lastMousePos.y) / 60.0f);
		camera->RotateY(-(curpos.x - lastMousePos.x) / 60.0f);		
		this->Refresh(false);
	}
	else if (event.MiddleIsDown())
	{
		if (event.ShiftDown())
		{
			camera->Zoom(-(curpos.y - lastMousePos.y) / 15.0f);			
			this->Refresh(false);
		}
		else
		{
			camera->Zoom(-(curpos.y - lastMousePos.y) / 30.0f);			
			this->Refresh(false);
		}
	}	

	lastMousePos = curpos;
}
void AuxGLCanvas::OnMouseWheel(wxMouseEvent& event)
{
	int r = event.GetWheelRotation();
	if (event.ShiftDown())
	{
		camera->Zoom((float)r / 400.0f);		
	}
	else
	{
		camera->Zoom((float)r / 150.0f);		
	}
	this->Refresh(false);
}

void AuxGLCanvas::OnLeftUp(wxMouseEvent& event)
{
	
}

void AuxGLCanvas::OnKeyUp(wxKeyEvent& event)
{
	
}

void AuxGLCanvas::CreateGrid(float size, unsigned int numberOfCells)
{
	if (!gridNode)
		gridNode = root->CreateChild("Editor_GridNode");
	gridNode->RemoveAllComponents();
	gridGeometry = gridNode->CreateComponent<CustomGeometry>();
	for (unsigned int i = 0; i < numberOfCells + 1; i++)
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
	gridGeometry->SetCollisionQueryMask(0);
}

Node *AuxGLCanvas::GetRootNode()
{
	return root;
}
