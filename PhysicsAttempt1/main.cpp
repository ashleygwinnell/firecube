#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "app.h"
#include "Grid3D.h"
#include "CollisionShape.h"
#include "RigidBody.h"
#include "Simulator.h"

App app;
CollisionShape cube;
vec3 lastPos;
vec3 rot(0, 0, 0), camPos(0, 0, 3.0f);
RigidBody body1;
RigidBody body2;
vector <CollisionInfo> collisions;
Simulator s;
int main(int argc, char *argv[])
{
    Filesystem::AddSearchPath("../Assets/Textures");
    if (!app.Initialize())
        return 0;
    app.SetTitle(string("Physics Test"));
    app.vShader = Renderer::GetShaderManager().Create("v.vert");
    app.pShader = Renderer::GetShaderManager().Create("p.frag");
    app.pShader2 = Renderer::GetShaderManager().Create("p2.frag");
    app.font = Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf", 18);
    app.program = Program(new ProgramResource);
    app.program->Create(app.vShader, app.pShader);
    app.program2 = Program(new ProgramResource);
    app.program2->Create(app.vShader, app.pShader2);
    app.model = LoadMesh("../Assets/Models/physcube.3ds");
    app.sphere = LoadMesh("../Assets/Models/sphere2.3ds");
    app.model->SetLighting(false);
    app.model->SetProgram(app.program);
    app.sphere->SetLighting(false);
    app.sphere->SetProgram(app.program);
    cube.FromNode(app.model, 20, 20, 20, 1.1f);
    body1.Init(app.model, &cube);
    body1.position.Set(-2, 0, 0);
    body2.Init(app.model, &cube);
    body2.position.Set(2, 0, 0);
    s.Add(&body1);
    s.Add(&body2);
    app.Run();
    return 0;
}
void App::Update(float t)
{
    s.Update(t);
}
void App::Render(float t)
{
    Renderer::SetPerspectiveProjection(60.0f, 0.1f, 100.0f);
    Renderer::Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);
    mat4 m;
    m.RotateX(rot.x);
    m.RotateY(rot.y);
    m.Translate(-camPos);
    Renderer::SetModelViewMatrix(m);
    Renderer::UseProgram(program);
    app.program->SetUniform("tex0", 0);
    s.Render(m);

    Renderer::SetOrthographicProjection();
    Renderer::SetModelViewMatrix(mat4());
    ostringstream oss;
    oss << "FPS:" << app.GetFps();
    Renderer::RenderText(font, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t)
{
    body1.CalculateWorldProperties();
    body2.CalculateWorldProperties();
    ::POINT p;
    vec3 m;
    ::GetCursorPos(&p);
    m.x = (float)p.x;
    m.y = (float)p.y;
    if (GetAsyncKeyState(1))
    {
        rot.x += t * (lastPos.y - m.y) / 2.0f;
        rot.y += t * (lastPos.x - m.x) / 2.0f;

        rot.x -= rot.x >= 2 * PI ? (float)(2 * PI) : 0;
        rot.x += rot.x < 0 ? (float)(2 * PI) : 0;
        rot.y -= rot.y >= 2 * PI ? (float)(2 * PI) : 0;
        rot.y += rot.y < 0 ? (float)(2 * PI) : 0;
    }
    if (GetAsyncKeyState(2))
    {
        vec3 v;
        v.FromAngles(rot.x, rot.y);
        camPos += v * (lastPos.y - m.y) * t;
    }
    lastPos = m;
}