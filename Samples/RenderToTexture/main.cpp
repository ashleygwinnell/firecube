#include <sstream>

#include <FireCube.h>
using namespace FireCube;

#include "app.h"
App app;

App::App() : scene(engine), scene2(engine)
{

}

bool App::Prepare()
{
    Filesystem::AddSearchPath("../Assets/Textures");
    Filesystem::AddSearchPath("../Assets/Models");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
    SetTitle("RenderToTexture Example");
    
	renderSurface = renderer->GetRenderSurface(128, 128, RenderSurfaceType::COLOR);
	Node *root = scene2.GetRootNode();
	camera2 = root->CreateComponent<Camera>();
	camera2->SetPerspectiveProjectionParameters(90.0f, (float)renderSurface->GetWidth() / (float)renderSurface->GetHeight(), 0.1f, 1000.0f);
	scene2.SetCamera(camera2);
	scene2.SetFogColor(vec3(0.2f, 0.4f, 0.4f));
	Node *lightNode = root->CreateChild("LightNode");
	Light *light = lightNode->CreateComponent<Light>();   
	light->SetColor(vec4(0.8f, 0.8f, 0.8f, 1.0f));    
	light->SetLightType(LightType::DIRECTIONAL);
    
	node = root->CreateChild("Mesh");
	node->CreateComponent<StaticModel>()->CreateFromMesh(resourceCache->GetResource<Mesh>("../Assets/Models/1.3ds"));
	node->Move(vec3(1.3f, 0, -3));    	
	node2 = root->CreateChild("Mesh2");
	node2->CreateComponent<StaticModel>()->CreateFromMesh(resourceCache->GetResource<Mesh>("../Assets/Models/teapot2.3ds"));
    node2->Move(vec3(-1.3f, 0, -3));	

	root = scene.GetRootNode();
	Node *cameraNode = root->CreateChild("cameraNode");
	camera = cameraNode->CreateComponent<Camera>();
	camera->SetPerspectiveProjectionParameters(90.0f, (float)GetWidth() / (float)GetHeight(), 0.1f, 1000.0f);
	cameraNode->Move(vec3(0, 0, 1.5f));
	scene.SetCamera(camera);
	Node *node3 = root->CreateChild("Plane");
	StaticModel *staticModel = node3->CreateComponent<StaticModel>();
	material = new Material(engine);
	material->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	material->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(0));
	material->SetTechnique(resourceCache->GetResource<Technique>("./Techniques/DiffuseMap.xml"));
	Geometry *plane = GeometryGenerator::GeneratePlane(engine, vec2(2, 2));
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(plane, material);
	mesh->SetBoundingBox(BoundingBox(vec3(-2), vec3(2)));
	staticModel->CreateFromMesh(mesh);	
	
	node3->Rotate(vec3(-(float)PI / 2.0f, 0, 0));
	scene.SetAmbientColor(vec3(1, 1, 1));
	scene.SetFogColor(vec3(0.4f, 0.4f, 0.4f));	
	
	material->SetTexture(TextureUnit::DIFFUSE, renderSurface->GetLinkedTexture());
		
	text = engine->GetUI()->GetRoot()->CreateChild<UIText>();
	text->SetFontFace(resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18));

    return true;
}
void App::Update(float time)
{
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	text->SetText(oss.str());

	node->Rotate(vec3(time * 1.5f, time * 1.5f, 0));
	node2->Rotate(vec3(time * 1.5f, time * 1.5f, 0));	
}
void App::Render(float time)
{	
	mat4 projection;	
	Frame frame(engine, &scene);
	Frame frame2(engine, &scene2);
	
	frame2.SetRenderTarget(renderSurface);
	frame2.Render(renderer);	
	renderSurface->GetLinkedTexture()->GenerateMipMaps();

	frame.Render(renderer);	
}
void App::HandleInput(float time, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
}
int main(int argc, char *argv[])
{
    if (app.Initialize())
        app.Run();
}