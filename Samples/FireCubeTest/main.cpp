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

	root = scene.GetRootNode();
	Node *childNode = root->CreateChild("Camera");
	InputManager &inputManager = GetInputManager();
	camera = childNode->CreateComponent<OrbitCamera>(inputManager);	
	camera->SetMaxAngX(0);
	camera->SetZoomFactor(20000.0f);	
	camera->SetDistance(50.0f);
	camera->SetRotation(vec3(-PI * 0.15f, 0, 0));

	root->CreateComponent<PhysicsWorld>()->SetGravity(vec3(0, -30, 0));

	root->CreateComponent<Skybox>()->SetMaterial(engine->GetResourceCache()->GetResource<Material>("Skybox.xml"));
	
	auto node = root->CreateChild("LightNode");
	Light *light = node->CreateComponent<Light>();
	light->SetLightType(FireCube::LightType::DIRECTIONAL);
	light->SetColor(vec3(1.0f));
	light->SetCastShadow(true);
	node->Rotate(vec3(-PI * 0.25f, 0.0f, 0.0f));

	node = root->CreateChild();
	auto mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, vec2(100.0f)), BoundingBox(vec3(-50.0f), vec3(50.0f)), resourceCache->GetResource<Material>("Materials/Default.xml"));
	//node->CreateComponent<StaticModel>(mesh)->SetCastShadow(false);	

	node = root->CreateChild();
	auto particleEmitter = node->CreateComponent<ParticleEmitter>(1000, engine->GetResourceCache()->GetResource<Material>("Materials/ParticleNoTexture.xml"));
	particleEmitter->SetBoundingBox(BoundingBox(-vec3(10), vec3(10)));
	particleEmitter->SetBoxEmitter(vec3(5, 5, 5));
	//particleEmitter->SetSphereEmitter(20);
	particleEmitter->SetEmissionRate(100);
	particleEmitter->SetLifeTime(10, 12);	
	particleEmitter->SetSpeed(5, 10);	
	particleEmitter->SetPrewarm(true);

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
