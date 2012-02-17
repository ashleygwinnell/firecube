#include <FireCube.h>
using namespace std;
using namespace FireCube;

#include "app.h"
App app;
bool App::Init()
{
    Filesystem::AddSearchPath("../Assets/Textures");
    Filesystem::AddSearchPath("../Assets/Models");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
    SetTitle("FBO Example");
    font = Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf", 10);
    root = NodePtr(new Node("Root"));
    Light light;
    light.SetAmbientColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));
    light.SetDiffuseColor(vec4(0.8f, 0.8f, 0.8f, 1.0f));
    light.SetSpecularColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
    light.SetType(DIRECTIONAL);
    root->AddLight(light);
    node = LoadMesh("../Assets/Models/1.3ds");
    root->AddChild(node);
    node->Move(vec3(1.3f, 0, -3));    	
    node2 = LoadMesh("../Assets/Models/teapot2.3ds");
    root->AddChild(node2);
    node2->Move(vec3(-1.3f, 0, -3));

	mainRoot = NodePtr(new Node);
	mainRoot->SetGeometry(GeometryGenerator::GeneratePlane(vec2(2,2)));    
	mainRoot->Rotate(vec3(-(float) PI / 2.0f, 0, 0));
	mainRoot->SetLighting(false);	

    fbo = FrameBufferPtr(new FrameBuffer);
    fbo->Create(128, 128);
    fbo->AddDepthBuffer();
    fbo->AddRenderTarget(0);
    fbo->GetRenderTarget(0)->GenerateMipMaps();
	mainRoot->GetGeometry()->GetMaterial()->SetDiffuseTexture(fbo->GetRenderTarget(0));

    if (fbo->IsValid() == false)
    {
        Logger::Write(Logger::LOG_ERROR, "Error: couldn't create FBO.\n");
        return false;
    }
	camera = CameraPtr(new Camera);
	orthographicCamera = CameraPtr(new Camera);
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
	camera->SetProjectionMatrix(projection);    
	camera->SetPosition(vec3(0 ,0 ,0));
	camera->SetRotation(vec3(0, 0, 0));	
	Renderer::UseCamera(camera);
    Renderer::UseFrameBuffer(fbo);	
    Renderer::Clear(vec4(0.2f, 0.4f, 0.4f, 1.0f), 1.0f);    
    Renderer::Render(root);
    projection.GenerateOrthographic(0, (float) fbo->GetWidth(), (float) fbo->GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(projection);
	Renderer::UseCamera(orthographicCamera);
    Renderer::RenderText(font, vec3(0, 0, 0), vec4(1, 1, 1, 1), "FBO Test.");

	
    Renderer::RestoreFrameBuffer();
    Renderer::SetViewport(0, 0, GetWidth(), GetHeight());    
    camera->SetPosition(vec3(0, 0, 1.5f));    
    Renderer::Clear(vec4(0.4f, 0.4f, 0.4f, 1.0f), 1.0f);
    Renderer::UseCamera(camera);
    fbo->GetRenderTarget(0)->GenerateMipMaps();
    Renderer::Render(mainRoot);

	projection.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(projection);
    Renderer::UseCamera(orthographicCamera);
    ostringstream oss;
    oss << "FPS:" << GetFps();
    Renderer::RenderText(font, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
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