#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "app.h"
App app;
int main(int argc, char *argv[])
{	
	if (!app.Initialize())
        return 0;
    app.Run();
    return 0;
}
void SetNormalMap(NodePtr node, TexturePtr texture)
{	
	if (node->GetGeometry())
		node->GetGeometry()->GetMaterial()->SetNormalTexture(texture);	

    for (unsigned int i = 0; i < node->GetChildren().size(); i++)
        SetNormalMap(node->GetChildren()[i], texture);
}
bool App::Init()
{
    Filesystem::AddSearchPath("../Assets/Textures");
    SetTitle(string("FireCube Test Application"));
	font = Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf", 18);
    root = NodePtr(new Node("Root"));
    NodePtr node;
    node = LoadMesh("../Assets/Models/teapot2.3ds");    
    root->AddChild(node);
    node = NodePtr(new Node("LightNode"));
    Light light;
    light.SetType(DIRECTIONAL);
    light.SetAmbientColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
    light.SetDiffuseColor(vec4(0.7f, 0.7f, 0.7f, 1.0f));
    light.SetSpecularColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
    node->AddLight(light);
    root->AddChild(node);
    SetNormalMap(root, Renderer::GetTextureManager().Create("normalMap.jpg"));
	
	camera = Renderer::GetCamera();
	camera->SetPosition(vec3(0.0f, 0.0f, 4.0f));
    return true;
}
void App::Update(float t)
{
    
}
void App::Render(float t)
{    
    Renderer::Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);
	mat4 projection;
	projection.GeneratePerspective(60.0f, (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);	
	camera->SetProjectionMatrix(projection);		
    Renderer::Render(root);		
    Renderer::SetOrthographicProjection();
	Renderer::SetModelViewMatrix(mat4::identity);
    ostringstream oss;
    oss << "FPS:" << app.GetFps();
    Renderer::RenderText(app.font, vec3(0, (float)app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t)
{
    static vec2 lastPos;
    vec2 pos;
    pos=GetCursorPos();    
    if (GetAsyncKeyState(1))
        root->GetChildren()[0]->Rotate(vec3(lastPos.y - pos.y, lastPos.x - pos.x, 0)*t);
    if (GetAsyncKeyState(2))
        root->GetChildren()[1]->Rotate(vec3(lastPos.y - pos.y, lastPos.x - pos.x, 0)*t);
    lastPos = pos;
}
