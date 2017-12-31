#include "AuxRenderWindow.h"
#include "imgui.h"
#include "EditorState.h"

using namespace FireCube;

AuxRenderWindow::AuxRenderWindow(Engine *engine) : Object(engine), gridNode(nullptr), mouseOverView(false)
{
	SubscribeToEvent(Events::HandleInput, &AuxRenderWindow::HandleInput);

	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f));
	gridMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));

	scene = new Scene(engine);
	root = scene->GetRootNode();
	clientRoot = root->CreateChild();
	CreateGrid(0.5f, 10);

	cameraTarget = root->CreateChild();
	Node *cameraNode = cameraTarget->CreateChild("Camera");
	camera = cameraNode->CreateComponent<OrbitCamera>();
	camera->SetFarPlane(20.0f);
	camera->SetDistance(2.0f);
	camera->SetMaxDistance(20.0f);
	camera->RotateX(-(float)PI * 0.25f);

	scene->SetFogColor(vec3(0.2f, 0.2f, 0.2f));

	renderSurface = engine->GetRenderer()->GetRenderSurface(128, 128, RenderSurfaceType::COLOR);
	sceneView = new SceneView(engine, scene, camera, renderSurface);
}

void AuxRenderWindow::Render()
{
	ImGui::BeginChild("TT");
	ImVec2 pos = ImGui::GetCursorScreenPos();
	canvasPos = vec2(pos.x, pos.y);

	ImVec2 size = ImGui::GetContentRegionAvail();
	canvasSize = vec2(size.x, size.y);
	mouseOverView = ImGui::IsMouseHoveringWindow();
	if (size.x != renderSurface->GetWidth() || size.y != renderSurface->GetHeight())
	{
		renderSurface = engine->GetRenderer()->GetRenderSurface((int)size.x, (int)size.y, RenderSurfaceType::COLOR);
		sceneView = new SceneView(engine, scene, camera, renderSurface);
	}

	engine->GetRenderer()->RenderSceneView(sceneView);
	renderSurface->GetLinkedTexture()->GenerateMipMaps();
	ImGui::GetWindowDrawList()->AddImage(
		(void *)renderSurface->GetLinkedTexture()->GetObjectId(), pos,
		ImVec2(pos.x + renderSurface->GetWidth(), pos.y + renderSurface->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void AuxRenderWindow::Reset()
{
	clientRoot->RemoveAllChildren();
	camera->SetDistance(2.0f);
	camera->SetRotation(vec3(-(float)PI * 0.25f, 0, 0));
}

Node *AuxRenderWindow::GetRoot()
{
	return clientRoot;
}

void AuxRenderWindow::CreateGrid(float size, unsigned int numberOfCells)
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

void AuxRenderWindow::HandleInput(float dt, const MappedInput &input)
{
	if (mouseOverView && input.IsStateOn("RotateCamera"))
	{
		float deltaX = input.GetValue("MouseXDelta");
		float deltaY = input.GetValue("MouseYDelta");
		camera->RotateX(-deltaY / 60.0f);
		camera->RotateY(-deltaX / 60.0f);
	}

	if (mouseOverView && input.HasValue("MouseWheelY"))
	{
		camera->Zoom(dt * input.GetValue("MouseWheelY") * 600.0f);
	}
}