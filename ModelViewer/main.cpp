#include <FireCube.h>
using namespace std;
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
	ostringstream oss3;
	oss3 << "Loading completed in " << t.Passed() << " seconds.";
	app->frame->statusBar1->SetStatusText(oss3.str());
	FireCube::Node l=root.AddChild(FireCube::Node("LightNode"));
	FireCube::Light light;
	light.Create();
	light.SetAmbientColor(FireCube::vec4(0.3f,0.3f,0.3f,1.0f));
	light.SetDiffuseColor(FireCube::vec4(0.7f,0.7f,0.7f,1.0f));
	light.SetSpecularColor(FireCube::vec4(0.3f,0.3f,0.3f,1.0f));
	light.SetType(FireCube::DIRECTIONAL);
	l.AddLight(light);
	l.Rotate(FireCube::vec3((float)PI/4.0f,(float)PI/4.0f,0));
	if (app->frame->glCanvas->customProgram)
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
		verticesCount+=node.GetGeometries()[j].GetVertices().size();
		faceCount+=node.GetGeometries()[j].GetFaces().size();
	}
	for (vector<FireCube::Node>::iterator i=node.GetChildren().begin();i!=node.GetChildren().end();i++)
		CountElements(*i,verticesCount,faceCount);
}
void FireCubeApp::AddMaterials(unsigned int &id,FireCube::Node node)
{
	MyApp *app=(MyApp*)wxTheApp;
	for (unsigned int j=0;j<node.GetGeometries().size();j++)
	{		
		for (DWORD i=0;i<node.GetGeometries()[j].GetMaterials().size();i++)
			app->frame->AddMaterial(id++,node.GetGeometries()[j].GetMaterials()[i]);
	}
	for (vector<FireCube::Node>::iterator i=node.GetChildren().begin();i!=node.GetChildren().end();i++)
		AddMaterials(id,*i);
}
void FireCubeApp::GenerateNormals(float l)
{
	vector<FireCube::vec3> normals;	
	GenerateNormals(root,l,normals);
	normalRenderingBuffer.LoadData(&normals[0],normals.size()*sizeof(FireCube::vec3),FireCube::STATIC);
	normalRenderingBufferSize=normals.size();
}
void FireCubeApp::GenerateNormals(FireCube::Node node,float l,vector<FireCube::vec3> &normals)
{		
	for (unsigned int j=0;j<node.GetGeometries().size();j++)
	{			
		for (unsigned int i=0;i<node.GetGeometries()[j].GetVertices().size();i++)
		{
			normals.push_back(node.GetGeometries()[j].GetVertices()[i]);
			normals.push_back(node.GetGeometries()[j].GetVertices()[i]+node.GetGeometries()[j].GetNormals()[i]*l);
		}
	}	
	for (vector<FireCube::Node>::iterator i=node.GetChildren().begin();i!=node.GetChildren().end();i++)
		GenerateNormals(*i,l,normals);
}