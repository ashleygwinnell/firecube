#include <FireCube.h>
using namespace FireCube;
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "GLCanvas.h"
#include "mainframe.h"
#include "MainFrameImpl.h"
#include "app.h"
#include "Types.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "MainFrameImpl.h"
#include "Commands/TransformCommands.h"
#include "Commands/RemoveNodeCommand.h"
#include "Commands/AddNodeCommand.h"

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, nullptr, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()),
	init(false), theApp((MyApp*)wxTheApp), gridNode(nullptr), gridMaterial(nullptr), gridGeometry(nullptr), currentOperation(Operation::NONE)
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
	Filesystem::AddSearchPath("../Tools/SceneEditor");
	
	theApp->fcApp.InitializeNoWindow();	
	
	engine->GetRenderer()->SetCurrentRenderPath(engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/ForwardWithOverlay.xml"));
	
	editorState = theApp->GetEditorState();
	SubscribeToEvent(editorState, editorState->selectedNodeChanged, &GLCanvas::SelectedNodeChanged);
	SubscribeToEvent(editorState, editorState->stateChanged, &GLCanvas::StateChanged);
	SubscribeToEvent(editorState, editorState->sceneChanged, &GLCanvas::SceneChanged);
	
	theApp->InitScene();
	scene = theApp->GetScene();
	root = scene->GetRootNode();
	root->SetName("Root");
	editorState->nodeAdded(editorState, scene->GetRootNode());

	theApp->GetMainFrame()->SetScene(scene);
	
	cameraTarget = root->CreateChild("Editor_CameraTarget");
	Node *cameraNode = cameraTarget->CreateChild("Camera");
	camera = cameraNode->CreateComponent<OrbitCamera>();
	camera->SetFarPlane(2000.0f);
	scene->SetCamera(camera);
	camera->SetDistance(5.0f);
	camera->SetMaxDistance(10000.0f);

	scene->SetFogColor(vec3(0.2f, 0.2f, 0.2f));
	
	/*Node *lightNode = cameraNode->CreateChild("lightNode");
	Light *light = lightNode->CreateComponent<Light>();
	light->SetLightType(LightType::DIRECTIONAL);
	light->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));		*/

	translateGizmo = new TranslateGizmo(engine, root);
	rotateGizmo = new RotateGizmo(engine, root);
	scaleGizmo = new ScaleGizmo(engine, root);
	transformGizmo = translateGizmo.Get();
	
	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));

	CreateGrid(10.0f, 100);	

	int w, h;
	GetClientSize(&w, &h);
	engine->GetRenderer()->SetWidth(w);
	engine->GetRenderer()->SetHeight(h);
	engine->GetRenderer()->SetViewport(0, 0, w, h);
}

void GLCanvas::SelectedNodeChanged(Node *node)
{
	UpdateGizmo();
	this->Refresh(false);
}

void GLCanvas::StateChanged()
{
	UpdateGizmo();
	this->Refresh(false);
}

void GLCanvas::SceneChanged()
{
	UpdateGizmo();
	this->Refresh(false);
}


