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

using namespace FireCube;

EditorWindow::EditorWindow(Engine *engine) : Object(engine), gridNode(nullptr), gridMaterial(nullptr), gridGeometry(nullptr), currentOperation(Operation3::NONE), leftButtonDown(false), firstLeftDownOutside(true)
{
	
}

void EditorWindow::Render()
{
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
}

void EditorWindow::SetScene(FireCube::Scene *scene, NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->scene = scene;	
	this->editorState = editorState;
	this->rootDesc = rootDesc;	

	SubscribeToEvent(Events::HandleInput, &EditorWindow::HandleInput);
	SubscribeToEvent(editorState, editorState->selectedNodeChanged, &EditorWindow::SelectedNodeChanged);
	SubscribeToEvent(editorState, editorState->stateChanged, &EditorWindow::StateChanged);
	engine->GetInputManager()->AddMapping(Key::MOUSE_LEFT_BUTTON, InputMappingType::STATE, "RotateCamera", KeyModifier::SHIFT);
	engine->GetInputManager()->AddMapping(Key::MOUSE_LEFT_BUTTON, InputMappingType::STATE, "LeftDown", KeyModifier::NONE);
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
	editorState->nodeAdded(editorState, editorState->GetNodeMap()[root]);

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

	editorState->newSceneCreated(editorState);	
}

void EditorWindow::OpenSceneFile(const std::string &filename)
{
	//SetAllPanelsVisibility(true);

	editorState->SetCurrentSceneFile(filename);

	Filesystem::SetAssetsFolder(Filesystem::GetDirectoryName(Filesystem::GetDirectoryName(filename)));

	::SceneReader sceneReader(engine);

	Reset();

	if (sceneReader.Read(rootDesc, filename))
	{
		root = rootDesc->Instantiate(nullptr, engine, editorState->GetNodeMap());
		rootDesc->SetNode(root);
		scene->GetRootNode()->AddChild(root);
		//NodeAdded(&rootDesc);
	}

	//SetTitle("SceneEditor - " + filename);

	//assetBrowserPanel->PopulateDirectoryTree();
	//assetBrowserPanel->SetAssetsPath(Filesystem::GetAssetsFolder());

	//UpdateCamerasList();

	editorState->sceneChanged(editorState);
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

void EditorWindow::Reset()
{
	editorState->ClearCommands();
	//sceneTreeCtrl->Freeze();
	//sceneTreeCtrl->DeleteAllItems();
	//sceneTreeCtrl->Thaw();
	//nodeToTreeItem.clear();
	//treeItemToNode.clear();
	editorState->GetNodeMap().clear();

	if (scene)
	{
		scene->GetRootNode()->RemoveAllComponents();
		rootDesc->RemoveAllComponents();
		rootDesc->RemoveAllChildren();
	}

	editorState->SetSelectedNode(nullptr);
}

void EditorWindow::HandleInput(float dt, const MappedInput &input)
{
	if (mouseOverView && input.IsStateOn("RotateCamera") && currentCamera == defaultCamera)
	{
		float deltaX = input.GetValue("MouseXDelta");
		float deltaY = input.GetValue("MouseYDelta");
		defaultCamera->RotateX(-deltaY / 60.0f);
		defaultCamera->RotateY(-deltaX / 60.0f);
		UpdateGizmo();
	}

	if (mouseOverView && input.HasValue("MouseWheelY"))
	{
		defaultCamera->Zoom(dt * input.GetValue("MouseWheelY") * 600.0f);
		UpdateGizmo();
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
					UpdateGizmo();
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
					UpdateGizmo();
				}
			}
		}

		leftButtonDown = true;
	}
	else if (leftButtonDown)
	{
		if (mouseOverView && !firstLeftDownOutside)
		{
			leftButtonDown = false;

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
					UpdateGizmo();
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

	if (input.IsActionTriggered("UseTranslateGizmo"))
	{
		UseTranslateGizmo();
	}
	else if (input.IsActionTriggered("UseRotateGizmo"))
	{
		UseRotateGizmo();
	}
	else if (input.IsActionTriggered("UseScaleGizmo"))
	{
		UseScaleGizmo();
	}
	else if (input.IsActionTriggered("Esc"))
	{
		editorState->SetSelectedNode(nullptr);
		UpdateGizmo();
		currentOperation = Operation3::NONE;
	}
	else if (input.IsActionTriggered("Clone"))
	{
		auto nodeDesc = editorState->GetSelectedNode();
		if (nodeDesc)
		{
			auto clonedNode = nodeDesc->Clone();

			auto command = new AddNodeCommand(editorState, "Clone", clonedNode, nodeDesc->GetParent());
			editorState->ExecuteCommand(command);
			editorState->SetSelectedNode(clonedNode);
			UpdateGizmo();			
		}

	}
	else if (input.IsActionTriggered("MoveToGround"))
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
			}
		}
	}
	else if (input.IsActionTriggered("OrbitSelection") && currentCamera == defaultCamera)	
	{
		if (editorState->GetSelectedNode())
		{
			cameraTarget->SetTranslation(editorState->GetSelectedNode()->GetNode()->GetWorldPosition());
			UpdateGizmo();			
		}
	}
	else if (input.IsActionTriggered("Delete"))
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

void EditorWindow::SelectedNodeChanged(NodeDescriptor *nodeDesc)
{
	UpdateGizmo();
}

void EditorWindow::UseTranslateGizmo()
{
	if (transformGizmo != translateGizmo.Get())
	{
		transformGizmo->Hide();
		transformGizmo = translateGizmo.Get();
		UpdateGizmo();

		editorState->switchedToTranslateGizmo(editorState);
	}
}

void EditorWindow::UseRotateGizmo()
{
	if (transformGizmo != rotateGizmo.Get())
	{
		transformGizmo->Hide();
		transformGizmo = rotateGizmo.Get();
		UpdateGizmo();

		editorState->switchedToRotateGizmo(editorState);
	}
}

void EditorWindow::UseScaleGizmo()
{
	if (transformGizmo != scaleGizmo.Get())
	{
		transformGizmo->Hide();
		transformGizmo = scaleGizmo.Get();
		UpdateGizmo();

		editorState->switchedToScaleGizmo(editorState);
	}
}

void EditorWindow::StateChanged()
{
	UpdateGizmo();	
}