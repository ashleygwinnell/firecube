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

App::App() : scene(engine)
{

}

bool App::Prepare()
{	
	SetTitle(std::string("FireCube Test Application"));	
	GetInputManager().AddMapping(Key::ESCAPE, InputMappingType::ACTION, "Close");
	GetInputManager().AddMapping(Key::Q, InputMappingType::ACTION, "Test");
	SubscribeToEvent(Events::HandleInput, &App::HandleInput);
	
	root = scene.GetRootNode();
	Node *childNode = root->CreateChild("Camera");
	InputManager &inputManager = GetInputManager();
	camera = childNode->CreateComponent<OrbitCamera>(inputManager);	
	camera->SetMaxAngX(0);
	camera->SetZoomFactor(1000.0f);	

	root->CreateComponent<PhysicsWorld>()->SetGravity(vec3(0, -30, 0));

	Node *node, *node2;
	CollisionShape *collisionShape;
	RigidBody *rigidBody;
	
	node = root->CreateChild("LightNode");
	Light *light = node->CreateComponent<Light>();
	light->SetLightType(FireCube::LightType::DIRECTIONAL);
	light->SetColor(vec3(1.0f));
	light->SetCastShadow(true);
	node->Rotate(vec3(-PI * 0.25f, 0.0f, 0.0f));

	node = root->CreateChild();

	float radius = 1.0f;
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, radius, 6, 6), BoundingBox(vec3(-radius), vec3(radius)), resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));

	node2 = node->CreateChild();	
	node2->CreateComponent<StaticModel>(mesh);
	collisionShape = node2->CreateComponent<CollisionShape>();
	collisionShape->SetSphere(radius);
	node2->Move(vec3(0, 0.0f, 0)); 	

	rigidBody = node->CreateComponent<RigidBody>();
	rigidBody->SetMass(10.0f);
	node->Move(vec3(0, 10, 0));	

	for (unsigned int i = 0; i < 100; ++i)
	{
		node = root->CreateChild();

		node2 = node->CreateChild();
		node2->CreateComponent<StaticModel>(mesh);
		collisionShape = node2->CreateComponent<CollisionShape>();
		collisionShape->SetSphere(radius);
		node2->Move(vec3(0, 0.0f, 0));

		rigidBody = node->CreateComponent<RigidBody>();
		rigidBody->SetMass(10.0f);
		node->Move(vec3(1, 12 + i * 2.0f, 0));
	}

	//rigidBody->ApplyForce(vec3(100.0f), vec3(-0.5f, 0.0f, -0.5f));

	node = root->CreateChild();
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, vec2(10.0f)), BoundingBox(vec3(-5.0f), vec3(5.0f)), resourceCache->GetResource<Material>("Materials/TerrainNoTexture.xml"));
	node->CreateComponent<StaticModel>(mesh);
	node->Move(vec3(0, 0, 0));
	collisionShape = node->CreateComponent<CollisionShape>();
	collisionShape->SetPlane(Plane(vec3(0, 1, 0), 0));
	rigidBody = node->CreateComponent<RigidBody>();
	rigidBody->SetMass(0.0f);
		
	scene.SetFogColor(vec3(44, 80, 222) / 255.0f);	
	text = engine->GetUI()->GetRoot()->CreateChild<UIText>();
	text->SetFontFace(resourceCache->GetResource<Font>("c:\\windows\\fonts\\arial.ttf")->GenerateFontFace(18));	

	renderer->SetSceneView(0, new SceneView(engine, &scene, camera));

	return true;
}

void App::Update(float t)
{		
	std::ostringstream oss;
	oss << "Rendered triangles: " << renderer->GetNumberOfPrimitivesRendered() << std::endl << "FPS: " << GetFps();
	text->SetText(oss.str());	
}

void App::Render(float t)
{    			
	/*auto debugRenderer = engine->GetDebugRenderer();

	float radius = 0.1f;
	float height = 0.5f;
	vec3 initialPos(-5, 2.0f, -0.2f);
	vec3 targetPos(5.0f, 2.0f, -0.2f);
	vec3 sphereCenter(0.0f, 1.3f, 0.0f);
	float sphereRadius = 1.0f;


	debugRenderer->AddSphere(sphereCenter, sphereRadius, 16, 16, vec3(0, 1, 0));		

	CollisionResult result;

	auto p0 = initialPos - vec3(0, height * 0.5f, 0);
	auto p1 = initialPos + vec3(0, height * 0.5f, 0);
	CollisionUtils::SweepSphereCapsule(sphereCenter, sphereRadius, p0, p1, radius, -(targetPos - initialPos).Normalized(), (targetPos - initialPos).Length(), result);

	vec3 newEndPoint;
	if (result.collisionFound)
	{
	newEndPoint = (targetPos - initialPos).Normalized() * result.nearestDistance + initialPos;
	debugRenderer->AddSphere(result.nearestIntersectionPoint, 0.05f, 16, 16, vec3(0));
	debugRenderer->AddLine(result.nearestIntersectionPoint, result.nearestIntersectionPoint + result.nearestNormal * 1.0f, vec3(0, 0, 1));
	}
	else
	{
	newEndPoint = targetPos;
	}

	debugRenderer->AddCapsule(p0, p1, radius, 4, 8, vec3(0, 1, 0));
	debugRenderer->AddCapsule(newEndPoint - vec3(0, height * 0.5f, 0), newEndPoint + vec3(0, height * 0.5f, 0), radius, 4, 8, vec3(0, 1, 0));

	debugRenderer->AddLine(initialPos, targetPos, vec3(1, 1, 0));
	engine->GetDebugRenderer()->Render(camera);*/
}

void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
