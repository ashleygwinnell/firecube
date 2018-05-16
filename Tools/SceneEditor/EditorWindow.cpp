#include "EditorWindow.h"
#include "EditorState.h"
#include <imgui.h>
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "Descriptors/PhysicsWorldDescriptor.h"
#include "SceneReader.h"
#include "Commands/AddNodeCommand.h"
#include "Commands/TransformCommands.h"
#include "Commands/RemoveNodeCommand.h"
#include "Commands/GroupCommand.h"
#include "Commands/AddComponentCommand.h"
#include "Descriptors/CameraDescriptor.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "AssetUtils.h"

using namespace FireCube;

EditorWindow::EditorWindow(Engine *engine) : Object(engine), gridNode(nullptr), gridMaterial(nullptr), gridGeometry(nullptr), currentOperation(Operation3::NONE), leftButtonDown(false), firstLeftDownOutside(true)
{
	
}

void EditorWindow::Render()
{
	UpdateGizmo();
	engine->GetRenderer()->SetRenderTarget(0, renderSurface);
	engine->GetRenderer()->UpdateFrameBuffer();
	if (editorState->GetSelectedNode())
	{
		if (editorState->GetSelectedNode())
		{
			RenderDebugGeometry(editorState->GetSelectedNode(), engine->GetDebugRenderer());
		}
		engine->GetDebugRenderer()->Render(currentCamera);
	}
	ImGui::SetNextDock(ImGuiDockSlot_Right);
	if (ImGui::BeginDock("Scene"))
	{
		ImGui::BeginChild("TT");
		ImVec2 pos = ImGui::GetCursorScreenPos();
		canvasPos = vec2(pos.x, pos.y);
		renderSurface->GetLinkedTexture()->GenerateMipMaps();
		ImGui::GetWindowDrawList()->AddImage(
			(void *)renderSurface->GetLinkedTexture()->GetObjectId(), pos,
			ImVec2(pos.x + renderSurface->GetWidth(), pos.y + renderSurface->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));

		ImVec2 size = ImGui::GetContentRegionAvail();
		canvasSize = vec2(size.x, size.y);
		mouseOverView = ImGui::IsMouseHoveringWindow();
		if (size.x != renderSurface->GetWidth() || size.y != renderSurface->GetHeight())
		{
			renderSurface = engine->GetRenderer()->GetRenderSurface((int) size.x, (int) size.y, RenderSurfaceType::COLOR);
			engine->GetRenderer()->SetSceneView(1, new SceneView(engine, scene, currentCamera, renderSurface));
			engine->GetRenderer()->SetSceneView(0, new SceneView(engine, editorScene, currentCamera, renderSurface, engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/ForwardNoClear.xml")));
		}
		ImGui::End();
	}
	ImGui::EndDock();

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset");
		if (payload)
		{
			AssetType type;
			std::string path;
			AssetUtils::DeserializeAssetDescription((const char *)payload->Data, type, path);
			std::replace(path.begin(), path.end(), '\\', '/');
			path = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), path);

			if (type == AssetType::MESH)
			{
				AddMesh(path);
			}
			else if (type == AssetType::PREFAB)
			{
				AddPrefab(path);
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void EditorWindow::SetScene(FireCube::Scene *scene, NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->scene = scene;	
	this->editorState = editorState;
	this->rootDesc = rootDesc;	

	SubscribeToEvent(Events::HandleInput, &EditorWindow::HandleInput);
	SubscribeToEvent(editorState->startMaterialPick, &EditorWindow::StartMaterialPick);
	SubscribeToEvent(editorState->addMesh, &EditorWindow::AddMesh);
	SubscribeToEvent(editorState->addPrefab, &EditorWindow::AddPrefab);
	engine->GetInputManager()->AddMapping(Key::MOUSE_LEFT_BUTTON, InputMappingType::STATE, "RotateCamera", KeyModifier::SHIFT);
	engine->GetInputManager()->AddMapping(Key::MOUSE_LEFT_BUTTON, InputMappingType::STATE, "LeftDown", KeyModifier::NONE);
	engine->GetInputManager()->AddMapping(Key::MOUSE_RIGHT_BUTTON, InputMappingType::STATE, "PanCamera", KeyModifier::NONE);
	engine->GetInputManager()->AddMapping(AnalogInput::MOUSE_AXIS_X_RELATIVE, "MouseXDelta");
	engine->GetInputManager()->AddMapping(AnalogInput::MOUSE_AXIS_Y_RELATIVE, "MouseYDelta");
	engine->GetInputManager()->AddMapping(AnalogInput::MOUSE_AXIS_X_ABSOLUTE, "MouseX");
	engine->GetInputManager()->AddMapping(AnalogInput::MOUSE_AXIS_Y_ABSOLUTE, "MouseY");
	engine->GetInputManager()->AddMapping(AnalogInput::MOUSE_WHEEL_Y_RELATIVE, "MouseWheelY");
	engine->GetInputManager()->AddMapping(Key::Q, InputMappingType::ACTION, "UseTranslateGizmo");
	engine->GetInputManager()->AddMapping(Key::W, InputMappingType::ACTION, "UseRotateGizmo");
	engine->GetInputManager()->AddMapping(Key::E, InputMappingType::ACTION, "UseScaleGizmo");
	engine->GetInputManager()->AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Esc");
	engine->GetInputManager()->AddMapping(Key::SPACE, InputMappingType::ACTION, "Clone");
	engine->GetInputManager()->AddMapping(Key::G, InputMappingType::ACTION, "MoveToGround");
	engine->GetInputManager()->AddMapping(Key::Z, InputMappingType::ACTION, "OrbitSelection", KeyModifier::NONE);
	engine->GetInputManager()->AddMapping(Key::R, InputMappingType::ACTION, "ResetCameraOrbit", KeyModifier::NONE);
	engine->GetInputManager()->AddMapping(Key::DELETE, InputMappingType::ACTION, "Delete");

	editorScene = new FireCube::Scene(engine);
	root = scene->GetRootNode();
	editorRoot = editorScene->GetRootNode();
	root->SetName("Root");

	rootDesc->SetNode(root);
	editorState->GetNodeMap()[root] = rootDesc;

	auto physicsWorldDescriptor = new PhysicsWorldDescriptor();
	editorState->GetNodeMap()[root]->AddComponent(physicsWorldDescriptor);
	physicsWorldDescriptor->SetParent(editorState->GetNodeMap()[root]);
	physicsWorldDescriptor->CreateComponent(root, engine);
	
	cameraTarget = root->CreateChild("Editor_CameraTarget");
	Node *cameraNode = cameraTarget->CreateChild("Camera");
	defaultCamera = cameraNode->CreateComponent<OrbitCamera>();
	defaultCamera->SetFarPlane(2000.0f);
	defaultCamera->SetDistance(5.0f);
	defaultCamera->SetMaxDistance(10000.0f);
	currentCamera = defaultCamera;

	scene->SetFogColor(vec3(0.2f, 0.2f, 0.2f));

	translateGizmo = new TranslateGizmo(engine, editorRoot);
	rotateGizmo = new RotateGizmo(engine, editorRoot);
	scaleGizmo = new ScaleGizmo(engine, editorRoot);
	transformGizmo = translateGizmo.Get();
	editorState->switchedToTranslateGizmo(editorState);

	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f));
	gridMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));

	CreateGrid(10.0f, 100);

	renderSurface = engine->GetRenderer()->GetRenderSurface(128, 128, RenderSurfaceType::COLOR);
	engine->GetRenderer()->SetSceneView(1, new SceneView(engine, scene, currentCamera, renderSurface));
	engine->GetRenderer()->SetSceneView(0, new SceneView(engine, editorScene, currentCamera, renderSurface, engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/ForwardNoClear.xml")));
}

void EditorWindow::CreateGrid(float size, unsigned int numberOfCells)
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

void EditorWindow::HandleInput(float dt, const MappedInput &input)
{
	if (mouseOverView && input.IsStateOn("RotateCamera") && currentCamera == defaultCamera)
	{
		float deltaX = input.GetValue("MouseXDelta");
		float deltaY = input.GetValue("MouseYDelta");
		defaultCamera->RotateX(-deltaY / 60.0f);
		defaultCamera->RotateY(-deltaX / 60.0f);
	}

	if (mouseOverView && input.IsStateOn("PanCamera") && currentCamera == defaultCamera)
	{
		float deltaX = input.GetValue("MouseXDelta");
		float deltaY = input.GetValue("MouseYDelta");
		mat4 invViewMatrix = defaultCamera->GetViewMatrix();
		invViewMatrix.Inverse();
		vec3 right = vec3(1.0f, 0.0f, 0.0f).TransformNormal(invViewMatrix);
		vec3 up = vec3(0.0f, 1.0f, 0.0f).TransformNormal(invViewMatrix);
		cameraTarget->Move((right * -deltaX + up * deltaY) * dt);
	}

	if (mouseOverView && std::abs(input.GetValue("MouseWheelY")) > 0.0f)
	{
		defaultCamera->Zoom(input.GetValue("MouseWheelY") * (engine->GetInputManager()->IsKeyDown(Key::LEFT_SHIFT) ? 20.0f : 80.0f));
	}

	if (input.IsStateOn("LeftDown"))
	{
		if (leftButtonDown == false)
		{
			firstLeftDownOutside = !mouseOverView;
			
			if (mouseOverView)
			{
				vec2 mousePos(input.GetValue("MouseX"), input.GetValue("MouseY"));
				mousePos = mousePos - canvasPos;
				Ray ray = currentCamera->GetPickingRay(vec2(mousePos.x, canvasSize.y - mousePos.y), canvasSize.x, canvasSize.y);

				if (currentOperation == Operation3::NONE && editorState->GetSelectedNode())
				{
					bool startTransaform = transformGizmo->CheckOperationStart(editorScene, editorState->GetSelectedNode(), ray, vec2(mousePos.x, mousePos.y));
					if (startTransaform)
					{
						currentOperation = Operation3::OBJECT_TRANSFORM;
					}
				}

				if (currentOperation == Operation3::OBJECT_TRANSFORM)
				{
					transformGizmo->PerformOperation(ray, vec2(mousePos.x, mousePos.y), editorState->GetSelectedNode());
				}
			}
		}
		else
		{
			if (mouseOverView)
			{
				vec2 mousePos(input.GetValue("MouseX"), input.GetValue("MouseY"));
				mousePos = mousePos - canvasPos;
				Ray ray = currentCamera->GetPickingRay(vec2(mousePos.x, canvasSize.y - mousePos.y), canvasSize.x, canvasSize.y);

				if (currentOperation == Operation3::OBJECT_TRANSFORM)
				{
					transformGizmo->PerformOperation(ray, vec2(mousePos.x, mousePos.y), editorState->GetSelectedNode());
				}
			}
		}

		leftButtonDown = true;
	}
	else if (leftButtonDown)
	{
		leftButtonDown = false;

		if (mouseOverView && !firstLeftDownOutside)
		{
			vec2 mousePos(input.GetValue("MouseX"), input.GetValue("MouseY"));
			mousePos = mousePos - canvasPos;
			Ray ray = currentCamera->GetPickingRay(vec2(mousePos.x, canvasSize.y - mousePos.y), canvasSize.x, canvasSize.y);
			RayQuery query(ray, 10e4);

			if (currentOperation == Operation3::NONE)
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
				}
			}
			else if (currentOperation == Operation3::OBJECT_TRANSFORM)
			{
				transformGizmo->OperationEnd();
				editorState->ExecuteCommand(transformGizmo->GetCommand(editorState, editorState->GetSelectedNode()));
			}
			else if (currentOperation == Operation3::PICK_MATERIAL)
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
			currentOperation = Operation3::NONE;
		}
	}

	if (input.IsActionTriggered("UseTranslateGizmo") && !ImGui::GetIO().WantCaptureKeyboard)
	{
		UseTranslateGizmo();
	}
	else if (input.IsActionTriggered("UseRotateGizmo") && !ImGui::GetIO().WantCaptureKeyboard)
	{
		UseRotateGizmo();
	}
	else if (input.IsActionTriggered("UseScaleGizmo") && !ImGui::GetIO().WantCaptureKeyboard)
	{
		UseScaleGizmo();
	}
	else if (input.IsActionTriggered("Esc"))
	{
		editorState->SetSelectedNode(nullptr);
		currentOperation = Operation3::NONE;
	}
	else if (input.IsActionTriggered("Clone") && !ImGui::GetIO().WantCaptureKeyboard)
	{
		auto nodeDesc = editorState->GetSelectedNode();
		if (nodeDesc)
		{
			auto clonedNode = nodeDesc->Clone();

			auto command = new AddNodeCommand(editorState, "Clone", clonedNode, nodeDesc->GetParent());
			editorState->ExecuteCommand(command);
			editorState->SetSelectedNode(clonedNode);
		}

	}
	else if (input.IsActionTriggered("MoveToGround") && !ImGui::GetIO().WantCaptureKeyboard)
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
			}
		}
	}
	else if (input.IsActionTriggered("OrbitSelection") && currentCamera == defaultCamera && !ImGui::GetIO().WantCaptureKeyboard)
	{
		if (editorState->GetSelectedNode())
		{
			cameraTarget->SetTranslation(editorState->GetSelectedNode()->GetNode()->GetWorldPosition());
		}
	}
	else if (input.IsActionTriggered("ResetCameraOrbit") && currentCamera == defaultCamera && !ImGui::GetIO().WantCaptureKeyboard)
	{
		cameraTarget->SetTranslation(vec3(0.0f));
	}
	else if (input.IsActionTriggered("Delete") && !ImGui::GetIO().WantCaptureKeyboard)
	{
		if (editorState->GetSelectedNode())
		{
			editorState->ExecuteCommand(new RemoveNodeCommand(editorState, "Remove Node", editorState->GetSelectedNode()));
			editorState->SetSelectedNode(nullptr);
		}
	}
}

