#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "QuadTree.h"
#include "Terrain.h"
#include "app.h"

App app;
vec3 pos(0, 2, 0);
vec3 speed;
vec3 angSpeed;
vec3 ang(0, (float)PI, 0);
::Frustum frustum;
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
    SetTitle("Terrain");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(MOUSE_AXIS_X_RELATIVE, "mouseX");
	GetInputManager().AddMapping(MOUSE_AXIS_Y_RELATIVE, "mouseY");
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
	GetInputManager().AddMapping(KEY_W, STATE, "Forward");
	GetInputManager().AddMapping(KEY_S, STATE, "Backward");
	GetInputManager().AddMapping(KEY_A, STATE, "Left");
	GetInputManager().AddMapping(KEY_D, STATE, "Right");
	GetInputManager().AddMapping(KEY_W, STATE, "ForwardFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(KEY_S, STATE, "BackwardFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(KEY_A, STATE, "LeftFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(KEY_D, STATE, "RightFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(KEY_LEFT, STATE, "RotateLeft");
	GetInputManager().AddMapping(KEY_RIGHT, STATE, "RotateRight");
	GetInputManager().AddMapping(KEY_UP, STATE, "RotateUp");
	GetInputManager().AddMapping(KEY_DOWN, STATE, "RotateDown");
	GetInputManager().AddMapping(KEY_MOUSE_LEFT_BUTTON, STATE, "RotateBoth");

    font = Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf", 18);
    program = ProgramPtr(new Program);
    program->Create(Renderer::GetShaderManager().Create("diffuseWithFog.vert"), Renderer::GetShaderManager().Create("diffuseWithFog.frag"));
    if (!terrain.GenerateTerrain("../Assets/Textures/heightmap.bmp", "../Assets/Textures/diffuse.bmp", vec3(512.0f, 50.0f, 512.0f), vec2(1.0f, 1.0f)))
        return false;
    root = NodePtr(new Node("root"));
    node = LoadMesh("../Assets/Models/teapot.3ds");
    node->Move(vec3(5, 5, 5));
    node->CreateHardNormals();
    node->SetLighting(false);
    node->SetProgram(program);
    root->AddChild(node);
    return true;
}
void App::Update(float time)
{
    pos += speed;
    ang += angSpeed;
    angSpeed *= 0.9f;
    float height = terrain.GetHeight(vec2(pos.x, pos.z));
    if (pos.y - height < 0.75f)
    {
        pos.y = height + 0.75f;
        vec3 n = terrain.GetNormal(vec2(pos.x, pos.z));
        speed = speed - speed.Dot(n) * n * 2;
    }
    speed = speed * 0.9f;
}
void App::Render(float time)
{
    Renderer::SetPerspectiveProjection(60.0f, 0.1f, 200.0f);
    Renderer::Clear(vec4(0.30f, 0.42f, 0.95f, 1.0f), 1.0f);
    mat4 t = mat4::identity;
    t.RotateX(ang.x);
    t.RotateY(ang.y);
    t.RotateZ(-angSpeed.z);
    t.Translate(-pos);
    Renderer::SetModelViewMatrix(t);
    Renderer::UseProgram(program);
    program->SetUniform("fogDensity", 0.01f);
    program->SetUniform("fogColor", vec4(0.30f, 0.42f, 0.95f, 1.0f));
    program->SetUniform("lightDir", t * vec3(1, -1, 1));

	mat4 mdl = Renderer::GetModelViewMatrix();
	mat4 proj = Renderer::GetProjectionMatrix();
    frustum.Extract(mdl, proj);
    DWORD n = terrain.Render(frustum);

    root->SetMatrixTransformation(t);
    node->Rotate(vec3(0.01f, 0, 0));
    Renderer::Render(root);

    Renderer::SetOrthographicProjection();
    Renderer::SetModelViewMatrix(mat4::identity);
    ostringstream oss, oss2;
    oss << "FPS:" << app.GetFps();
    Renderer::RenderText(font, vec3(0, 0, 0), vec4(1.0f, 1.0f, 1.0f, 1.0f), oss.str());
    oss2 << "Rendered triangles: " << n;
    Renderer::RenderText(font, vec3(0, 20, 0), vec4(1.0f, 1.0f, 0.0f, 1.0f), oss2.str());
}
void App::HandleInput(float time, const MappedInput &input)
{
    if (input.IsActionTriggered("Close"))
		Close();
    if (input.IsStateOn("RotateBoth"))
    {
        angSpeed.x += -input.GetValue("mouseY") * time * 0.1f;
        angSpeed.y += -input.GetValue("mouseX") * time * 0.1f;
    }
    if (input.IsStateOn("RotateLeft"))
    {
        angSpeed.y += time * 0.3f;
        angSpeed.z += time * 0.5f;
    }
    if (input.IsStateOn("RotateRight"))
    {
        angSpeed.y -= time * 0.3f;
        angSpeed.z -= time * 0.5f;
    }
    if (input.IsStateOn("RotateDown"))
        angSpeed.x -= time * 0.3f;
    if (input.IsStateOn("RotateUp"))
        angSpeed.x += time * 0.3f;
    vec3 dir;
    
	dir.FromAngles(ang.x, ang.y);
	vec3 strafe = Cross(dir, vec3(0, 1, 0)).Normalize();

    if (input.IsStateOn("Forward"))
        speed += dir * time * 0.4f;
    if (input.IsStateOn("Backward"))
        speed -= dir * time * 0.4f;    
    if (input.IsStateOn("Left"))
        speed -= strafe * time * 0.4f;
    if (input.IsStateOn("Right"))
        speed += strafe * time * 0.4f;    

	if (input.IsStateOn("ForwardFast"))
		speed += dir * time * 2.0f;
	if (input.IsStateOn("BackwardFast"))
		speed -= dir * time * 2.0f;	
	if (input.IsStateOn("LeftFast"))
		speed -= strafe * time * 2.0f;
	if (input.IsStateOn("RightFast"))
		speed += strafe * time * 2.0f;    
}