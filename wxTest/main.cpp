#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "MyGLCanvas.h"
#include "mainframe.h"
#include "mymainframe.h"
#include "main.h"

IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	frame=new MyMainFrame(NULL);	
	frame->Show(true);
	return true;
}
bool FireCubeApp::HandleInput(float t)
{
	return true;
}
bool FireCubeApp::Render(float t)
{
	return true;
}
bool FireCubeApp::Update(float t)
{
	return true;
}
void FireCubeApp::LoadModel(const string &filename)
{
	MyApp *app=(MyApp*)wxTheApp;
	model=mm.Create(filename);		
	model->SetProgram(program);	
	vector<FireCube::vec3> normals;
	for (unsigned int i=0;i<model->object[0].vertex.size();i++)
	{
		normals.push_back(model->object[0].vertex[i]);
		normals.push_back(model->object[0].vertex[i]+model->object[0].normal[i]*0.07f);
	}
	normalRenderingBuffer->LoadData(&normals[0],normals.size()*sizeof(FireCube::vec3),FireCube::STATIC);
	normalRenderingBufferSize=normals.size();
	ostringstream oss,oss2;
	oss << model->object[0].vertex.size();
	oss2 << model->object[0].face.size();
	app->frame->textCtrl1->SetValue(oss.str());
	app->frame->textCtrl2->SetValue(oss2.str());
}
