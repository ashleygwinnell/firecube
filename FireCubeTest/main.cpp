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
void SetNormalMap(Node node,Texture texture)
{
	for (unsigned int i=0;i<node->GetGeometries().size();i++)
	{
		Geometry g=node->GetGeometries()[i];
		for (unsigned int j=0;j<g->GetMaterials().size();j++)
		{
			Material m=g->GetMaterials()[j];
			m->SetNormalTexture(texture);			
		}
	}
	for (unsigned int i=0;i<node->GetChildren().size();i++)
		SetNormalMap(node->GetChildren()[i],texture);
}
bool App::Init()
{
	Application::AddSearchPath("../Assets/Textures");
	SetTitle(string("FireCube Test Application"));		
	font=Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf",18);
	root=Node(new NodeResource("Root"));
	Node node;
	node=LoadMesh("../Assets/Models/teapot2.3ds");	
	node->SetTranslation(vec3(0,0,-4.0f));
	root->AddChild(node);
	node=Node(new NodeResource("LightNode"));
	Light light;
	light.SetType(DIRECTIONAL);
	light.SetAmbientColor(vec4(0.2f,0.2f,0.2f,1.0f));
	light.SetDiffuseColor(vec4(0.7f,0.7f,0.7f,1.0f));
	light.SetSpecularColor(vec4(0.2f,0.2f,0.2f,1.0f));	
	node->AddLight(light);
	root->AddChild(node);
	SetNormalMap(root,Renderer::GetTextureManager().Create("normalMap.jpg"));	
	return true;
}
void App::Update(float t)
{
	//root.GetChildren()[0].Rotate(vec3(0,t/2.0f,0));
}
void App::Render(float t)
{
	RenderQueue renderQueue;
	Renderer::Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(60.0f,0.1f,100.0f);	
	Renderer::SetModelViewMatrix(mat4());	
	//Renderer::Render(root);
	renderQueue.AddNode(root);
	Renderer::Render(renderQueue);
	Renderer::SetOrthographicProjection();	
	ostringstream oss;
	oss << "FPS:"<<app.GetFps();	
	Renderer::RenderText(app.font,vec3(0,(float)app.GetHeight()-20.0f,0.0f),vec4(1,1,1,1),oss.str());
}
void App::HandleInput(float t)
{
	static vec2 lastPos;
	vec2 pos;
	::POINT p;
	::GetCursorPos(&p);
	pos.x=(float)p.x;
	pos.y=(float)p.y;
	if (GetAsyncKeyState(1))	
		root->GetChildren()[0]->Rotate(vec3(lastPos.y-pos.y,lastPos.x-pos.x,0)*t);
	if (GetAsyncKeyState(2))	
		root->GetChildren()[1]->Rotate(vec3(lastPos.y-pos.y,lastPos.x-pos.x,0)*t);
	lastPos=pos;
}
