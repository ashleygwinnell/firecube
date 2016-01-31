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
#include "Commands/AddComponentCommand.h"
#include "Commands/GroupCommand.h"
#include "Descriptors/PhysicsWorldDescriptor.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "AssetUtils.h"
#include "SceneReader.h"

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, nullptr, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()),
	init(false), theApp((MyApp*)wxTheApp), gridNode(nullptr), gridMaterial(nullptr), gridGeometry(nullptr), currentOperation(Operation::NONE), rootDesc(nullptr)
{
	context = new wxGLContext(this);
	Bind(wxEVT_SIZE, &GLCanvas::OnSize, this);
	Bind(wxEVT_PAINT, &GLCanvas::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &GLCanvas::OnEraseBackground, this);
	Bind(wxEVT_ENTER_WINDOW, &GLCanvas::OnEnterWindow, this);
	Bind(wxEVT_MOTION, &GLCanvas::OnMotion, this);
	Bind(wxEVT_MOUSEWHEEL, &GLCanvas::OnMouseWheel, this);
	Bind(wxEVT_LEFT_UP, &GLCanvas::OnLeftUp, this);
	Bind(wxEVT_LEFT_DOWN, &GLCanvas::OnLeftDown, this);
	Bind(wxEVT_KEY_UP, &GLCanvas::OnKeyUp, this);

	SetDropTarget(new CanvasDropTarget(this));
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
	Unbind(wxEVT_LEFT_DOWN, &GLCanvas::OnLeftDown, this);
	Unbind(wxEVT_KEY_UP, &GLCanvas::OnKeyUp, this);
}

