#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <queue>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
#include <cmath>
App app;
int main(int argc, char *argv[])
{	
	if (!app.Initialize(1024,768,32,0,false))
		return 0;		
	app.Run();			
	return 0;
}
bool App::Init()
{
	Application::AddSearchPath("../Media/Textures");
	Application::AddSearchPath("../Media/Models");
	SetTitle("SceneGraph Test Application");		
	font=Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf",18);
	Geometry lightMarker=GeometryGenerator::GenerateSphere(0.1f,10,10);
	root=Node("Root");	
	Light l(new LightResource);
	l->ambientColor.Set(0.3f,0.3f,0.3f,1);
	l->diffuseColor.Set(1.0f,1.0f,1.0f,1);
	l->specularColor.Set(1.0f,1.0f,1.0f,1.0f);
	l->type=FireCube::POINT;
	Node nn("Ln");
	nn.SetParent(root);
	Node lightNode("LightNode1");
	lightNode.SetParent(nn);
	lightNode.AddLight(l);
	lightNode.Move(vec3(0,0,4.0f));
	lightNode.SetLighting(false);
	lightNode.AddGeometry(lightMarker);

	Material mat(new MaterialResource);
	mat->ambient=vec4(0.3f,0.3f,0.3f,1.0f);
	mat->diffuse=vec4(0.7f,0.7f,0.7f,1.0f);
	mat->specular=vec4(0.3f,0.3f,0.3f,1.0f);
	mat->shininess=20.0f;
	mat->diffuseTexture=Renderer::GetTextureManager().Create("earthmap1k.jpg");
	Node n("Earth");
	n.SetParent(root);	
	n.AddGeometry(GeometryGenerator::GenerateSphere(2.0f,32,32,mat));	
	
	n=root.AddChild(LoadMesh("../Media/Models/teapot.3ds"));
	n.SetName("Teapot");
	n.CreateHardNormals();
	n.Move(vec3(8,-2,0));	
	
	n=root.AddChild(LoadMesh("../Media/Models/duck.dae"));	
	n.SetName("Duck");	
	n.Scale(vec3(0.03f,0.03f,0.03f));
	n.Move(vec3(-8,-4,0));	
	
	root.Move(vec3(0,0,-10));
	return true;
}
void App::Update(float t)
{
	root.GetChild("Ln").Rotate(vec3(0,0.02f,0));	
}
void App::Render(float t)
{
	Renderer::Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,1000.0f);	
	Renderer::SetModelViewMatrix(mat4());
	Renderer::Render(root);	
	//sg.Root()->RenderBoundingBox();
	Renderer::SetModelViewMatrix(mat4());
	Renderer::SetOrthographicProjection();	
	ostringstream oss;
	oss << "FPS:"<<app.GetFps();	
	Renderer::RenderText(app.font,vec2(0,(float)app.GetHeight()-20.0f),vec4(1,1,1,1),oss.str());
}
void App::HandleInput(float t)
{	
}
