#include <sstream>

#include <FireCube.h>
using namespace FireCube;

#include "app.h"
App app;

App::App() : scene(engine), scene2(engine)
{

}

App::~App()
{
	delete material;
	delete fbo;
	delete plane;
}
bool App::Prepare()
{
    Filesystem::AddSearchPath("../Assets/Textures");
    Filesystem::AddSearchPath("../Assets/Models");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
    SetTitle("FBO Example");
    fontFace = engine->GetResourcePool()->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(10);
	root = scene2.GetRootNode();
	camera2 = root->CreateComponent<Camera>();
	scene2.SetCamera(camera2);
	Node *lightNode = root->CreateChild("LightNode");
	Light *light = lightNode->CreateComponent<Light>();   
    light->SetDiffuseColor(vec4(0.8f, 0.8f, 0.8f, 1.0f));
    light->SetSpecularColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));	
    light->SetLightType(DIRECTIONAL);
    
	node = root->CreateChild("Mesh");
	node->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourcePool()->GetResource<Mesh>("../Assets/Models/1.3ds"));
	node->Move(vec3(1.3f, 0, -3));    	
	node2 = root->CreateChild("Mesh2");
	node2->CreateComponent<StaticModel>()->CreateFromMesh(engine->GetResourcePool()->GetResource<Mesh>("../Assets/Models/teapot2.3ds"));	    
    node2->Move(vec3(-1.3f, 0, -3));

	mainRoot = scene.GetRootNode();
	Node *cameraNode = mainRoot->CreateChild("cameraNode");
	camera = cameraNode->CreateComponent<Camera>();
	cameraNode->Move(vec3(0, 0, 1.5f));
	scene.SetCamera(camera);
	Node *node3 = mainRoot->CreateChild("Plane");
	StaticModel *staticModel = node3->CreateComponent<StaticModel>();
	material = new Material(engine);
	material->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	material->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(0));
	material->SetTechnique(engine->GetResourcePool()->GetResource<Technique>("./Techniques/DiffuseMap.xml"));
	plane = GeometryGenerator::GeneratePlane(engine, vec2(2, 2));
	staticModel->AddRenderablePart(plane, material);	
	staticModel->SetBoundingBox(BoundingBox(vec3(-2, -2, -2), vec3(2, 2, 2)));	
	node3->Rotate(vec3(-(float)PI / 2.0f, 0, 0));
	scene.SetAmbientColor(vec3(1, 1, 1));
	
    fbo = new FrameBuffer(engine);
    fbo->Create(128, 128);
    fbo->AddDepthBuffer();
    fbo->AddRenderTarget(0);
    fbo->GetRenderTarget(0)->GenerateMipMaps();	
	material->SetTexture(TEXTURE_UNIT_DIFFUSE, fbo->GetRenderTarget(0));

    if (fbo->IsValid() == false)
    {
        Logger::Write(Logger::LOG_ERROR, "Error: couldn't create FBO.\n");
        return false;
    }
		
    return true;
}
void App::Update(float time)
{
	node->Rotate(vec3(time * 1.5f, time * 1.5f, 0));
	node2->Rotate(vec3(time * 1.5f, time * 1.5f, 0));	
}
void App::Render(float time)
{
	mat4 projection;
	projection.GeneratePerspective(90.0f, (float) fbo->GetWidth() / (float) fbo->GetHeight(), 0.1f, 1000.0f);
	camera2->SetProjectionMatrix(projection);    
	
    renderer->UseFrameBuffer(fbo);	
    renderer->Clear(vec4(0.2f, 0.4f, 0.4f, 1.0f), 1.0f);    
	scene2.Render(renderer);
    projection.GenerateOrthographic(0, (float) fbo->GetWidth(), (float) fbo->GetHeight(), 0, 0, 1);	
	renderer->RenderText(fontFace, projection, vec3(0, 0, 0), vec4(1, 1, 1, 1), "FBO Test.");

	
    renderer->RestoreFrameBuffer();
    renderer->SetViewport(0, 0, GetWidth(), GetHeight());        
    renderer->Clear(vec4(0.4f, 0.4f, 0.4f, 1.0f), 1.0f);    
    fbo->GetRenderTarget(0)->GenerateMipMaps();
	projection.GeneratePerspective(90.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 1000.0f);
	camera->SetProjectionMatrix(projection);
	scene.Render(renderer);

	projection.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);
	
    std::ostringstream oss;
    oss << "FPS:" << GetFps();
	renderer->RenderText(fontFace, projection, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
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