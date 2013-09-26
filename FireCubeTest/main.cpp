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

bool App::Prepare()
{	
	SetTitle(std::string("FireCube Test Application"));
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
	
	root = NodePtr(new Node("Root"));		
	camera = NodeObserverCameraPtr(new NodeObserverCamera(GetInputManager()));	
	camera->SetTarget(root);
	camera->SetMaxAngX(0);
	camera->SetZoomFactor(1000.0f);
	viewport.SetRootNodeAndCamera(root, camera);	

	NodePtr childNode = root->CreateChild("Model");	
	StaticModel *staticModel = new StaticModel(engine);
	staticModel->LoadMesh("scene.3ds");	
	childNode->AddComponent(staticModel);	
	Terrain *terrain = new Terrain(engine);
	childNode->AddComponent(terrain);
	childNode->Scale(vec3(0.05f));
	Image image;
	image.Load("heightmap3.bmp");
	terrain->SetPatchSize(64);
	terrain->CreateHeightMap(image);
	MaterialPtr material(new Material(engine));
	material->Load("Materials/TerrainNoTexture.xml");
	material->SetAmbientColor(vec3(0));
	material->SetDiffuseColor(vec3(0.7f));
	terrain->SetMaterial(material);

	Light *light = new Light(engine);
	light->SetLightType(FireCube::POINT);
	light->SetDiffuseColor(vec4(1, 1, 1, 1));
	light->SetSpecularColor(vec4(0, 0, 0, 0));
	childNode = root->CreateChild("LightNode");	
	childNode->AddComponent(light);					
	childNode->Move(vec3(0, 1, 0));

	orthographicCamera = CameraPtr(new Camera);	
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
	viewport.Render(renderer);

	mat4 ortho;
	ortho.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(ortho);
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	renderer->RenderText(fontFace, orthographicCamera, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
