#include <sstream>

#include <FireCube.h>

using namespace FireCube;
#include "app.h"
#include <cmath>
App app;
int main(int argc, char *argv[])
{
	Filesystem::SetCoreDataFolder("../../FireCube");
	Filesystem::SetAssetsFolder("../../Assets");

	if (!app.Initialize(1024, 768, 0, false, false))
		return 0;
	app.Run();
	return 0;
}

App::App() : scene(engine)
{

}

App::~App()
{		
	
}
bool App::Prepare()
{	
	SetTitle("SceneGraph Test Application");	
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	SubscribeToEvent(Events::HandleInput, &App::HandleInput);
	
	root = scene.GetRootNode();
	Node *cameraNode = root->CreateChild("cameraNode");	
	camera = cameraNode->CreateComponent<Camera>();
	cameraNode->Move(vec3(0, 0, 10));	
	
	Node *nn = root->CreateChild("Ln");	
	Node *lightNode = nn->CreateChild("LightNode1");	
	Light *light = lightNode->CreateComponent<Light>();		
	light->SetColor(vec3(1.0f));	
	light->SetLightType(FireCube::LightType::POINT);
	lightNode->Move(vec3(0, 0, 4.0f));	
	StaticModel *staticModel = lightNode->CreateComponent<StaticModel>();
	SharedPtr<Mesh> mesh;
	mesh = new Mesh(engine);
	Material *mat = engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml")->Clone();
	mat->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));
	mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, 0.1f, 10, 10), BoundingBox(vec3(-0.05f), vec3(0.05f)), mat);
	staticModel->CreateFromMesh(mesh);

	mesh = new Mesh(engine);
	mat = new Material(engine);	
	mat->SetParameter(PARAM_MATERIAL_DIFFUSE_NAME, vec3(0.7f));
	mat->SetParameter(PARAM_MATERIAL_SPECULAR_NAME, vec3(0.3f));
	mat->SetParameter(PARAM_MATERIAL_SHININESS_NAME, 20.0f);
	mat->SetTexture(TextureUnit::DIFFUSE, engine->GetResourceCache()->GetResource<Texture2D>("Textures/earthmap1k.jpg"));
	mat->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseMap.xml"));

	Node *n = root->CreateChild("Earth");
	staticModel = n->CreateComponent<StaticModel>();	
	mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, 2.0f, 32, 32), BoundingBox(vec3(-2.0f), vec3(2.0f)), mat);
	staticModel->CreateFromMesh(mesh);
			
	Node *n2 = root->CreateChild("Teapot");
	n2->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourceCache()->GetResource<Mesh>("Models/teapot.3ds"));
	n2->Move(vec3(5, -2, 0));

	n2 = root->CreateChild("Duck");	
	n2->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourceCache()->GetResource<Mesh>("Models/duck.dae"));
	n2->Move(vec3(-5, -2, 0));	
	
	scene.SetFogColor(vec3(0.2f, 0.2f, 0.6f));

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

	root->GetChild("Ln")->Rotate(vec3(0, 1.0f, 0) * t);	
	root->GetChild("Earth")->Rotate(vec3(0, 0.1f, 0) * t);	
}
void App::Render(float t)
{		
	
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
}

