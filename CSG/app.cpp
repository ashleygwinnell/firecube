#include <sstream>

#include "FireCube.h"
using namespace FireCube;

#include "app.h"
#include "BSPTree.h"

bool App::Init()
{
	camera = NodeObserverCameraPtr(new NodeObserverCamera(GetInputManager()));
	camera->SetDistance(3.0f);
	camera->SetMaxAngX(3.14f / 2.0f);
	camera->SetMinAngX(-3.14f / 2.0f);
	orthographicCamera = CameraPtr(new Camera);
	font = Renderer::GetFontPool().Create("c:\\windows\\fonts\\arial.ttf", 18);
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");	

	MaterialPtr redMaterial(new Material);
	redMaterial->SetAmbientColor(vec3(0.0f, 0.0f, 0.0f));
	redMaterial->SetDiffuseColor(vec3(1.0f, 0.0f, 0.0f));
	redMaterial->SetSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	redMaterial->SetShininess(128.0f);

	MaterialPtr blueMaterial = redMaterial->Clone();
	blueMaterial->SetDiffuseColor(vec3(0.0f, 0.0f, 1.0f));
	
	GeometryPtr cube = GeometryGenerator::GenerateBox(vec3(1,1,1), redMaterial);
	GeometryPtr sphere = GeometryGenerator::GenerateSphere(0.65f, 16, 16, blueMaterial);			
	BSPTree bsp1, bsp2, out1, out2;
	bsp1.FromGeometry(cube);
	bsp2.FromGeometry(sphere);
	bsp1.Subtract(bsp2, out1, out2);		

	root = NodePtr(new Node);
	camera->SetTarget(root);
	NodePtr scene(new Node("csgScene"));
	root->AddChild(scene);
	GeometryNodePtr node = GeometryNodePtr(new GeometryNode("out1"));
	scene->AddChild(node);
	node->SetGeometry(out1.GetGeometry());
	node = GeometryNodePtr(new GeometryNode("out2"));
	scene->AddChild(node);
	node->SetGeometry(out2.GetGeometry());
	
	root->SetTechnique("default");	
	LightNodePtr lightNode = LightNodePtr(new LightNode("LightNode"));
	lightNode->GetLight().SetAmbientColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightNode->GetLight().SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightNode->GetLight().SetSpecularColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightNode->GetLight().SetType(DIRECTIONAL);	
	lightNode->Rotate(vec3(0.7f, 0.7f, 0));
	root->AddChild(lightNode);
	return true;
}

void App::Update(float time)
{
	
}

void App::Render(float time)
{		
	Renderer::Clear(vec4(0.7f, 0.7f, 0.7f, 1.0f), 1.0f);
	mat4 projection;
	projection.GeneratePerspective(60.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 1000.0f);
	camera->SetProjectionMatrix(projection);
	Renderer::UseCamera(camera);
	Renderer::Render(root);
	projection.GenerateOrthographic(0, (float) GetWidth(), (float) GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(projection);
	Renderer::UseCamera(orthographicCamera);
	std::ostringstream oss;
	oss << "FPS: " << GetFps() << std::endl 
		<< "Rendered triangles:" << Renderer::GetNumberOfPrimitivesRendered();
	Renderer::RenderText(font, vec3(0, 0, 0), vec4(1, 1, 1, 1), oss.str());
}

void App::HandleInput(float time, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
}

void main()
{
	App app;
	if (app.Initialize())
		app.Run();
}