void GLCanvas::RenderDebugGeometry(FireCube::Node *node, DebugRenderer *debugRenderer)
{
	if (node->GetName().substr(0, 7) != "Editor_")
	{
		std::vector<CollisionShape *> collisionShapes;
		node->GetComponents(collisionShapes);
		for (auto c : collisionShapes)
		{
			c->RenderDebugGeometry(debugRenderer);
		}

		for (auto child : node->GetChildren())
		{
			RenderDebugGeometry(child, debugRenderer);
		}
	}
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
	
	FireCube::Frame frame(engine, scene);
	frame.Render(engine->GetRenderer());
	if (editorState->GetSelectedNode())
	{
		RenderDebugGeometry(editorState->GetSelectedNode(), engine->GetDebugRenderer());
	}
	engine->GetDebugRenderer()->Render(camera);

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
	if (!this->HasFocus())
	{
		this->SetFocus();
	}

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

		if (currentOperation == Operation::NONE && editorState->GetSelectedNode())
		{							
			bool startTransaform = transformGizmo->CheckOperationStart(scene, editorState->GetSelectedNode(), ray, vec2(mousePos.x, mousePos.y));
			if (startTransaform)
			{							
				currentOperation = Operation::OBJECT_TRANSFORM;
			}							
		}		
		
		if (currentOperation == Operation::OBJECT_TRANSFORM)
		{						
			transformGizmo->PerformOperation(ray, vec2(mousePos.x, mousePos.y), editorState->GetSelectedNode());
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
				auto node = result.renderable->GetNode();	
				editorState->SetSelectedNode(node);
			}
			else
			{
				editorState->SetSelectedNode(nullptr);				
			}			
		}
		else if (currentOperation == Operation::OBJECT_TRANSFORM)
		{
			editorState->ExecuteCommand(transformGizmo->GetCommand(editorState, editorState->GetSelectedNode()));
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
			UpdateGizmo();
			
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == 'W')
	{	
		if (transformGizmo != rotateGizmo.Get())
		{
			transformGizmo->Hide();
			transformGizmo = rotateGizmo.Get();			
			UpdateGizmo();
			
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == 'E')
	{
		if (transformGizmo != scaleGizmo.Get())
		{
			transformGizmo->Hide();
			transformGizmo = scaleGizmo.Get();			
			UpdateGizmo();
			
			this->Refresh(false);
		}
	}	
	else if (event.GetKeyCode() == WXK_ESCAPE)
	{
		editorState->SetSelectedNode(nullptr);
		UpdateGizmo();
		currentOperation = Operation::NONE;		
		this->Refresh(false);
	}
	else if (event.GetKeyCode() == WXK_SPACE)
	{
		auto node = editorState->GetSelectedNode();
		if (node)
		{
			auto clonedNode = node->Clone();
			auto command = new AddNodeCommand(editorState, clonedNode, clonedNode->GetParent());
			editorState->ExecuteCommand(command);
			editorState->SetSelectedNode(clonedNode);
			UpdateGizmo();			
			this->Refresh(false);
		}
		
	}
	else if (event.GetKeyCode() == 'G')
	{
		if (editorState->GetSelectedNode())
		{		
			std::vector<StaticModel *> models;
			editorState->GetSelectedNode()->GetComponents<StaticModel>(models, true);
			if (models.empty() == false)
			{
				BoundingBox bbox;
				for (auto &m : models)
					bbox.Expand(m->GetWorldBoundingBox());
				vec3 newPos = editorState->GetSelectedNode()->GetWorldPosition();
				newPos.y += -bbox.GetMin().y;				
				editorState->ExecuteCommand(new SetTranslationCommand(editorState, editorState->GetSelectedNode(), editorState->GetSelectedNode()->GetWorldPosition(), newPos));
				UpdateGizmo();
				this->Refresh(false);
			}
		}		
	}
	else if (event.GetKeyCode() == 'Z' && event.ControlDown() == false)
	{
		if (editorState->GetSelectedNode())
		{
			cameraTarget->SetTranslation(editorState->GetSelectedNode()->GetWorldPosition());
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == WXK_DELETE)
	{
		if (editorState->GetSelectedNode())
		{							
			editorState->ExecuteCommand(new RemoveNodeCommand(editorState, editorState->GetSelectedNode()));			
			editorState->SetSelectedNode(nullptr);			
		}
	}
}

void GLCanvas::UpdateGizmo()
{
	if (transformGizmo && editorState->GetSelectedNode())
	{
		transformGizmo->SetPosition(editorState->GetSelectedNode()->GetWorldPosition());
		transformGizmo->SetScale((camera->GetNode()->GetWorldPosition() - editorState->GetSelectedNode()->GetWorldPosition()).Length() * 0.1f);
		transformGizmo->Show();
	}
	else if (transformGizmo && editorState->GetSelectedNode() == nullptr)
	{
		transformGizmo->Hide();
	}	
}

void GLCanvas::CreateGrid(float size, unsigned int numberOfCells)
{
	if (!gridNode)
		gridNode = root->CreateChild("Editor_GridNode");
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
