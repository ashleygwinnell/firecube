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
	
	app.Run();
	return 0;
}

bool App::Init()
{
	SetTitle(string("Physics Test"));
	GetInputManager().AddInputListener(&app);
	GetInputManager().AddMapping(MOUSE_AXIS_X_RELATIVE, "mouseX");
	GetInputManager().AddMapping(MOUSE_AXIS_Y_RELATIVE, "mouseY");
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
	GetInputManager().AddMapping(KEY_MOUSE_LEFT_BUTTON, STATE, "Rotate");
	GetInputManager().AddMapping(KEY_MOUSE_RIGHT_BUTTON, STATE, "Forward");
	font = Renderer::GetFontPool().Create("c:\\windows\\fonts\\arial.ttf", 18);
	model = LoadMesh("../Assets/Models/physcube.3ds");
	sphere = LoadMesh("../Assets/Models/sphere2.3ds");
	model->SetLighting(false);
	sphere->SetLighting(false);
	cube.FromNode(app.model, 20, 20, 20, 1.1f);
	body1.Init(app.model, &cube);
	body1.position.Set(-2, 0, 0);
	body2.Init(app.model, &cube);
	body2.position.Set(2, 0, 0);
	s.Add(&body1);
	s.Add(&body2);
	camera = CameraPtr(new Camera);
	orthographicCamera = CameraPtr(new Camera);
	return true;
}

void App::Update(float t)
{
	s.Update(t);
}

void App::Render(float t)
{
	mat4 projection;
	projection.GeneratePerspective(90.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 1000.0f);
	camera->SetProjectionMatrix(projection);
	Renderer::UseCamera(camera);	
	Renderer::Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);
	mat4 m = mat4::identity;
	m.RotateX(rot.x);
	m.RotateY(rot.y);
	m.Translate(-camPos);	
	camera->SetRotation(vec3(rot.x, rot.y, 0));
	camera->SetPosition(camPos);
	s.Render(m);

	Renderer::UseCamera(orthographicCamera);
	mat4 orthographicProjection;
	orthographicProjection.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0 ,1);
	orthographicCamera->SetProjectionMatrix(orthographicProjection);		
	ostringstream oss;
	oss << "FPS:" << app.GetFps();
	Renderer::RenderText(font, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
}

void App::HandleInput(float t, const MappedInput &input)
{
	body1.CalculateWorldProperties();
	body2.CalculateWorldProperties();

	if (input.IsActionTriggered("Close"))
		Close();
	if (input.IsStateOn("Rotate"))
	{
		rot.x += t * input.GetValue("mouseY") / 2.0f;
		rot.y += t * input.GetValue("mouseX") / 2.0f;

		rot.x -= rot.x >= 2 * PI ? (float)(2 * PI) : 0;
		rot.x += rot.x < 0 ? (float)(2 * PI) : 0;
		rot.y -= rot.y >= 2 * PI ? (float)(2 * PI) : 0;
		rot.y += rot.y < 0 ? (float)(2 * PI) : 0;
	}
	if (input.IsStateOn("Forward"))
	{
		vec3 v;
		v.FromAngles(-rot.x, -rot.y);
		camPos += v * -input.GetValue("mouseY") * t;
	}    
}