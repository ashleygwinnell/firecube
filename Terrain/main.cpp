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

App::App() : ang(0, (float)PI, 0)
{

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

	font = Renderer::GetFontPool().Create("c:\\windows\\fonts\\arial.ttf", 18);		
	root = NodePtr(new Node("root"));
	node = LoadMesh("../Assets/Models/teapot.3ds");
	node->Move(vec3(5, 5, 5));
	node->CreateHardNormals();	
	root->AddChild(node);
	terrainNode = TerrainNodePtr(new TerrainNode);
	if (!terrainNode->GetTerrain().GenerateTerrain("../Assets/Textures/heightmap.bmp", vec3(512.0f, 50.0f, 512.0f), vec2(1.0f, 1.0f), false))
		return false;
	TexturePtr diffuseTexture = Renderer::GetTexturePool().Create("../Assets/Textures/diffuse.bmp");
	terrainNode->GetMaterial()->SetDiffuseTexture(diffuseTexture);
	
	root->AddChild(terrainNode);
	LightNodePtr lightNode(new LightNode);
	lightNode->GetLight().SetType(DIRECTIONAL);
	lightNode->GetLight().SetAmbientColor(vec4(0, 0, 0, 1));
	lightNode->GetLight().SetDiffuseColor(vec4(1, 1, 1, 1));
	lightNode->GetLight().SetSpecularColor(vec4(0, 0, 0, 1));
	lightNode->Rotate(vec3(1, 1, 0));
	root->AddChild(lightNode);
	root->SetFog(true);
	root->SetFogDensity(0.01f);
	root->SetFogColor(vec4(0.30f, 0.42f, 0.95f, 1.0f));	

	camera = CameraPtr(new Camera);
	camera->SetPosition(vec3(0, 2, 0));
	orthographicCamera = CameraPtr(new Camera);
	return true;
}

void App::Update(float time)
{	
	camera->Move(speed);
	ang += angSpeed;
	angSpeed *= 0.9f;
	vec3 pos = camera->GetPosition();
	float height = terrainNode->GetTerrain().GetHeight(vec2(pos.x, pos.z));	
	if (pos.y - height < 0.75f)
	{
		pos.y = height + 0.75f;
		vec3 n = terrainNode->GetTerrain().GetNormal(vec2(pos.x, pos.z));
		speed = speed - speed.Dot(n) * n * 2;
		camera->SetPosition(pos);
	}
	speed = speed * 0.9f;
}

void App::Render(float time)
{
	mat4 projection;
	Renderer::UseCamera(camera);
	projection.GeneratePerspective(60.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 200.0f);
	Renderer::GetCamera()->SetProjectionMatrix(projection);
	Renderer::Clear(vec4(0.30f, 0.42f, 0.95f, 1.0f), 1.0f);	
	Renderer::GetCamera()->SetRotation(vec3(ang.x, ang.y, angSpeed.z));
	
	Renderer::Render(root);
	
	Renderer::UseCamera(orthographicCamera);
	mat4 ortho;
	ortho.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(ortho);
	ostringstream oss;
	oss << "FPS:" << app.GetFps() << endl << "Rendered primitives: " << Renderer::GetNumberOfTrianglesRendered();
	Renderer::RenderText(font, vec3(0, 0, 0), vec4(1.0f, 1.0f, 1.0f, 1.0f), oss.str());    
}

void App::HandleInput(float time, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
	if (input.IsStateOn("RotateBoth"))
	{
		angSpeed.x += input.GetValue("mouseY") * time * 0.1f;
		angSpeed.y += input.GetValue("mouseX") * time * 0.1f;
	}
	if (input.IsStateOn("RotateLeft"))
	{
		angSpeed.y -= time * 0.3f;
		angSpeed.z -= time * 0.5f;
	}
	if (input.IsStateOn("RotateRight"))
	{
		angSpeed.y += time * 0.3f;
		angSpeed.z += time * 0.5f;
	}
	if (input.IsStateOn("RotateDown"))
		angSpeed.x -= time * 0.3f;
	if (input.IsStateOn("RotateUp"))
		angSpeed.x += time * 0.3f;
	
	vec3 dir = camera->GetViewMatrix().GetDirection();			
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