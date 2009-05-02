#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <queue>
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
void FireCubeApp::HandleInput(float t)
{	
}
void FireCubeApp::Render(float t)
{	
}
void FireCubeApp::Update(float t)
{	
}
void FireCubeApp::LoadModel(const string &filename)
{
	MyApp *app=(MyApp*)wxTheApp;
	FireCube::Timer t;
	t.Init();
	model=mm.Create(filename);		
	ostringstream oss3;
	oss3 << "Loading completed in " << t.Passed() << " seconds.";
	app->frame->statusBar1->SetStatusText(oss3.str());
	model->SetProgram(program);		
	DWORD vertexCount=0,faceCount=0;
	for (unsigned int j=0;j<model->object.size();j++)
	{	
		vertexCount+=model->object[j].vertex.size();
		faceCount+=model->object[j].face.size();		
	}
	GenerateNormals(app->frame->glCanvas->normalsLength);
	ostringstream oss,oss2;	
	oss << vertexCount;
	oss2 << faceCount;
	app->frame->textCtrl1->SetValue(oss.str());
	app->frame->textCtrl2->SetValue(oss2.str());
	app->frame->propertyGrid1->Clear();
	for (DWORD i=0;i<model->material.size();i++)
		app->frame->AddMaterial(i+1,model->material[i]);
}
void FireCubeApp::GenerateNormals(float l)
{	
	vector<FireCube::vec3> normals;	
	for (unsigned int j=0;j<model->object.size();j++)
	{			
		for (unsigned int i=0;i<model->object[j].vertex.size();i++)
		{
			normals.push_back(model->object[j].vertex[i]);
			normals.push_back(model->object[j].vertex[i]+model->object[j].normal[i]*l);
		}
	}
	normalRenderingBuffer->LoadData(&normals[0],normals.size()*sizeof(FireCube::vec3),FireCube::STATIC);
	normalRenderingBufferSize=normals.size();
}