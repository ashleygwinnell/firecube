#include <sstream>

#include <FireCube.h>

using namespace FireCube;
#include "app.h"
#include <cmath>
App app;
int main(int argc, char *argv[])
{
	if (!app.Initialize(1024, 768, 0, false))
		return 0;
	app.Run();
	return 0;
}

App::App() : scene(engine)
{

}

App::~App()
{		
	delete lightMarker;
	delete earthMesh;	
}
bool App::Prepare()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	Filesystem::AddSearchPath("../Assets/Models");
	SetTitle("SceneGraph Test Application");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	fontFace = engine->GetResourceCache()->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18);	
	
	root = scene.GetRootNode();
	Node *cameraNode = root->CreateChild("cameraNode");	
	camera = cameraNode->CreateComponent<Camera>();
	cameraNode->Move(vec3(0, 0, 10));	
	scene.SetCamera(camera);

	Node *nn = root->CreateChild("Ln");	
	Node *lightNode = nn->CreateChild("LightNode1");	
	Light *light = lightNode->CreateComponent<Light>();		
	light->SetColor(vec4(1.0f, 1.0f, 1.0f, 1));	
	light->SetLightType(FireCube::LightType::POINT);
	lightNode->Move(vec3(0, 0, 4.0f));	
	StaticModel *staticModel = lightNode->CreateComponent<StaticModel>();
	lightMarker = new Mesh(engine);
	Material *mat = engine->GetResourceCache()->GetResource<Material>("./Materials/TerrainNoTexture.xml")->Clone();
	mat->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("./Techniques/Unlit.xml"));
	lightMarker->AddGeometry(GeometryGenerator::GenerateSphere(engine, 0.1f, 10, 10), mat);
	lightMarker->SetBoundingBox(BoundingBox(vec3(-0.05f), vec3(0.05f)));
	staticModel->CreateFromMesh(lightMarker);	

	earthMesh = new Mesh(engine);
	mat = new Material(engine);
	mat->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(0.3f, 0.3f, 0.3f, 1.0f));
	mat->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(0.7f, 0.7f, 0.7f, 1.0f));
	mat->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(0.3f, 0.3f, 0.3f, 1.0f));
	mat->SetParameter(PARAM_MATERIAL_SHININESS, 20.0f);
	mat->SetTexture(TextureUnit::DIFFUSE, engine->GetResourceCache()->GetResource<Texture>("earthmap1k.jpg"));
	mat->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("./Techniques/DiffuseMap.xml"));

	Node *n = root->CreateChild("Earth");
	staticModel = n->CreateComponent<StaticModel>();	
	earthMesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, 2.0f, 32, 32), mat);
	earthMesh->SetBoundingBox(BoundingBox(vec3(-2.0f), vec3(2.0f)));
	staticModel->CreateFromMesh(earthMesh);		
			
	Node *n2 = root->CreateChild("Teapot");
	n2->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourceCache()->GetResource<Mesh>("../Assets/Models/teapot.3ds"));
	n2->Move(vec3(8, -2, 0));

	n2 = root->CreateChild("Duck");	
	n2->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourceCache()->GetResource<Mesh>("../Assets/Models/duck.dae"));
	n2->Scale(vec3(0.03f, 0.03f, 0.03f));
	n2->Move(vec3(-8, -4, 0));	
	
	scene.SetFogColor(vec3(0.2f, 0.2f, 0.6f));
	return true;
}

void App::Update(float t)
{	
	root->GetChild("Ln")->Rotate(vec3(0, 1.0f, 0) * t);	
	root->GetChild("Earth")->Rotate(vec3(0, 0.1f, 0) * t);	
}
void App::Render(float t)
{		
	scene.Render(renderer);
	
	mat4 projection;
	projection.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);	
	std::ostringstream oss;
	oss << "FPS:" << app.GetFps();
	renderer->RenderText(fontFace, projection, vec3(0, (float)app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
}

