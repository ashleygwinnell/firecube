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

	childNode = root->CreateChild("Model");	
	/*StaticModel *staticModel = childNode->CreateComponent<StaticModel>(resourceCache->GetResource<Mesh>("scene.3ds"));	
	childNode->Scale(vec3(0.05f));
	childNode->Move(vec3(0, 0.0251f, 0));*/
	AnimatedModel *animatedModel = childNode->CreateComponent<AnimatedModel>(rr);	
	childNode->Scale(vec3(0.05f));

	/*childNode = root->CreateChild("Particles");
	childNode->CreateComponent<ParticleEmitter>();*/

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
	light->SetLightType(FireCube::LightType::POINT);
	light->SetColor(vec4(1, 1, 1, 1));	
	childNode->Move(vec3(0, 1, 0));
		
	fontFace = resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18);

	scene.SetFogColor(vec3(44, 80, 222) / 255.0f);
	//renderer->SetCurrentRenderPath(resourceCache->GetResource<RenderPath>("RenderPaths/ForwardGrayscale.xml"));
	return true;
}
void App::Update(float t)
{		
	//root->GetChild("LightNode")->Move(vec3(0.2f, 0.0f, 0.2f) * t);
}
void App::Render(float t)
{    		
	scene.Render(renderer);

	scene.RenderDebugGeometry(engine->GetDebugRenderer());

	mat4 ortho;
	ortho.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);	
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	renderer->RestoreFrameBuffer();
	renderer->RenderText(fontFace, ortho, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
