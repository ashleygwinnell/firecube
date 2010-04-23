#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <queue>
using namespace std;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
#include <FireCube.h>
using namespace FireCube;

#include "app.h"
App app;
bool App::Init()
{
	Application::AddSearchPath("../Media/Textures");
	Application::AddSearchPath("../Media/Models");
	SetTitle("Lua FBO Example");
	font=Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf",10);	
	model=mm.Create("1.3ds");
	program=Program(new ProgramResource);
	program2=Program(new ProgramResource);
	program->Create(Renderer::GetShaderManager().Create("1.vshader"),Renderer::GetShaderManager().Create("1.fshader"));
	Renderer::UseProgram(program);
	program->SetUniform("tex",0);
	model->SetProgram(program);
	vector<vec3> vb;
	vector<vec2> uvb;
	vb.push_back(vec3(-1,1,0));
	vb.push_back(vec3(-1,-1,0));
	vb.push_back(vec3(1,-1,0));
	vb.push_back(vec3(1,1,0));
	vb.push_back(vec3(-1,1,0));
	vb.push_back(vec3(1,1,0));
	vb.push_back(vec3(1,-1,0));
	vb.push_back(vec3(-1,-1,0));

	uvb.push_back(vec2(0,1));
	uvb.push_back(vec2(0,0));
	uvb.push_back(vec2(1,0));
	uvb.push_back(vec2(1,1));
	uvb.push_back(vec2(0,1));
	uvb.push_back(vec2(1,1));
	uvb.push_back(vec2(1,0));
	uvb.push_back(vec2(0,0));
	vBuffer=Buffer(new BufferResource);
	uvBuffer=Buffer(new BufferResource);
	vBuffer->Create();
	vBuffer->LoadData(&vb[0],sizeof(vec3)*vb.size(),STATIC);
	uvBuffer->Create();
	uvBuffer->LoadData(&uvb[0],sizeof(vec2)*uvb.size(),STATIC);
	program2->Create(Renderer::GetShaderManager().Create("v.vshader"),Renderer::GetShaderManager().Create("p.fshader"));
	model2=mm.Create("teapot2.3ds");
	model2->SetProgram(program2);	
	fbo=FrameBuffer(new FrameBufferResource);
	fbo->Create(128,128);
	fbo->AddDepthBuffer();
	fbo->AddRenderTarget(0);
	fbo->GetRenderTarget(0)->GenerateMipMaps();
	ang=0;
	if (fbo->IsValid()==false)
	{
		Logger::Write("Error: couldn't create FBO.\n");
		return false;
	}	
	return true;
}
void App::Update(float time)
{
	ang+=time*1.5f;
}
void App::Render(float time)
{
	mat4 t;
	mat4 t2;
	t.Translate(vec3(1.3f,0,-3));
	t.RotateX(ang);
	t.RotateY(ang);
	t2.Translate(vec3(-1.3f,0,-3));
	t2.RotateX(ang);
	t2.RotateY(ang);
	Renderer::UseFrameBuffer(fbo);
	Renderer::Clear(vec4(0.2f,0.4f,0.4f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,100);
	Renderer::SetModelViewMatrix(t);
	Renderer::Render(model);
	Renderer::SetModelViewMatrix(t2);
	Renderer::Render(model2);
	Renderer::SetOrthographicProjection();
	Renderer::SetModelViewMatrix(mat4());
	Renderer::RenderText(font,vec3(0,0,0),vec4(1,1,1,1),"FBO Test.");

	Renderer::RestoreFrameBuffer();
	Renderer::SetViewport(0,0,GetWidth(),GetHeight());
	t.Identity();
	t.Translate(vec3(0,0,-2.5f));
	t.RotateY(ang/3.0f);
	Renderer::SetModelViewMatrix(t);
	Renderer::Clear(vec4(0.4f,0.4f,0.4f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90,0.1f,100);
	Renderer::UseProgram(program);
	vBuffer->SetVertexStream(3);
	uvBuffer->SetTexCoordStream(0);
	fbo->GetRenderTarget(0)->GenerateMipMaps();
	Renderer::UseTexture(fbo->GetRenderTarget(0),0);
	Renderer::RenderStream(QUADS,8);

	Renderer::SetOrthographicProjection();
	Renderer::SetModelViewMatrix(mat4());
	ostringstream oss;
	oss << "FPS:" << GetFps();
	Renderer::RenderText(font,vec3(0,0,0),vec4(1,1,1,1),oss.str());
}
void App::HandleInput(float time)
{

}

int main(int argc, char *argv[])
{
	if (app.Initialize())
		app.Run();
}