void GLCanvas::Init()
{	
	Filesystem::SetCoreDataFolder("../../FireCube");

	theApp->fcApp.InitializeNoWindow();			
	
	editorState = theApp->GetEditorState();
	SubscribeToEvent(editorState, editorState->selectedNodeChanged, &GLCanvas::SelectedNodeChanged);
	SubscribeToEvent(editorState, editorState->stateChanged, &GLCanvas::StateChanged);
	SubscribeToEvent(editorState, editorState->sceneChanged, &GLCanvas::SceneChanged);
	SubscribeToEvent(editorState->startMaterialPick, &GLCanvas::StartMaterialPick);
	SubscribeToEvent(editorState->addMesh, &GLCanvas::AddMesh);
	SubscribeToEvent(editorState->addPrefab, &GLCanvas::AddPrefab);

	theApp->InitScene();
	scene = theApp->GetScene();
	editorScene = theApp->GetEditorScene();
	root = scene->GetRootNode();
	editorRoot = editorScene->GetRootNode();
	root->SetName("Root");
	
	theApp->GetMainFrame()->SetScene(scene);
	auto physicsWorldDescriptor = new PhysicsWorldDescriptor();
	editorState->GetNodeMap()[root]->AddComponent(physicsWorldDescriptor);
	physicsWorldDescriptor->SetParent(editorState->GetNodeMap()[root]);
	physicsWorldDescriptor->CreateComponent(root, engine);
	editorState->nodeAdded(editorState, editorState->GetNodeMap()[root]);
	
	cameraTarget = root->CreateChild("Editor_CameraTarget");
	Node *cameraNode = cameraTarget->CreateChild("Camera");
	camera = cameraNode->CreateComponent<OrbitCamera>();
	camera->SetFarPlane(2000.0f);	
	camera->SetDistance(5.0f);
	camera->SetMaxDistance(10000.0f);

	scene->SetFogColor(vec3(0.2f, 0.2f, 0.2f));		

	translateGizmo = new TranslateGizmo(engine, editorRoot);
	rotateGizmo = new RotateGizmo(engine, editorRoot);
	scaleGizmo = new ScaleGizmo(engine, editorRoot);
	transformGizmo = translateGizmo.Get();
	
	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f));
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));

	CreateGrid(10.0f, 100);	

	int w, h;
	GetClientSize(&w, &h);
	engine->GetRenderer()->SetWidth(w);
	engine->GetRenderer()->SetHeight(h);
	engine->GetRenderer()->SetViewport(0, 0, w, h);

	engine->GetRenderer()->SetSceneView(1, new SceneView(engine, scene, camera, nullptr));
	engine->GetRenderer()->SetSceneView(0, new SceneView(engine, editorScene, camera, nullptr, engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/ForwardNoClear.xml")));

	editorState->newSceneCreated(editorState);
}

void GLCanvas::SelectedNodeChanged(NodeDescriptor *nodeDesc)
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


void GLCanvas::RenderDebugGeometry(NodeDescriptor *nodeDesc, DebugRenderer *debugRenderer)
{
	auto node = nodeDesc->GetNode();
	std::vector<CollisionShape *> collisionShapes;
	node->GetComponents(collisionShapes);
	for (auto c : collisionShapes)
	{
		c->RenderDebugGeometry(debugRenderer);
	}

	std::vector<CharacterController *> characterControllers;
	node->GetComponents(characterControllers);
	for (auto c : characterControllers)
	{
		c->RenderDebugGeometry(debugRenderer);
	}

	std::vector<Light *> lights;
	node->GetComponents(lights);
	for (auto l : lights)
	{
		l->RenderDebugGeometry(debugRenderer);
	}

	std::vector<ParticleEmitter *> particleEmitters;
	node->GetComponents(particleEmitters);
	for (auto p : particleEmitters)
	{
		p->RenderDebugGeometry(debugRenderer);
	}

	for (auto child : nodeDesc->GetChildren())
	{
		RenderDebugGeometry(child, debugRenderer);
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
	
	engine->GetRenderer()->Render();
	
	if (editorState->GetSelectedNode())
	{
		RenderDebugGeometry(editorState->GetSelectedNode(), engine->GetDebugRenderer());
	}
	engine->GetDebugRenderer()->Render(camera);

	SwapBuffers();
}

void GLCanvas::OnEnterWindow(wxMouseEvent& event)
{
	if (!this->HasFocus())
	{
		this->SetFocus();
	}

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

void GLCanvas::OnLeftDown(wxMouseEvent& event)
{	
	vec2 curpos(event.GetPosition().x, event.GetPosition().y);

	if (event.ShiftDown() == false)
	{
		wxPoint mousePos = event.GetPosition();
		wxSize size = this->GetSize();
		Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());

		if (currentOperation == Operation::NONE && editorState->GetSelectedNode())
		{
			bool startTransaform = transformGizmo->CheckOperationStart(editorScene, editorState->GetSelectedNode(), ray, vec2(mousePos.x, mousePos.y));
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

void GLCanvas::OnMotion(wxMouseEvent& event)
{
	vec2 curpos(event.GetPosition().x, event.GetPosition().y);
	
	if (event.LeftIsDown() && event.ShiftDown() == true)
	{
		camera->RotateX(-(curpos.y - lastMousePos.y) / 60.0f);
		camera->RotateY(-(curpos.x - lastMousePos.x) / 60.0f);
		UpdateGizmo();
		this->Refresh(false);
	}
	else if (event.MiddleIsDown())
	{
		if (event.ShiftDown())
		{
			camera->Zoom(-(curpos.y - lastMousePos.y) / 15.0f);
			UpdateGizmo();
			this->Refresh(false);
		}
		else
		{
			camera->Zoom(-(curpos.y - lastMousePos.y) / 30.0f);
			UpdateGizmo();
			this->Refresh(false);
		}
	}
	else if (event.LeftIsDown() && event.ShiftDown() == false)
	{
		wxPoint mousePos = event.GetPosition();
		wxSize size = this->GetSize();
		Ray ray = camera->GetPickingRay(vec2(mousePos.x, size.GetHeight() - mousePos.y), size.GetWidth(), size.GetHeight());
		
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
			scene->IntersectRay(query);
			if (query.results.empty() == false)
			{
				for (auto &result : query.results)
				{					
					auto node = result.renderable->GetNode();
					if (node->GetName().substr(0, 7) != "Editor_")
					{
						editorState->SetSelectedNode(editorState->GetNodeMap()[node]);
						break;
					}
				}
			}
			else
			{
				editorState->SetSelectedNode(nullptr);
				UpdateGizmo();				
				this->Refresh(false);
			}			
		}
		else if (currentOperation == Operation::OBJECT_TRANSFORM)
		{
			transformGizmo->OperationEnd();
			editorState->ExecuteCommand(transformGizmo->GetCommand(editorState, editorState->GetSelectedNode()));
			this->Refresh(false);
		}
		else if (currentOperation == Operation::PICK_MATERIAL)
		{
			scene->IntersectRay(query);
			if (query.results.empty() == false)
			{
				auto &result = query.results.front();
				auto renderable = result.renderable;
				if (renderable->GetType() == StaticModel::GetTypeStatic())
				{
					auto staticModel = (StaticModel *)renderable;
					auto material = staticModel->GetMaterials()[0];
					editorState->materialPicked(editorState, material);
				}
			}
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
		auto nodeDesc = editorState->GetSelectedNode();
		if (nodeDesc)
		{			
			auto clonedNode = nodeDesc->Clone();
			
			auto command = new AddNodeCommand(editorState, "Clone", clonedNode, nodeDesc->GetParent());
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
			editorState->GetSelectedNode()->GetNode()->GetComponents<StaticModel>(models, true);
			if (models.empty() == false)
			{
				BoundingBox bbox;
				for (auto &m : models)
					bbox.Expand(m->GetWorldBoundingBox());
				vec3 newPos = editorState->GetSelectedNode()->GetNode()->GetWorldPosition();
				newPos.y += -bbox.GetMin().y;				
				editorState->ExecuteCommand(new SetTranslationCommand(editorState, "Translate", editorState->GetSelectedNode(), editorState->GetSelectedNode()->GetNode()->GetWorldPosition(), newPos));
				UpdateGizmo();
				this->Refresh(false);
			}
		}		
	}
	else if (event.GetKeyCode() == 'Z' && event.ControlDown() == false)
	{
		if (editorState->GetSelectedNode())
		{
			cameraTarget->SetTranslation(editorState->GetSelectedNode()->GetNode()->GetWorldPosition());
			UpdateGizmo();
			this->Refresh(false);
		}
	}
	else if (event.GetKeyCode() == WXK_DELETE)
	{
		if (editorState->GetSelectedNode())
		{							
			editorState->ExecuteCommand(new RemoveNodeCommand(editorState, "Remove Node", editorState->GetSelectedNode()));
			editorState->SetSelectedNode(nullptr);			
		}
	}
}

void GLCanvas::UpdateGizmo()
{
	if (transformGizmo && editorState->GetSelectedNode())
	{
		transformGizmo->UpdateTransformation(camera, editorState->GetSelectedNode());
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

void GLCanvas::StartMaterialPick()
{
	currentOperation = Operation::PICK_MATERIAL;
}

void GLCanvas::SetRootDescriptor(NodeDescriptor *rootDescriptor)
{
	this->rootDesc = rootDescriptor;
}

void GLCanvas::AddMesh(const std::string &path)
{
	auto nodeDesc = new NodeDescriptor("Node");
	auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", nodeDesc, rootDesc);
	auto staticModelDescriptor = new StaticModelDescriptor();
	staticModelDescriptor->SetMeshFilename(path, engine);

	auto addComponentCommand = new AddComponentCommand(editorState, "Add Component", nodeDesc, staticModelDescriptor, engine);

	GroupCommand *groupCommand = new GroupCommand(editorState, "Add Mesh", { addNodeCommand, addComponentCommand });
	editorState->ExecuteCommand(groupCommand);
}

void GLCanvas::AddPrefab(const std::string &path)
{
	::SceneReader sceneReader(engine, editorState);
	auto prefab = sceneReader.ReadPrefab(path);
	if (prefab)
	{
		auto prefabInstance = prefab->Clone();
		auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", prefabInstance, rootDesc);
		editorState->ExecuteCommand(addNodeCommand);
	}
}

CanvasDropTarget::CanvasDropTarget(GLCanvas *canvas) : wxDropTarget(new wxCustomDataObject(wxDataFormat("Asset"))), canvas(canvas)
{

}

wxDragResult CanvasDropTarget::OnData(wxCoord vX, wxCoord vY, wxDragResult eResult)
{
	if (GetData())
	{
		auto dataObject = (wxCustomDataObject*)GetDataObject();
		AssetType type;
		std::string path;
		AssetUtils::DeserializeAssetDescription((char *)dataObject->GetData(), type, path);
		std::replace(path.begin(), path.end(), '\\', '/');
		path = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);

		if (type == AssetType::MESH)
		{	
			canvas->AddMesh(path);			
		}
		else if (type == AssetType::PREFAB)
		{
			canvas->AddPrefab(path);			
		}
	}

	return eResult;
}

wxDragResult CanvasDropTarget::OnEnter(wxCoord x, wxCoord y, wxDragResult def)
{
	return OnDragOver(x, y, def);
}

wxDragResult CanvasDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
{
	return wxDragCopy;
}