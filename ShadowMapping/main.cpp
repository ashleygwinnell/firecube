#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "app.h"
App app;
#include <fstream>
#include <cmath>
vec3 lastPos;
int main(int argc, char *argv[])
{
    if (!app.Initialize())
        return 0;
    app.Run();
    return 0;
}
bool App::Init()
{
    Filesystem::AddSearchPath("../Assets/Textures");
    SetTitle(string("Shadow Mapping Test Application"));
    GetInputManager().AddInputListener(this);
    GetInputManager().AddMapping(MOUSE_AXIS_X_RELATIVE, "mouseX");
    GetInputManager().AddMapping(MOUSE_AXIS_Y_RELATIVE, "mouseY");
    GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
    GetInputManager().AddMapping(KEY_MOUSE_LEFT_BUTTON, STATE, "Rotate");
    GetInputManager().AddMapping(KEY_MOUSE_RIGHT_BUTTON, STATE, "Zoom");
    node = LoadMesh("../Assets/Models/scene.3ds");
    node->SetLighting(false);
    font = Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf", 18);
    plain = ProgramPtr(new Program);
    plain->Create(Renderer::GetShaderManager().Create("plain.vert"), Renderer::GetShaderManager().Create("plain.frag"));
    TechniquePtr t(new Technique);
    t->LoadShader("shadowMap.vert");
    t->LoadShader("shadowMap.frag");
    Renderer::AddTechnique("shadowMap", t);
    programUniformsList.SetIntValue("shadowMap", 1);
    program = ProgramPtr(new Program);
    program->Create(Renderer::GetShaderManager().Create("1.vert"), Renderer::GetShaderManager().Create("1.frag"));
    vBuffer = BufferPtr(new Buffer);
    uvBuffer = BufferPtr(new Buffer);
    vBuffer->Create();
    uvBuffer->Create();
    vec2 vb[4] = {vec2(0, 0), vec2(0, 150), vec2(150, 150), vec2(150, 0)};
    vec2 uvb[4] = {vec2(0, 1), vec2(0, 0), vec2(1, 0), vec2(1, 1)};

    vBuffer->LoadData(&vb[0], sizeof(vec2) * 4, STATIC);
    uvBuffer->LoadData(&uvb[0], sizeof(vec2) * 4, STATIC);
    fb = FrameBufferPtr(new FrameBuffer);
    fb->Create(1024, 1024);
    fb->AddDepthBufferTexture();

    if (!fb->IsValid())
        return false;

    defaultCamera = NodeObserverCameraPtr(new NodeObserverCamera);
    defaultCamera->SetTarget(node);
    defaultCamera->SetMinAngX((float) (-PI / 2.0));
    defaultCamera->SetMaxAngX((float) (PI / 2.0));
    defaultCamera->SetDistance(3.0f);
    lightCamera = CameraPtr(new Camera);
    orthographicCamera = CameraPtr(new Camera);
    return true;
}
void App::Update(float t)
{
}
void App::SetupLight()
{
    static float t = 0.0f;
    vec3 lightPos = vec3(cos(t) * 6, 4, sin(t) * 6);
    lightProj.GeneratePerspective(45, (float)fb->GetWidth() / (float)fb->GetHeight(), 1.0f, 20.0f);
    lightModelview.LookAt(lightPos, vec3(0, 0, 0), vec3(0, 1, 0));
    lightCamera->SetProjectionMatrix(lightProj);
    lightCamera->SetPosition(lightPos);
    lightCamera->LookAt(vec3(0, 0, 0), vec3(0, 1, 0));
    programUniformsList.SetVec3Value("lightPos", lightPos);
    t += 0.01f;

    mat4 m = mat4::identity;
    m.Translate(0.5f, 0.5f, 0.5f);
    m.Scale(0.5f, 0.5f, 0.5f);
    m *= lightProj;
    m *= lightModelview;
    //Renderer::SetTextureMatrix(m, 1);
}
void App::RenderShadowMap()
{
    SetupLight();
    glPolygonOffset(1.3f, 0.0f);
    glEnable(GL_POLYGON_OFFSET_FILL);
    Renderer::UseFrameBuffer(fb);
    node->SetProgram(plain);
    Renderer::Clear(vec4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f);
    Renderer::UseCamera(lightCamera);
    Renderer::Render(app.node);
    glDisable(GL_POLYGON_OFFSET_FILL);
}
void App::Render(float t)
{
    mat4 projection;
    projection.GeneratePerspective(90.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 1000.0f);
    defaultCamera->SetProjectionMatrix(projection);
    projection.GenerateOrthographic(0, (float) app.GetWidth(), (float) app.GetHeight(), 0, 0, 1);
    orthographicCamera->SetProjectionMatrix(projection);

    RenderShadowMap();
    Renderer::RestoreFrameBuffer();
    Renderer::SetViewport(0, 0, app.GetWidth(), app.GetHeight());
    Renderer::Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);
    Renderer::UseTexture(fb->GetDepthBuffer(), 1);
    Renderer::UseCamera(defaultCamera);
    //Renderer::Render(app.node, "shadowMap", programUniformsList);

    Renderer::UseCamera(orthographicCamera);
    //RenderDepth();
    ostringstream oss;
    oss << "FPS:" << app.GetFps();
    Renderer::RenderText(app.font, vec3(0, (float) app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
    if (input.IsActionTriggered("Close"))
        Close();
    if (input.IsStateOn("Rotate"))
        defaultCamera->Rotate(vec3(t * -input.GetValue("mouseY") / 2.0f, t * -input.GetValue("mouseX") / 2.0f, 0.0f));

    if (input.IsStateOn("Zoom"))
        defaultCamera->Zoom(t * -input.GetValue("mouseY") * 2.0f);
}
void App::RenderDepth()
{
    vBuffer->SetVertexAttribute(0, 2, 0, 0);
    uvBuffer->SetVertexAttribute(4, 2, 0, 0);
    Renderer::UseProgram(program);
    Renderer::UseTexture(fb->GetDepthBuffer(), 0);
    glTexParameteri(GL_TEXTURE_2D, 34892, GL_NONE);
    Renderer::RenderStream(QUADS, 4);
    glTexParameteri(GL_TEXTURE_2D, 34892, 34894);
}
