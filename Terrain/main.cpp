#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "Frustum.h"
#include "QuadTree.h"
#include "Terrain.h"
#include "app.h"

App app;
vec3 pos(0,2,0);
vec3 speed;
vec3 angSpeed;
vec3 ang(0,(float)PI,0);
Frustum frustum;
int main(int argc, char *argv[])
{
	if (!app.Initialize())
		return 0;
	app.Run();
	return 0;
}
bool App::Init()
{	
	SetTitle("Terrain");
	font=Renderer::GetFontManager()->Create("c:\\windows\\fonts\\arial.ttf",18);
	program=Program(new ProgramResource);
	program->Create(Renderer::GetShaderManager()->Create("diffuseWithFog.vshader"),Renderer::GetShaderManager()->Create("diffuseWithFog.fshader"));
	if (!terrain.GenerateTerrain("heightmap.bmp","diffuse.bmp",vec3(512.0f,50.0f,512.0f),vec2(1.0f,1.0f)))
		return false;
	model=mm.Create("teapot.3ds");
	model->CreateHardNormals();
	model->SetProgram(program);
	return true;
}
void App::Update(float time)
{
	pos+=speed;
	ang+=angSpeed;
	angSpeed*=0.9f;
	float height=terrain.GetHeight(vec2(pos.x,pos.z));
	if (pos.y-height<0.75f)
	{
		pos.y=height+0.75f;
		vec3 n=terrain.GetNormal(vec2(pos.x,pos.z));
		speed=speed-speed.Dot(n)*n*2;
	}
	speed=speed*0.9f;
}
void App::Render(float time)
{
	static float lAng=0.0f;
	lAng+=0.01f;
	Renderer::SetPerspectiveProjection(60.0f,0.1f,200.0f);
	Renderer::Clear(vec4(0.30f,0.42f,0.95f,1.0f),1.0f);
	mat4 t;
	t.RotateX(ang.x);
	t.RotateY(ang.y);
	t.RotateZ(-angSpeed.z);
	t.Translate(-pos);
	Renderer::SetModelViewMatrix(t);
	Renderer::UseProgram(program);
	program->SetUniform("fogDensity",0.01f);
	program->SetUniform("fogColor",vec4(0.30f,0.42f,0.95f,1.0f));
	program->SetUniform("lightDir",t*vec3(1,-1,1));

	frustum.ExtractFrustum();
	DWORD n=terrain.Render(frustum);
	
	mat4 tt;
	tt.Translate(vec3(5,5,5));
	tt.RotateX(lAng);
	Renderer::SaveModelViewMatrix();
	Renderer::MultiplyModelViewMatrix(tt);
	Renderer::Render(model);
	Renderer::RestoreModelViewMatrix();
		
	Renderer::SetOrthographicProjection();
	Renderer::SetModelViewMatrix(mat4());
	ostringstream oss,oss2;
	oss << "FPS:" << app.GetFps();
	Renderer::RenderText(font,vec2(0,0),vec4(1.0f,1.0f,1.0f,1.0f),oss.str());
	oss2 << "Rendered triangles: " << n;
	Renderer::RenderText(font,vec2(0,20),vec4(1.0f,1.0f,0.0f,1.0f),oss2.str());
}
void App::HandleInput(float time)
{
	static vec2 lastPos;
	POINT p;
	GetCursorPos(&p);
	vec2 curPos((float)p.x,(float)p.y);
	if (GetAsyncKeyState(1))
	{
		angSpeed.x+=-(curPos.y-lastPos.y)*time*0.1f;
		angSpeed.y+=-(curPos.x-lastPos.x)*time*0.1f;
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		angSpeed.y+=time*0.3f;
		angSpeed.z+=time*0.5f;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		angSpeed.y-=time*0.3f;
		angSpeed.z-=time*0.5f;
	}
	if (GetAsyncKeyState(VK_UP))
		angSpeed.x-=time*0.3f;
	if (GetAsyncKeyState(VK_DOWN))
		angSpeed.x+=time*0.3f;
	vec3 dir;
	float scale=0.4f;
	if (GetAsyncKeyState(VK_SHIFT))
		scale=2.0f;
	dir.FromAngles(ang.x,ang.y);
	if (GetAsyncKeyState('W'))
		speed+=dir*time*scale;
	if (GetAsyncKeyState('S'))
		speed-=dir*time*scale;
	vec3 strafe=Cross(dir,vec3(0,1,0)).Normalize();
	if (GetAsyncKeyState('A'))
		speed-=strafe*time*scale;
	if (GetAsyncKeyState('D'))
		speed+=strafe*time*scale;
	lastPos=curPos;
}