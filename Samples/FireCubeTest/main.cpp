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
	
	/*childNode = root->CreateChild("Model");	
	StaticModel *staticModel = childNode->CreateComponent<StaticModel>(resourceCache->GetResource<Mesh>("scene.3ds"));
	auto luaScript = childNode->CreateComponent<LuaScript>();
	luaScript->CreateObject(resourceCache->GetResource<LuaFile>("../Samples/FireCubeTest/test.lua"), "Rotator");	
	luaScript->CallMemberFunction("SetRotationSpeed", vec3(0.0f, 0.0f, 0.0f));		
	
	childNode = root->CreateChild("Model");
	AnimatedModel *animatedModel = childNode->CreateComponent<AnimatedModel>();	
	animatedModel->CreateFromMesh(rr);
	childNode->Scale(vec3(0.05f));

	childNode = childNode->Clone();
	childNode->Move(vec3(5, 0, 0));

	childNode = root->CreateChild("Particles");
	childNode->CreateComponent<ParticleEmitter>(1000, engine->GetResourceCache()->GetResource<Material>("Materials/ParticleNoTexture.xml"));*/

	/*childNode = root->CreateChild("Terrain");	
	Terrain *terrain = childNode->CreateComponent<Terrain>();
	childNode->Scale(vec3(0.05f));
	Image *image = resourceCache->GetResource<Image>("heightmap3.bmp");
	terrain->SetPatchSize(64);
	terrain->CreateFromHeightMap(image);
	terrain->SetMaterial(resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	//terrain->SetMaterial(resourceCache->GetResource<Material>("Materials/DebugNormals.xml"));*/

	childNode = root->CreateChild("LightNode");	
	Light *light = childNode->CreateComponent<Light>();
	light->SetLightType(FireCube::LightType::DIRECTIONAL);
	light->SetColor(vec3(1.0f));
	light->SetCastShadow(true);
	childNode->Rotate(vec3(PI * 0.25f, 0.0f, 0.0f));
		
	scene.SetFogColor(vec3(44, 80, 222) / 255.0f);
	//renderer->SetCurrentRenderPath(resourceCache->GetResource<RenderPath>("RenderPaths/ForwardGrayscale.xml"));
	text = engine->GetUI()->GetRoot()->CreateChild<UIText>();
	text->SetFontFace(resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18));	
	
	/*SceneReader reader(engine);
	reader.Read(scene, "../Samples/FireCubeTest/scene.xml");*/	

	renderer->SetSceneView(0, new SceneView(engine, &scene, camera));

	return true;
}

void App::Update(float t)
{		
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	text->SetText(oss.str());
	//root->GetChild("LightNode")->Move(vec3(0.2f, 0.0f, 0.2f) * t);
}

void App::Render(float t)
{    			
	auto debugRenderer = engine->GetDebugRenderer();

	float radius = 0.1f;
	float height = 0.5f;
	vec3 initialPos(-5, 2.0f, -0.2f);
	vec3 targetPos(5.0f, 2.0f, -0.2f);
	std::vector<vec3> tri = { vec3(0, -2, -2), vec3(0, -2 ,2), vec3(0, 2 ,0) };

	
	for (unsigned int i = 0; i < tri.size(); i += 3)
	{
		debugRenderer->AddLine(tri[i], tri[i + 1], vec3(0, 1, 0));
		debugRenderer->AddLine(tri[i + 1], tri[i + 2], vec3(0, 1, 0));
		debugRenderer->AddLine(tri[i + 2], tri[i], vec3(0, 1, 0));
	}


	CollisionMesh cm;	
	for (unsigned int i = 0; i < tri.size(); i += 3)
	{		
		cm.triangles.push_back(CollisionTriangle(tri[i], tri[i + 1], tri[i + 2]));
	}
	
	CollisionResult result;

	auto p0 = initialPos - vec3(0, height * 0.5f, 0);
	auto p1 = initialPos + vec3(0, height * 0.5f, 0);		
	CollisionUtils::SweepCapsuleMesh((targetPos - initialPos).Normalized(), (targetPos - initialPos).Length(), p0, p1, radius, cm, mat4::IDENTITY, result);

	vec3 newEndPoint;
	if (result.collisionFound)
	{
		newEndPoint = (targetPos - initialPos).Normalized() * result.nearestDistance + initialPos;
		debugRenderer->AddSphere(result.nearestIntersectionPoint, 0.05f, 16, 16, vec3(0));
		debugRenderer->AddLine(result.nearestIntersectionPoint, result.nearestIntersectionPoint + result.nearestNormal * 1.0f, vec3(0, 0, 1));
	}
	else
	{
		newEndPoint = targetPos;
	}

	debugRenderer->AddCapsule(p0, p1, radius, 4, 8, vec3(0, 1, 0));
	debugRenderer->AddCapsule(newEndPoint - vec3(0, height * 0.5f, 0), newEndPoint + vec3(0, height * 0.5f, 0), radius, 4, 8, vec3(0, 1, 0));

	debugRenderer->AddLine(initialPos, targetPos, vec3(1, 1, 0));
	engine->GetDebugRenderer()->Render(camera);
}

void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
