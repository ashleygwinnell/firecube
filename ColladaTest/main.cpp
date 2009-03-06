#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <Windows.h>
#include <gl/GL.h>
#include <dae.h>
#include <dom/domCOLLADA.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
#include "ColladaLoader.h"

App app;
vec3 rot(0,0,-15);
int main(int argc, char *argv[])
{
	if (!app.Initialize())
		return 0;
	app.Run();
	return 0;
}
bool App::Init()
{
	model=Model(new ModelResource);
	program=Program(new ProgramResource);
	program->Create(Renderer::GetShaderManager()->Create("plainColor.vshader"),Renderer::GetShaderManager()->Create("plainColor.fshader"));	
	cout << program->GetInfoLog();
	SetTitle("ColladaTest");
	font=Renderer::GetFontManager()->Create("c:\\windows\\fonts\\arial.ttf",18);
	if (!font)
		return false;
	ColladaLoader loader(model.get());
	loader.Load("c.dae");
	model->SetProgram(program);
	//return false;	
	return true;
}
void App::Render(float time)
{
	Renderer::Clear(vec4(0.7f,0.7f,0.7f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,500.0f);
	mat4 t;
	t.Translate(vec3(0,0,rot.z));
	t.RotateX(rot.x);
	t.RotateY(rot.y);
	Renderer::SetModelViewMatrix(t);
	Renderer::UseProgram(program);
	vector<int> textured;
	vector<int> textureId;
	for (DWORD i=0;i<model->material.size();i++)
		for (DWORD t=0;t<MAX_TEXTURES;t++)
			if ((model->material[i]->texture[t]) && (model->material[i]->texture[t]->IsValid()))
				textured.push_back(true);
			else
				textured.push_back(false);
	for (DWORD t=0;t<MAX_TEXTURES;t++)	
		textureId.push_back(t);		
	program->SetUniform("texture",textureId);
	program->SetUniform("textured",textured);
	Renderer::Render(model);
	Renderer::SetOrthographicProjection();
	Renderer::SetModelViewMatrix(mat4());
	ostringstream oss;
	oss << "FPS: " << app.GetFps();
	Renderer::RenderText(font,vec2(0,0),vec4(1,1,1,1),oss.str());
}
void App::Update(float time)
{

}
void App::HandleInput(float time)
{
	static vec3 lastPos;
	POINT p;
	vec3 m;
	GetCursorPos(&p);
	m.x=(float)p.x;
	m.y=(float)p.y;
	if (GetAsyncKeyState(1))
	{
		rot.x+=time*(lastPos.y-m.y)/4.0f;
		rot.y+=time*(lastPos.x-m.x)/4.0f;

		rot.x-=rot.x>=2*PI ? (float)(2*PI) : 0;
		rot.x+=rot.x<0 ? (float)(2*PI) : 0;
		rot.y-=rot.y>=2*PI ? (float)(2*PI) : 0;
		rot.y+=rot.y<0 ? (float)(2*PI) : 0;
	}
	if (GetAsyncKeyState(2))
		rot.z+=time*(lastPos.y-m.y)*5.0f;

	lastPos=m;
}