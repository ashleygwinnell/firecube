#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "tinyxml.h"
#include "ColladaLoader.h"

class App : public Application
{
public:
	virtual void Update(float t);	
	virtual void HandleInput(float t);
	virtual void Render(float t);	
	Node node;
};
void App::Update(float t)
{

}
void App::HandleInput(float t)
{

}
void App::Render(float t)
{
	Renderer::Clear(vec4(0.5f,0.5f,0.5f,1),1);	
	node.Rotate(vec3(0,t,0));	
	Renderer::Render(node);
}
void main()
{
	Application::AddSearchPath("../Media/Textures");
	App app;
	ColladaLoader l("../Media/Models/duck_triangulate.dae");
	l.Load();	
	app.Initialize();
	app.node=l.GenerateSceneGraph();
	app.node.Move(vec3(0,-70,-180));
	
	Light light;
	light.Create();
	light.SetAmbientColor(vec4(1,1,1,1));
	light.SetDiffuseColor(vec4(1,1,1,1));
	app.node.AddLight(light);

	Renderer::SetPerspectiveProjection(60.0f,0.1f,2000.0f);	
	app.Run();
	cout << "All OK." << endl;
}