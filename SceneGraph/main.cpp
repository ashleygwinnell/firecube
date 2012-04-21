#include <FireCube.h>
using namespace std;
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
bool App::Init()
{
    Filesystem::AddSearchPath("../Assets/Textures");
    Filesystem::AddSearchPath("../Assets/Models");
    SetTitle("SceneGraph Test Application");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
    font = Renderer::GetFontPool().Create("c:\\windows\\fonts\\arial.ttf", 18);
    GeometryPtr lightMarker = GeometryGenerator::GenerateSphere(0.1f, 10, 10);
    root = NodePtr(new Node("Root"));
    Light l;
    l.SetAmbientColor(vec4(0.3f, 0.3f, 0.3f, 1));
    l.SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1));
    l.SetSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    l.SetType(FireCube::POINT);
    NodePtr nn(new Node("Ln"));
    nn->SetParent(root);
    NodePtr lightNode(new Node("LightNode1"));
    lightNode->SetParent(nn);
    lightNode->AddLight(l);
    lightNode->Move(vec3(0, 0, 4.0f));
    lightNode->SetLighting(false);
    lightNode->SetGeometry(lightMarker);

    MaterialPtr mat(new Material);
    mat->SetAmbientColor(vec3(0.3f, 0.3f, 0.3f));
    mat->SetDiffuseColor(vec3(0.7f, 0.7f, 0.7f));
    mat->SetSpecularColor(vec3(0.3f, 0.3f, 0.3f));
    mat->SetShininess(20.0f);
    mat->SetDiffuseTexture(Renderer::GetTexturePool().Create("earthmap1k.jpg"));
    NodePtr n(new Node("Earth"));
    n->SetParent(root);
    n->SetGeometry(GeometryGenerator::GenerateSphere(2.0f, 32, 32, mat));

    n = root->AddChild(LoadMesh("../Assets/Models/teapot.3ds"));
	n->SetName("Teapot");
    n->CreateHardNormals();
    n->Move(vec3(8, -2, 0));

    n = root->AddChild(LoadMesh("../Assets/Models/duck.dae"));
    n->SetName("Duck");
    n->Scale(vec3(0.03f, 0.03f, 0.03f));
    n->Move(vec3(-8, -4, 0));

    root->Move(vec3(0, 0, -10));
    root->SetTechnique("default");

	camera = CameraPtr(new Camera);
	orthographicCamera = CameraPtr(new Camera);
    return true;
}
void App::Update(float t)
{
    root->GetChild("Ln")->Rotate(vec3(0, 0.02f, 0));	
}
void App::Render(float t)
{
    Renderer::Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);
	mat4 projection;
    projection.GeneratePerspective(90.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 1000.0f);
	camera->SetProjectionMatrix(projection);    
	Renderer::UseCamera(camera);
    Renderer::Render(root);
    //root->RenderBoundingBox(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    projection.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(projection);
	Renderer::UseCamera(orthographicCamera);
    ostringstream oss;
    oss << "FPS:" << app.GetFps();
    Renderer::RenderText(app.font, vec3(0, (float)app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
}

