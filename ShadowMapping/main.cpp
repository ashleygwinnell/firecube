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
App app;
#include <fstream>
#include <cmath>
vec3 lastPos;
vec3 rot(0,0,-3);
int main(int argc, char *argv[])
{	
	if (!app.Initialize())
		return 0;		
	app.Run();			
	return 0;
}
bool App::Init()
{
	Application::AddSearchPath("../Media/Textures");
	SetTitle(string("Shadow Mapping Test Application"));	
	model=app.modelManager.Create("../Media/Models/scene.3ds");	
	font=Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf",18);
	plain=Program(new ProgramResource);
	shadowMap=Program(new ProgramResource);
	program=Program(new ProgramResource);
	plain->Create(Renderer::GetShaderManager().Create("plain.vshader"),Renderer::GetShaderManager().Create("plain.fshader"));
	shadowMap->Create(Renderer::GetShaderManager().Create("shadowMap.vshader"),Renderer::GetShaderManager().Create("shadowMap.fshader"));
	program->Create(Renderer::GetShaderManager().Create("1.vshader"),Renderer::GetShaderManager().Create("1.fshader"));
	fb=FrameBuffer(new FrameBufferResource);
	fb->Create(1024,1024);
	fb->AddDepthBufferTexture();
	
	if (!fb->IsValid())
		return false;
	return true;
}
void App::Update(float t)
{	
}
void App::SetupLight()
{
	static float t=0.0f;	
	vec3 lightPos=vec3(cos(t)*6,4,sin(t)*6);
	lightProj.GeneratePerspective(45,(float)fb->GetWidth()/(float)fb->GetHeight(),1.0f,20.0f);
	lightModelview.LookAt(lightPos,vec3(0,0,0),vec3(0,1,0));			
	Renderer::UseProgram(shadowMap);
	shadowMap->SetUniform("lightPos",lightPos);
	t+=0.01f;

	mat4 m;	
	m.Translate(0.5f,0.5f,0.5f);
	m.Scale(0.5f,0.5f,0.5f);		
	m*=lightProj;
	m*=lightModelview;
	Renderer::SetTextureMatrix(m,1);
}
void App::RenderShadowMap()
{	
	SetupLight();
	glPolygonOffset(1.3f,0.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	Renderer::UseFrameBuffer(fb);
	Renderer::SetGlobalProgram(plain);
	Renderer::Clear(vec4(0.0f,0.0f,0.0f,1.0f),1.0f);
	Renderer::SetProjectionMatrix(lightProj);
	Renderer::SetModelViewMatrix(lightModelview);
	Renderer::UseProgram(Program(new ProgramResource));
	Renderer::Render(app.model);
	Renderer::SetGlobalProgram(Program());	
	glDisable(GL_POLYGON_OFFSET_FILL);	
}
void App::Render(float t)
{
	RenderShadowMap();
	Renderer::RestoreFrameBuffer();
	Renderer::SetViewport(0,0,app.GetWidth(),app.GetHeight());
	Renderer::Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,100.0f);	
	Renderer::RestoreFrameBuffer();
	mat4 m;	
	m.Translate(vec3(0,0,rot.z));
	m.RotateX(rot.x);
	m.RotateY(rot.y);		
	Renderer::SetModelViewMatrix(m);		
	Renderer::SetGlobalProgram(shadowMap);	
	shadowMap->SetUniform("shadowMap",1);	
	Renderer::UseTexture(fb->GetDepthBuffer(),1);
	Renderer::Render(app.model);
	Renderer::SetGlobalProgram(Program());	
	Renderer::SetModelViewMatrix(mat4());
	Renderer::SetOrthographicProjection();
	RenderDepth();
	ostringstream oss;
	oss << "FPS:"<<app.GetFps();	
	Renderer::RenderText(app.font,vec2(0,(float)app.GetHeight()-20.0f),vec4(1,1,1,1),oss.str());
}
void App::HandleInput(float t)
{
	POINT p;
	vec3 m;	
	GetCursorPos(&p);	
	m.x=(float)p.x;
	m.y=(float)p.y;	
	if (GetAsyncKeyState(1))
	{
		rot.x+=t*(lastPos.y-m.y)/2.0f;
		rot.y+=t*(lastPos.x-m.x)/2.0f;		
		
		rot.x-=rot.x>=2*PI ? (float)(2*PI) : 0;
		rot.x+=rot.x<0 ? (float)(2*PI) : 0;
		rot.y-=rot.y>=2*PI ? (float)(2*PI) : 0;
		rot.y+=rot.y<0 ? (float)(2*PI) : 0;				
	}
	if (GetAsyncKeyState(2))	
		rot.z+=t*(lastPos.y-m.y)*2.0f;

	lastPos=m;	
}
void App::RenderDepth()
{
	Buffer vBuffer(new BufferResource);
	Buffer uvBuffer(new BufferResource);	
	vec2 vb[4]={vec2(0,0),vec2(0,150),vec2(150,150),vec2(150,0)};
	vec2 uvb[4]={vec2(0,1),vec2(0,0),vec2(1,0),vec2(1,1)};
	vBuffer->Create();
	vBuffer->LoadData(&vb[0],sizeof(vec2)*4,STATIC);
	uvBuffer->Create();
	uvBuffer->LoadData(&uvb[0],sizeof(vec2)*4,STATIC);
	vBuffer->SetVertexStream(2);
	uvBuffer->SetTexCoordStream(0);
	Renderer::UseProgram(program);
	Renderer::UseTexture(fb->GetDepthBuffer(),0);
	glTexParameteri(GL_TEXTURE_2D, 34892, GL_NONE);	
	Renderer::RenderStream(QUADS,4);
	glTexParameteri(GL_TEXTURE_2D, 34892, 34894);		
}
