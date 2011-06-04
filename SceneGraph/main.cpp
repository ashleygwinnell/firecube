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
    font = Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf", 18);
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
    mat->SetAmbientColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));
    mat->SetDiffuseColor(vec4(0.7f, 0.7f, 0.7f, 1.0f));
    mat->SetSpecularColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));
    mat->SetShininess(20.0f);
    mat->SetDiffuseTexture(Renderer::GetTextureManager().Create("earthmap1k.jpg"));
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
	Renderer::GetCamera()->SetProjectionMatrix(projection);    
    Renderer::Render(root);
    //root->RenderBoundingBox(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    Renderer::SetModelViewMatrix(mat4::identity);
    Renderer::SetOrthographicProjection();
    ostringstream oss;
    oss << "FPS:" << app.GetFps();
    Renderer::RenderText(app.font, vec3(0, (float)app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t)
{
}

