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
	SetTitle(std::string("FireCube Test Application"));	
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	GetInputManager().AddMapping(Key::Q, InputMappingType::ACTION, "Test");
	SubscribeToEvent(Events::HandleInput, &App::HandleInput);
	auto rr = resourceCache->GetResource<Mesh>("../Assets/Models/AnimationTest.fbx");

	root = scene.GetRootNode();
	Node *childNode = root->CreateChild("Camera");
	InputManager &inputManager = GetInputManager();
	camera = childNode->CreateComponent<OrbitCamera>(inputManager);	
	camera->SetMaxAngX(0);
	camera->SetZoomFactor(1000.0f);	

	root->CreateComponent<PhysicsWorld>();

	Node *node;
	CollisionShape *collisionShape;
	RigidBody *rigidBody;
	
	node = root->CreateChild("LightNode");
	Light *light = node->CreateComponent<Light>();
	light->SetLightType(FireCube::LightType::DIRECTIONAL);
	light->SetColor(vec3(1.0f));
	light->SetCastShadow(true);
	node->Rotate(vec3(PI * 0.25f, 0.0f, 0.0f));

	node = root->CreateChild();
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(1.0f)), BoundingBox(vec3(-0.5f), vec3(0.5f)), resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	node->CreateComponent<StaticModel>(mesh);
	collisionShape = node->CreateComponent<CollisionShape>();
	collisionShape->SetBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));

	rigidBody = node->CreateComponent<RigidBody>();
	rigidBody->SetMass(1.0f);
	rigidBody->ApplyForce(vec3(100.0f), vec3(-0.5f, 0.0f, -0.5f));

	node = root->CreateChild();
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, vec2(10.0f)), BoundingBox(vec3(-10.0f), vec3(10.0f)), resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	node->CreateComponent<StaticModel>(mesh);
	node->Move(vec3(0, -1, 0));
		
	scene.SetFogColor(vec3(44, 80, 222) / 255.0f);	
	text = engine->GetUI()->GetRoot()->CreateChild<UIText>();
	text->SetFontFace(resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18));	

	renderer->SetSceneView(0, new SceneView(engine, &scene, camera));

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
	
}

void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
