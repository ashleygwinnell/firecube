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
	root=FireCube::LoadMesh(filename);
	root.SetLighting(false);
	ostringstream oss3;
	oss3 << "Loading completed in " << t.Passed() << " seconds.";
	app->frame->statusBar1->SetStatusText(oss3.str());
	root.SetProgram(program);		
	unsigned int vertexCount=0,faceCount=0;
	CountElements(root,vertexCount,faceCount);
	GenerateNormals(app->frame->glCanvas->normalsLength);
	ostringstream oss,oss2;	
	oss << vertexCount;
	oss2 << faceCount;
	app->frame->textCtrl1->SetValue(oss.str());
	app->frame->textCtrl2->SetValue(oss2.str());
	app->frame->propertyGrid1->Clear();
	unsigned int id=1;
	AddMaterials(id,root);
}
void FireCubeApp::CountElements(FireCube::Node node,unsigned int &verticesCount,unsigned int &faceCount)
{
	for (unsigned int j=0;j<node.GetGeometries().size();j++)
	{		
		verticesCount+=node.GetGeometries()[j]->vertex.size();
		faceCount+=node.GetGeometries()[j]->face.size();
	}
	for (vector<FireCube::Node>::iterator i=node.GetChildren().begin();i!=node.GetChildren().end();i++)
		CountElements(*i,verticesCount,faceCount);
}
void FireCubeApp::AddMaterials(unsigned int &id,FireCube::Node node)
{
	MyApp *app=(MyApp*)wxTheApp;
	for (unsigned int j=0;j<node.GetGeometries().size();j++)
	{		
		for (DWORD i=0;i<node.GetGeometries()[j]->material.size();i++)
			app->frame->AddMaterial(id++,node.GetGeometries()[j]->material[i]);
	}
	for (vector<FireCube::Node>::iterator i=node.GetChildren().begin();i!=node.GetChildren().end();i++)
		AddMaterials(id,*i);
}
void FireCubeApp::GenerateNormals(float l)
{
	vector<FireCube::vec3> normals;	
	GenerateNormals(root,l,normals);
	normalRenderingBuffer->LoadData(&normals[0],normals.size()*sizeof(FireCube::vec3),FireCube::STATIC);
	normalRenderingBufferSize=normals.size();
}
void FireCubeApp::GenerateNormals(FireCube::Node node,float l,vector<FireCube::vec3> &normals)
{		
	for (unsigned int j=0;j<node.GetGeometries().size();j++)
	{			
		for (unsigned int i=0;i<node.GetGeometries()[j]->vertex.size();i++)
		{
			normals.push_back(node.GetGeometries()[j]->vertex[i]);
			normals.push_back(node.GetGeometries()[j]->vertex[i]+node.GetGeometries()[j]->normal[i]*l);
		}
	}	
	for (vector<FireCube::Node>::iterator i=node.GetChildren().begin();i!=node.GetChildren().end();i++)
		GenerateNormals(*i,l,normals);
}