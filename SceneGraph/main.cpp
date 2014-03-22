#include <sstream>

#include <FireCube.h>

using namespace FireCube;
#include "app.h"
#include <cmath>
App app;
int main(int argc, char *argv[])
{
	if (!app.Initialize(1024, 768, 32, 0, false))
		return 0;
	app.Run();
	return 0;
}
bool App::Prepare()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	Filesystem::AddSearchPath("../Assets/Models");
	SetTitle("SceneGraph Test Application");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
	fontFace = engine->GetResourcePool()->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18);
	GeometryPtr lightMarker = GeometryPtr(GeometryGenerator::GenerateSphere(engine, 0.1f, 10, 10));
	root = NodePtr(new Node(engine, "Root"));
	camera = CameraPtr(new Camera);
	orthographicCamera = CameraPtr(new Camera);
	scene.SetRootNodeAndCamera(root, camera);

	NodePtr nn = root->CreateChild("Ln");	
	NodePtr lightNode = nn->CreateChild("LightNode1");	
	Light *light = lightNode->CreateComponent<Light>();		
	light->SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1));
	light->SetSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light->SetLightType(FireCube::POINT);
	lightNode->Move(vec3(0, 0, 4.0f));	
	StaticModel *staticModel = lightNode->CreateComponent<StaticModel>();
	MaterialPtr mat = engine->GetResourcePool()->GetResource<Material>("./Materials/TerrainNoTexture.xml")->Clone();
	mat->SetTechnique(engine->GetResourcePool()->GetResource<Technique>("./Techniques/Unlit.xml"));
	staticModel->AddRenderablePart(lightMarker, mat);
	staticModel->SetBoundingBox(BoundingBox(vec3(-0.05f), vec3(0.05f)));	

	mat = MaterialPtr(new Material(engine));
	mat->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(0.3f, 0.3f, 0.3f, 1.0f));
	mat->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(0.7f, 0.7f, 0.7f, 1.0f));
	mat->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(0.3f, 0.3f, 0.3f, 1.0f));
	mat->SetParameter(PARAM_MATERIAL_SHININESS, 20.0f);
	mat->SetTexture(TEXTURE_UNIT_DIFFUSE,  engine->GetResourcePool()->GetResource<Texture>("earthmap1k.jpg"));
	mat->SetTechnique(engine->GetResourcePool()->GetResource<Technique>("./Techniques/DiffuseMap.xml"));

	NodePtr n = root->CreateChild("Earth");
	staticModel = n->CreateComponent<StaticModel>();
	staticModel->AddRenderablePart(GeometryPtr(GeometryGenerator::GenerateSphere(engine, 2.0f, 32, 32)), mat);
	staticModel->SetBoundingBox(BoundingBox(vec3(-1.0f), vec3(1.0f)));
			
	NodePtr n2 = root->CreateChild("Teapot");
	n2->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourcePool()->GetResource<Mesh>("../Assets/Models/teapot.3ds"));	
	n2->Move(vec3(8, -2, 0));

	n2 = root->CreateChild("Duck");	
	n2->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourcePool()->GetResource<Mesh>("../Assets/Models/duck.dae"));			
	n2->Scale(vec3(0.03f, 0.03f, 0.03f));
	n2->Move(vec3(-8, -4, 0));

	root->Move(vec3(0, 0, -10));	
	
	return true;
}

void App::Update(float t)
{
	root->GetChild("Ln")->Rotate(vec3(0, 1.0f, 0) * t);	
	root->GetChild("Earth")->Rotate(vec3(0, 0.1f, 0) * t);	
}
void App::Render(float t)
{
	renderer->Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);
	mat4 projection;
	projection.GeneratePerspective(90.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 1000.0f);
	camera->SetProjectionMatrix(projection);	
	
	scene.Render(renderer);
			
	projection.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(projection);	
	std::ostringstream oss;
	oss << "FPS:" << app.GetFps();
	renderer->RenderText(fontFace, orthographicCamera, vec3(0, (float)app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
}

