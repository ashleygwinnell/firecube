#include <sstream>

#include <FireCube.h>

using namespace FireCube;
#include "app.h"

App app;
int main(int argc, char *argv[])
{
	Filesystem::SetCoreDataFolder("../../FireCube");
	Filesystem::SetAssetsFolder("../../Assets");

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
	SetTitle("Terrain");	
	GetInputManager().AddMapping(AnalogInput::MOUSE_AXIS_X_RELATIVE, "mouseX");
	GetInputManager().AddMapping(AnalogInput::MOUSE_AXIS_Y_RELATIVE, "mouseY");
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	GetInputManager().AddMapping(Key::W, InputMappingType::STATE, "Forward");
	GetInputManager().AddMapping(Key::S, InputMappingType::STATE, "Backward");
	GetInputManager().AddMapping(Key::A, InputMappingType::STATE, "Left");
	GetInputManager().AddMapping(Key::D, InputMappingType::STATE, "Right");
	GetInputManager().AddMapping(Key::W, InputMappingType::STATE, "ForwardFast", KeyModifier::SHIFT);
	GetInputManager().AddMapping(Key::S, InputMappingType::STATE, "BackwardFast", KeyModifier::SHIFT);
	GetInputManager().AddMapping(Key::A, InputMappingType::STATE, "LeftFast", KeyModifier::SHIFT);
	GetInputManager().AddMapping(Key::D, InputMappingType::STATE, "RightFast", KeyModifier::SHIFT);
	GetInputManager().AddMapping(Key::LEFT, InputMappingType::STATE, "RotateLeft");
	GetInputManager().AddMapping(Key::RIGHT, InputMappingType::STATE, "RotateRight");
	GetInputManager().AddMapping(Key::UP, InputMappingType::STATE, "RotateUp");
	GetInputManager().AddMapping(Key::DOWN, InputMappingType::STATE, "RotateDown");
	GetInputManager().AddMapping(Key::MOUSE_LEFT_BUTTON, InputMappingType::STATE, "RotateBoth");
	SubscribeToEvent(Events::HandleInput, &App::HandleInput);

	root = scene.GetRootNode();
	
	cameraNode = root->CreateChild();
	camera = cameraNode->CreateComponent<Camera>();	
	cameraNode->SetTranslation(vec3(0, 2, 0));
	camera->SetPerspectiveProjectionParameters(60.0f, (float)GetWidth() / (float)GetHeight(), 0.1f, 200.0f);
	
	Node *node = root->CreateChild();
	node->Move(vec3(5, 5, 5));	
	node->CreateComponent<StaticModel>()->CreateFromMesh(resourceCache->GetResource<Mesh>("Models/teapot.3ds"));

	node = root->CreateChild("Terrain");
	terrain = node->CreateComponent<Terrain>();
	terrain->SetPatchSize(64);
	terrain->SetVerticesSpacing(vec3(1.0f, 50.0f, 1.0f));
	terrain->SetGenerateHardNormals(true);
	terrain->CreateFromHeightMap(resourceCache->GetResource<Image>("Textures/heightmap.bmp"));
	terrain->SetMaterial(resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));
		
	Node *lightNode = root->CreateChild("Light");
	Light *light = lightNode->CreateComponent<Light>();
	light->SetLightType(FireCube::LightType::DIRECTIONAL);
	light->SetColor(vec3(1.0f));	
	lightNode->Rotate(vec3(-1, -1, 0));

	scene.SetFogEnabled(true);
	scene.SetFogParameters(vec3(0, 0.01f, 0));
	scene.SetFogColor(vec3(0.30f, 0.42f, 0.95f));
		
	text = engine->GetUI()->GetRoot()->CreateChild<UIText>();
	text->SetFontFace(resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18));

	renderer->SetSceneView(0, new SceneView(engine, &scene, camera));

	return true;
}

void App::Update(float time)
{	
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	text->SetText(oss.str());

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
	quat qx(ang.x, 0, 0), qy(0, ang.y, 0), qz(0, 0, angSpeed.z);
	cameraNode->SetRotation(qy * qx * qz);
}

void App::HandleInput(float time, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
	if (input.IsStateOn("RotateBoth"))
	{
		angSpeed.x -= input.GetValue("mouseY") * time * 0.1f;
		angSpeed.y -= input.GetValue("mouseX") * time * 0.1f;
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
		angSpeed.x += time * 0.3f;
	if (input.IsStateOn("RotateUp"))
		angSpeed.x -= time * 0.3f;
	
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