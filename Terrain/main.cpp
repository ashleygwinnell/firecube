#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "Frustum.h"
#include "Terrain.h"
#include "QuadTree.h"
#include "app.h"

App app;
vec3 pos(0,2,0);
vec3 speed;
float angx,angy=(float)PI;
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
	terrain.GenerateTerrain("heightmap2.bmp",vec3(512.0f,50.0f,512.0f),vec2(1.0f,1.0f));	
	qt.Initialize();
	//qt.Init(vec2((float)terrain.GetWidth(),(float)terrain.GetHeight()),vec2(512.0f,512.0f));
	//qt.Build(16.0f,1);
	//qt.Save("heightmap4.bin");
	qt.Load("heightmap2.bin");
	return true;	
}
void App::Update(float time)
{	
}
void App::Render(float time)
{	
	Renderer::SetPerspectiveProjection(90.0f,0.1f,200.0f);
	Renderer::Clear(vec4(0.30f,0.42f,0.95f,1.0f),1.0f);
	mat4 t;	
	bool cubeRendered=false;
	t.RotateX(angx);
	t.RotateY(angy);
	t.Translate(-pos);	
	Renderer::SetModelViewMatrix(t);
	frustum.ExtractFrustum();	
	terrain.PrepareRender();	
	int n=qt.Render(frustum);			
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
		angx+=-(curPos.y-lastPos.y)*time*0.5f;
		angy+=-(curPos.x-lastPos.x)*time*0.5f;
	}
	vec3 dir;
	float scale=0.2f;
	if (GetAsyncKeyState(VK_SHIFT))
		scale=1.0f;
	dir.FromAngles(angx,angy);
	if (GetAsyncKeyState('W'))
		speed+=dir*time*2.0f*scale;		
	if (GetAsyncKeyState('S'))
		speed-=dir*time*2.0f*scale;		
	vec3 strafe=Cross(dir,vec3(0,1,0)).Normalize();
	if (GetAsyncKeyState('A'))
		pos-=strafe*time*2.0f*scale;
	if (GetAsyncKeyState('D'))
		pos+=strafe*time*2.0f*scale;
	float height=terrain.GetHeight(pos.x,pos.z);
	if (pos.y-height<0.75f) pos.y=height+0.75f;	
	pos+=speed;
	speed=speed*0.9f;
	lastPos=curPos;	
}
