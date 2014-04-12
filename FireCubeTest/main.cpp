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
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
	
	root = scene.GetRootNode();
	camera = root->CreateComponent<NodeObserverCamera>();
	camera->SetTarget(root);
	camera->SetMaxAngX(0);
	camera->SetZoomFactor(1000.0f);
	camera->RegisterWithInputManager(GetInputManager());
	scene.SetCamera(camera);	

	Node *childNode = root->CreateChild("Model");	
	StaticModel *staticModel = childNode->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(resourcePool->GetResource<Mesh>("scene.3ds"));
	childNode->Scale(vec3(0.05f));
	childNode->Move(vec3(0, 0.0251f, 0));
	childNode = root->CreateChild("Terrain");	
	Terrain *terrain = childNode->CreateComponent<Terrain>();
	childNode->Scale(vec3(0.05f));
	Image *image = resourcePool->GetResource<Image>("heightmap3.bmp");	
	terrain->SetPatchSize(64);
	terrain->CreateFromHeightMap(image);
	terrain->SetMaterial(resourcePool->GetResource<Material>("Materials/TerrainNoTexture.xml"));	
	//terrain->SetMaterial(resourcePool->GetResource<Material>("Materials/DebugNormals.xml"));	

	childNode = root->CreateChild("LightNode");	
	Light *light = childNode->CreateComponent<Light>();
	light->SetLightType(FireCube::POINT);
	light->SetDiffuseColor(vec4(1, 1, 1, 1));
	light->SetSpecularColor(vec4(0, 0, 0, 0));	
	childNode->Move(vec3(0, 1, 0));
	
	font = resourcePool->GetResource<Font>("c:\\windows\\fonts\\arial.ttf");
	fontFace = font->GenerateFontFace(18);
	return true;
}
void App::Update(float t)
{
	//root->GetChild("LightNode")->Move(vec3(0.2f, 0.0f, 0.2f) * t);
}
void App::Render(float t)
{    
	renderer->Clear(vec4(44, 80, 222, 255) / 255.0f, 1.0f);	
	mat4 projection;
	projection.GeneratePerspective(60.0f, (float)GetWidth() / (float)GetHeight(), 0.1f, 500.0f);	
	camera->SetProjectionMatrix(projection);	
	scene.Render(renderer);

	mat4 ortho;
	ortho.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);	
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	renderer->RenderText(fontFace, ortho, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