void EditorWindow::UpdateGizmo()
{
	if (transformGizmo && editorState->GetSelectedNode())
	{
		transformGizmo->UpdateTransformation(currentCamera, editorState->GetSelectedNode());
		transformGizmo->Show();
	}
	else if (transformGizmo && editorState->GetSelectedNode() == nullptr)
	{
		transformGizmo->Hide();
	}
}

void EditorWindow::UseTranslateGizmo()
{
	if (transformGizmo != translateGizmo.Get())
	{
		transformGizmo->Hide();
		transformGizmo = translateGizmo.Get();		

		editorState->switchedToTranslateGizmo(editorState);
	}
}

void EditorWindow::UseRotateGizmo()
{
	if (transformGizmo != rotateGizmo.Get())
	{
		transformGizmo->Hide();
		transformGizmo = rotateGizmo.Get();

		editorState->switchedToRotateGizmo(editorState);
	}
}

void EditorWindow::UseScaleGizmo()
{
	if (transformGizmo != scaleGizmo.Get())
	{
		transformGizmo->Hide();
		transformGizmo = scaleGizmo.Get();

		editorState->switchedToScaleGizmo(editorState);
	}
}

void EditorWindow::UseDefaultCamera()
{
	UseCamera(defaultCamera);
}

void EditorWindow::UseCamera(Camera *camera)
{
	currentCamera = camera;

	engine->GetRenderer()->SetSceneView(1, new SceneView(engine, scene, currentCamera, renderSurface));
	engine->GetRenderer()->SetSceneView(0, new SceneView(engine, editorScene, currentCamera, renderSurface, engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/ForwardNoClear.xml")));
}

FireCube::Camera * EditorWindow::GetCurrentCamera()
{
	return currentCamera;
}

bool EditorWindow::IsUsingDefaultCamera() const
{
	return currentCamera == defaultCamera;
}

void EditorWindow::UseCamera(CameraDescriptor *camera)
{
	UseCamera((Camera *)camera->GetComponent());
}

void EditorWindow::RenderDebugGeometry(NodeDescriptor *nodeDesc, DebugRenderer *debugRenderer)
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

	std::vector<Camera *> cameras;
	node->GetComponents(cameras);
	for (auto c : cameras)
	{
		c->RenderDebugGeometry(debugRenderer);
	}
}

void EditorWindow::StartMaterialPick()
{
	currentOperation = Operation3::PICK_MATERIAL;
}

void EditorWindow::AddMesh(const std::string &path)
{
	auto nodeDesc = new NodeDescriptor("Node");
	auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", nodeDesc, rootDesc);
	auto staticModelDescriptor = new StaticModelDescriptor();
	staticModelDescriptor->SetMeshFilename(path, engine);

	auto addComponentCommand = new AddComponentCommand(editorState, "Add Component", nodeDesc, staticModelDescriptor, engine);

	GroupCommand *groupCommand = new GroupCommand(editorState, "Add Mesh", { addNodeCommand, addComponentCommand });
	editorState->ExecuteCommand(groupCommand);
}

void EditorWindow::AddPrefab(const std::string &path)
{
	::SceneReader sceneReader(engine);
	auto prefab = sceneReader.ReadPrefab(path);
	if (prefab)
	{
		auto prefabInstance = prefab->Clone();
		auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", prefabInstance, rootDesc);
		editorState->ExecuteCommand(addNodeCommand);
	}
}
