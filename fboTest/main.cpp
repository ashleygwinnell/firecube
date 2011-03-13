#include <FireCube.h>
using namespace std;
using namespace FireCube;

#include "app.h"
App app;
bool App::Init()
{
    Application::AddSearchPath("../Assets/Textures");
    Application::AddSearchPath("../Assets/Models");
    SetTitle("FBO Example");
    font = Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf", 10);
    root = Node(new NodeResource("Root"));
    Light light;    
    light.SetAmbientColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));
    light.SetDiffuseColor(vec4(0.8f, 0.8f, 0.8f, 1.0f));
    light.SetSpecularColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
    light.SetType(DIRECTIONAL);
    root->AddLight(light);
    node = LoadMesh("../Assets/Models/1.3ds");
    root->AddChild(node);
    node->Move(vec3(1.3f, 0, -3));
    program = Program(new ProgramResource);
    program->Create(Renderer::GetShaderManager().Create("1.vert"), Renderer::GetShaderManager().Create("1.frag"));
    Renderer::UseProgram(program);
    program->SetUniform("tex", 0);
    node->SetProgram(program);
    vector<vec3> vb;
    vector<vec2> uvb;
    vb.push_back(vec3(-1, 1, 0));
    vb.push_back(vec3(-1, -1, 0));
    vb.push_back(vec3(1, -1, 0));
    vb.push_back(vec3(1, 1, 0));
    vb.push_back(vec3(-1, 1, 0));
    vb.push_back(vec3(1, 1, 0));
    vb.push_back(vec3(1, -1, 0));
    vb.push_back(vec3(-1, -1, 0));

    uvb.push_back(vec2(0, 1));
    uvb.push_back(vec2(0, 0));
    uvb.push_back(vec2(1, 0));
    uvb.push_back(vec2(1, 1));
    uvb.push_back(vec2(0, 1));
    uvb.push_back(vec2(1, 1));
    uvb.push_back(vec2(1, 0));
    uvb.push_back(vec2(0, 0));
    vBuffer = Buffer(new BufferResource);
    uvBuffer = Buffer(new BufferResource);
    vBuffer->Create();
    vBuffer->LoadData(&vb[0], sizeof(vec3)*vb.size(), STATIC);
    uvBuffer->Create();
    uvBuffer->LoadData(&uvb[0], sizeof(vec2)*uvb.size(), STATIC);
    node2 = LoadMesh("../Assets/Models/teapot2.3ds");
    root->AddChild(node2);
    node2->Move(vec3(-1.3f, 0, -3));
    fbo = FrameBuffer(new FrameBufferResource);
    fbo->Create(128, 128);
    fbo->AddDepthBuffer();
    fbo->AddRenderTarget(0);
    fbo->GetRenderTarget(0)->GenerateMipMaps();
    ang = 0;
    if (fbo->IsValid() == false)
    {
        Logger::Write(Logger::LOG_ERROR, "Error: couldn't create FBO.\n");
        return false;
    }
    return true;
}
void App::Update(float time)
{
    ang += time * 1.5f;
}
void App::Render(float time)
{
    mat4 t;
    node->Rotate(vec3(time * 1.5f, time * 1.5f, 0));
    node2->Rotate(vec3(time * 1.5f, time * 1.5f, 0));
    Renderer::UseFrameBuffer(fbo);
    Renderer::Clear(vec4(0.2f, 0.4f, 0.4f, 1.0f), 1.0f);
    Renderer::SetPerspectiveProjection(90.0f, 0.1f, 100);
    Renderer::Render(root);
    Renderer::SetOrthographicProjection();
    Renderer::SetModelViewMatrix(mat4());
    Renderer::RenderText(font, vec3(0, 0, 0), vec4(1, 1, 1, 1), "FBO Test.");

    Renderer::RestoreFrameBuffer();
    Renderer::SetViewport(0, 0, GetWidth(), GetHeight());
    t.Identity();
    t.Translate(vec3(0, 0, -2.5f));
    t.RotateY(ang / 3.0f);
    Renderer::SetModelViewMatrix(t);
    Renderer::Clear(vec4(0.4f, 0.4f, 0.4f, 1.0f), 1.0f);
    Renderer::SetPerspectiveProjection(90, 0.1f, 100);
    Renderer::UseProgram(program);
    vBuffer->SetVertexStream(3);
    uvBuffer->SetTexCoordStream(0);
    fbo->GetRenderTarget(0)->GenerateMipMaps();
    Renderer::UseTexture(fbo->GetRenderTarget(0), 0);
    Renderer::RenderStream(QUADS, 8);

    Renderer::SetOrthographicProjection();
    Renderer::SetModelViewMatrix(mat4());
    ostringstream oss;
    oss << "FPS:" << GetFps();
    Renderer::RenderText(font, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float time)
{

}
int main(int argc, char *argv[])
{
    if (app.Initialize())
        app.Run();
}