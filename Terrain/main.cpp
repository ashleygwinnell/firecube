#include <sstream>

#include <FireCube.h>

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

App::App() : ang(0, (float)PI, 0), scene(engine)
{

}

bool App::Prepare()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	SetTitle("Terrain");
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(MOUSE_AXIS_X_RELATIVE, "mouseX");
	GetInputManager().AddMapping(MOUSE_AXIS_Y_RELATIVE, "mouseY");
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	GetInputManager().AddMapping(Key::W, InputMappingType::STATE, "Forward");
	GetInputManager().AddMapping(Key::S, InputMappingType::STATE, "Backward");
	GetInputManager().AddMapping(Key::A, InputMappingType::STATE, "Left");
	GetInputManager().AddMapping(Key::D, InputMappingType::STATE, "Right");
	GetInputManager().AddMapping(Key::W, InputMappingType::STATE, "ForwardFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(Key::S, InputMappingType::STATE, "BackwardFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(Key::A, InputMappingType::STATE, "LeftFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(Key::D, InputMappingType::STATE, "RightFast", MODIFIER_SHIFT);
	GetInputManager().AddMapping(Key::LEFT, InputMappingType::STATE, "RotateLeft");
	GetInputManager().AddMapping(Key::RIGHT, InputMappingType::STATE, "RotateRight");
	GetInputManager().AddMapping(Key::UP, InputMappingType::STATE, "RotateUp");
	GetInputManager().AddMapping(Key::DOWN, InputMappingType::STATE, "RotateDown");
	GetInputManager().AddMapping(Key::MOUSE_LEFT_BUTTON, InputMappingType::STATE, "RotateBoth");

	fontFace = resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18);
	root = scene.GetRootNode();
	
	cameraNode = root->CreateChild();
	camera = cameraNode->CreateComponent<Camera>();	
	cameraNode->SetTranslation(vec3(0, 2, 0));
	camera->SetPerspectiveProjectionParameters(60.0f, (float)GetWidth() / (float)GetHeight(), 0.1f, 200.0f);
	scene.SetCamera(camera);

	Node *node = root->CreateChild();
	node->Move(vec3(5, 5, 5));	
	node->CreateComponent<StaticModel>()->CreateFromMesh(resourceCache->GetResource<Mesh>("../Assets/Models/teapot.3ds"));

	node = root->CreateChild("Terrain");
	terrain = node->CreateComponent<Terrain>();
	terrain->SetPatchSize(64);
	terrain->SetVerticesSpacing(vec3(1.0f, 50.0f, 1.0f));
	terrain->SetGenerateHardNormals(true);
	terrain->CreateFromHeightMap(resourceCache->GetResource<Image>("../Assets/Textures/heightmap.bmp"));
	terrain->SetMaterial(resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));
		
	Node *lightNode = root->CreateChild("Light");
	Light *light = lightNode->CreateComponent<Light>();
	light->SetLightType(FireCube::DIRECTIONAL);
	light->SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1));
	light->SetSpecularColor(vec4(0, 0, 0, 0));
	lightNode->Rotate(vec3(1, 1, 0));

	scene.SetFogEnabled(true);
	scene.SetFogParameters(vec3(0, 0.01f, 0));
	scene.SetFogColor(vec3(0.30f, 0.42f, 0.95f));
		
	return true;
}

void App::Update(float time)
{	
	cameraNode->Move(speed);
	ang += angSpeed;
	angSpeed *= 0.9f;
	vec3 pos = cameraNode->GetTranslation();
	float height = terrain->GetHeight(vec2(pos.x, pos.z));	
	if (pos.y - height < 0.75f)
	{
		pos.y = height + 0.75f;
		vec3 n = terrain->GetNormal(vec2(pos.x, pos.z));
		speed = speed - speed.Dot(n) * n * 2;
		cameraNode->SetTranslation(pos);
	}
	speed = speed * 0.9f;
}

void App::Render(float time)
{	
	renderer->Clear(vec4(0.30f, 0.42f, 0.95f, 1.0f), 1.0f);	
	mat4 rot = mat4::IDENTITY;
	rot.RotateY(ang.y);
	rot.RotateX(ang.x);	
	rot.RotateZ(angSpeed.z);
	cameraNode->SetRotation(rot);
	
	scene.Render(renderer);
		
	mat4 ortho;
	ortho.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);	
	std::ostringstream oss;
	oss << "FPS:" << app.GetFps() << std::endl << "Rendered primitives: " << renderer->GetNumberOfPrimitivesRendered();
	renderer->RenderText(fontFace, ortho, vec3(0, 0, 0), vec4(1), oss.str());
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
	
	mat4 inverseView = camera->GetViewMatrix();
	inverseView.Inverse();
	vec3 dir = inverseView.GetDirection().Normalized();	
	vec3 strafe = Cross(dir, vec3(0, 1, 0)).Normalized();

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