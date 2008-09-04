#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
using namespace std;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
#include "Grid3D.h"
#include "CollisionShape.h"
#include "RigidBody.h"
#include "Simulator.h"

App app;
Model model,sphere;
CollisionShape cube;
Renderer renderer;
vec3 lastPos;
vec3 rot(0,0,0),camPos(0,0,3.0f);
Shader vShader,pShader,pShader2;
Program program,program2;
RigidBody body1;
RigidBody body2;
vector <CollisionInfo> collisions;
Simulator s;
Font font;
int main(int argc, char *argv[])
{
	if (!app.Initialize())
		return 0;	
	app.SetTitle(string("Physics Test"));
	vShader=app.GetContext().shaderManager->Create("v.vshader");
	pShader=app.GetContext().shaderManager->Create("p.pshader");	
	pShader2=app.GetContext().shaderManager->Create("p2.pshader");
	font=app.GetContext().fontManager->Create("c:\\windows\\fonts\\arial.ttf:18");
	program.Create(vShader,pShader);
	program2.Create(vShader,pShader2);		
	model=app.modelManager.Create("1.3ds");
	sphere=app.modelManager.Create("sphere2.3ds");
	cube.FromModel(model->Reduce(),20,20,20,1.1f);		
	body1.Init(model,&cube);
	body1.position.Set(-2,0,0);
	body2.Init(model,&cube);
	body2.position.Set(2,0,0);
	s.Add(&body1);
	s.Add(&body2);	
	app.Run();
	return 0;
}
bool App::Update(float t)
{
	s.Update(t);	
	return true;
}
bool App::Render(float t)
{	
	mat4 p,mi;	
	p.GeneratePerspective(90,800.0f/600.0f,0.1f,100.0f);
	renderer.SetProjectionMatrix(p);
	static float appTime=0;
	appTime+=t;
	renderer.Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	mat4 m;	
	m.RotateX(rot.x);
	m.RotateY(rot.y);
	m.Translate(-camPos);
	renderer.SetModelViewMatrix(m);
	renderer.UseProgram(program);
	program.Uniform1i("tex0",0);
	s.Render(renderer);	
	p.GenerateOrthographic(0,800,600,0,-1,1);
	renderer.SetProjectionMatrix(p);
	renderer.SetModelViewMatrix(mi);
	ostringstream oss;
	oss << "FPS:" << app.GetFps();
	renderer.RenderText(font,vec2(0,0),oss.str());
	return true;
}
bool App::HandleInput(float t)
{
	body1.CalculateWorldProperties();
	body2.CalculateWorldProperties();
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
	{
		vec3 v;
		v.FromAngles(rot.x,rot.y);
		camPos+=v*(lastPos.y-m.y)*t;
	}		
	lastPos=m;
	return true;
}
