#include <sstream>

#include <FireCube.h>
using namespace FireCube;
#include "app.h"
App app;
int main(int argc, char *argv[])
{	
	Filesystem::AddSearchPath("../Assets/Textures");
	Filesystem::AddSearchPath("../Assets/Models");
	
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
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	

	auto rr = resourceCache->GetResource<AnimatedMesh>("../Assets/Models/AnimationTest.fbx");

	root = scene.GetRootNode();
	Node *childNode = root->CreateChild("Camera");
	camera = childNode->CreateComponent<OrbitCamera>();
	camera->SetMaxAngX(0);
	camera->SetZoomFactor(1000.0f);
	camera->RegisterWithInputManager(GetInputManager());	
	scene.SetCamera(camera);	
	//camera->SetOrthographicProjectionParameters(-10, 10, -10, 10, -2, 20);
	//camera->GetNode()->Rotate(vec3(PI * 0.25f, 0.0f, 0.0f));
	engine->GetLuaState()->ExecuteFile(resourceCache->GetResource<LuaFile>("../Samples/FireCubeTest/test.lua"));
	childNode = root->CreateChild("Model");	
	StaticModel *staticModel = childNode->CreateComponent<StaticModel>(resourceCache->GetResource<Mesh>("scene.3ds"));	
	childNode->CreateComponent<LuaScript>()->CreateObject("Rotator");	
	//childNode->Scale(vec3(0.05f));
	//childNode->Move(vec3(0, 0.0251f, 0));
	childNode = root->CreateChild("Model");
	AnimatedModel *animatedModel = childNode->CreateComponent<AnimatedModel>(rr);	
	childNode->Scale(vec3(0.05f));

	/*childNode = root->CreateChild("Particles");
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
	light->SetColor(vec4(1, 1, 1, 1));	
	light->SetCastShadow(true);
	childNode->Rotate(vec3(PI * 0.25f, 0.0f, 0.0f));
		
	scene.SetFogColor(vec3(44, 80, 222) / 255.0f);
	//renderer->SetCurrentRenderPath(resourceCache->GetResource<RenderPath>("RenderPaths/ForwardGrayscale.xml"));
	text = engine->GetUI()->GetRoot()->CreateChild<UIText>();
	text->SetFontFace(resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18));	
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
	Frame frame(engine, &scene);
	frame.Render(renderer);

	//frame.RenderDebugGeometry(engine->GetDebugRenderer());
}

void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
