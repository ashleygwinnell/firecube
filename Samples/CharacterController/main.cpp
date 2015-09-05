#include <sstream>

#include <FireCube.h>
using namespace FireCube;
#include "app.h"
App app;
int main(int argc, char *argv[])
{
	Filesystem::SetCoreDataFolder("../../FireCube");
	Filesystem::SetAssetsFolder("../../Assets");

	if (!app.Initialize())
		return 0;
	app.Run();
	return 0;
}

App::App() : scene(engine)
{

}

bool App::Prepare()
{
	SetTitle(std::string("Character controller test"));		
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	GetInputManager().AddMapping(Key::W, InputMappingType::STATE, "Forward");
	GetInputManager().AddMapping(Key::S, InputMappingType::STATE, "Backward");
	GetInputManager().AddMapping(Key::A, InputMappingType::STATE, "RotateLeft");
	GetInputManager().AddMapping(Key::D, InputMappingType::STATE, "RotateRight");
	GetInputManager().AddMapping(Key::SPACE, InputMappingType::STATE, "Jump");
	SubscribeToEvent(Events::HandleInput, &App::HandleInput);

	root = scene.GetRootNode();
	root->CreateComponent<PhysicsWorld>();

	Node *childNode, *childNode2;
	CollisionShape *collisionShape;
	StaticModel *staticModel;

	childNode = root->CreateChild("Model");
	childNode->Move(vec3(0, -5, 0));
	childNode->Scale(vec3(2.0f));
	staticModel = childNode->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(resourceCache->GetResource<Mesh>("Models/level1.dae"));
	collisionShape = childNode->CreateComponent<CollisionShape>();
	collisionShape->SetMesh(resourceCache->GetResource<Mesh>("Models/level1.dae"));
	collisionShape = childNode->CreateComponent<CollisionShape>();
	collisionShape->SetPlane(Plane(vec3(0, 1, 0), 0));
	childNode2 = childNode->CreateChild();	
	childNode2->Move(vec3(0.8f, 2, -0.8f));
	collisionShape = childNode2->CreateComponent<CollisionShape>();
	collisionShape->SetBox(BoundingBox(-vec3(0.7f, 2, 0.7f) * 0.5f, vec3(0.7f, 2, 0.7f) * 0.5f));

	Node *playerNode = root->CreateChild("Player");
	staticModel = playerNode->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, 0.5f, 20, 20), BoundingBox(vec3(-0.5f), vec3(0.5f)), resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));	
	staticModel->CreateFromMesh(mesh);
	playerNode->Move(vec3(2.0f, 10.0f, -2.0f));
	characterController = playerNode->CreateComponent<CharacterController>();
	characterController->SetRadius(0.5f);
	characterController->SetHeight(1.0f);
	childNode = playerNode->CreateChild("Gun");
	staticModel = childNode->CreateComponent<StaticModel>();
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.2f, 0.2f, 1.0f)), BoundingBox(vec3(-1.0f), vec3(1.0f)), resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));	
	staticModel->CreateFromMesh(mesh);
	childNode->Move(vec3(0.3f, 0.0f, -0.5f));

	childNode = playerNode->CreateChild("Camera");
	camera = childNode->CreateComponent<Camera>();		
	childNode->LookAt(vec3(0, 5, 5), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));	
	

	childNode = root->CreateChild("LightNode");
	Light *light = childNode->CreateComponent<Light>();
	light->SetLightType(FireCube::LightType::DIRECTIONAL);
	light->SetColor(vec4(1, 1, 1, 1));	
	childNode->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));	

	scene.SetFogColor(vec3(44, 80, 222) / 255.0f);
	//renderer->SetCurrentRenderPath(resourceCache->GetResource<RenderPath>("RenderPaths/ForwardGrayscale.xml"));

	text = engine->GetUI()->GetRoot()->CreateChild<UIText>();
	text->SetFontFace(resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18));

	renderer->SetSceneView(0, new SceneView(engine, &scene, camera));

	float startY = -5;
	float stepHeight = 0.5f;
	float stepDepth = 5;
	float stepWidth = 5;
	for (int i = 0; i < 5; ++i)
	{
		childNode = root->CreateChild();
		collisionShape = childNode->CreateComponent<CollisionShape>();
		collisionShape->SetBox(BoundingBox(vec3(-stepWidth * 0.5f, 0, 0), vec3(stepWidth * 0.5f, stepHeight, stepDepth)));
		childNode->SetTranslation(vec3(10, startY + (float)i * stepHeight, (float)i * stepDepth));
	}


	childNode = root->CreateChild();
	collisionShape = childNode->CreateComponent<CollisionShape>();
	collisionShape->SetBox(BoundingBox(vec3(0), vec3(5)));
	childNode->SetTranslation(vec3(-10, -5, 0));
	childNode->Rotate(vec3(0, 0.1f, 0));
	
	childNode = root->CreateChild();
	collisionShape = childNode->CreateComponent<CollisionShape>();
	collisionShape->SetBox(BoundingBox(vec3(0), vec3(5)));
	childNode->SetTranslation(vec3(-15, -5, 5));

	return true;
}
void App::Update(float t)
{
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	text->SetText(oss.str());
}
void App::Render(float t)
{
	root->GetComponent<PhysicsWorld>()->RenderDebugGeometry(debugRenderer);
	debugRenderer->Render(camera);
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
	
	if (input.IsStateOn("Forward"))
	{
		vec3 dir = characterController->GetNode()->GetWorldTransformation().GetDirection().Normalized();
		characterController->SetVelocity(characterController->GetVelocity() + dir * 2.0f * t);
	}

	if (input.IsStateOn("Backward"))
	{
		vec3 dir = characterController->GetNode()->GetWorldTransformation().GetDirection().Normalized();
		characterController->SetVelocity(characterController->GetVelocity() - dir * 2.0f * t);
	}

	if (input.IsStateOn("RotateLeft"))
	{
		characterController->GetNode()->Rotate(vec3(0, -2.0f, 0) * t);		
	}

	if (input.IsStateOn("RotateRight"))
	{
		characterController->GetNode()->Rotate(vec3(0, 2.0f, 0) * t);
	}

	if (input.IsStateOn("Jump") && characterController->IsOnGround())
	{
		characterController->SetVelocity(characterController->GetVelocity() + vec3(0.0f, 8.0f, 0.0f) * t);
	}

	
}
