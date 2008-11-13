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

App app;
map<string,vector <vec3>> vertices2;
vec3 rot(0,0,-150);
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
	Material mat=Material(new MaterialResource);
	mat->program=Program(new ProgramResource);
	mat->program->Create(Renderer::GetShaderManager()->Create("plainColor.vshader"),Renderer::GetShaderManager()->Create("plainColor.fshader"));
	model->material.push_back(mat);	
	SetTitle("ColladaTest");
	font=Renderer::GetFontManager()->Create("c:\\windows\\fonts\\arial.ttf",18);
	if (!font)
		return false;
	LoadCollada("cube.dae");
	for (DWORD i=0;i<model->object.size();i++)
		for (DWORD j=0;j<model->object[i].mesh.size();j++)
		model->object[i].mesh[j].material=mat;
	return true;
}
bool App::LoadCollada(const string &filename)
{
	DAE colladaDom;
	daeInt result = colladaDom.load(filename.c_str());

	if(result != DAE_OK) {		
		return false;
	}
	vector<domMaterial*> materials=colladaDom.getDatabase()->typeLookup<domMaterial>();
	std::cout << "Num materials: " << materials.size() << std::endl;
	for (DWORD matIndex=0;matIndex<materials.size();matIndex++)
	{
		domMaterial *mat=materials[matIndex];
		daeElement *elm=mat->getInstance_effect()->getUrl().getElement();
		domEffect *effect=(domEffect*)elm;
		daeElementRefArray &elmArray=effect->getContents();
		for (DWORD i=0;i<elmArray.getCount();i++)
		{
			daeElementRef el=elmArray.get(i);
			if (strcmp(el->getTypeName(),"profile_COMMON")==0)
			{
				
				
			}
		}
	}
	vector<domMesh*> meshes = colladaDom.getDatabase()->typeLookup<domMesh>();	
	daeUInt meshCount = meshes.size();
	std::cout << "Num meshes: " << meshCount << std::endl;
	for (DWORD meshIndex=0;meshIndex<meshCount;meshIndex++)
	{
		model->object.push_back(Object());		
		domMesh *thisMesh=meshes[meshIndex];
		domVerticesRef vert=thisMesh->getVertices();
		domInputLocal_Array &v=vert->getInput_array();
		for (unsigned int i=0;i<v.getCount();i++)
		{				
			if  (strcmp(v[i]->getSemantic(),"POSITION")==0)
			{
				daeElement *e=v[i]->getSource().getElement();
				domSource *sr=(domSource*)e;				
				for (unsigned int iv=0;iv<sr->getFloat_array()->getValue().getCount();iv+=3)
				{
					vec3 vr((float)sr->getFloat_array()->getValue()[iv+0],(float)sr->getFloat_array()->getValue()[iv+1],(float)sr->getFloat_array()->getValue()[iv+2]);
					vertices2[vert->getId()].push_back(vr);					
				}
			}		
		}
		domTriangles_Array &triArray=thisMesh->getTriangles_array();

		for (DWORD triArrayIndex=0;triArrayIndex<triArray.getCount();triArrayIndex++)
		{		
			domTrianglesRef tris=triArray.get(triArrayIndex);			
			model->object[meshIndex].mesh.push_back(Mesh());
			domPRef p=tris->getP();
			DWORD numTriangles=(DWORD)tris->getCount();
			domInputLocalOffset_Array &tv=tris->getInput_array();
			domListOfUInts &pv=p->getValue();
			DWORD cc=0;
			for (DWORD i=0;i<pv.getCount();)
			{	
				DWORD highestOffset=0;
				for (DWORD cs=0;cs<tv.getCount();cs++)
				{
					domInputLocalOffsetRef lo=tv.get(cs);
					if (lo->getOffset()>highestOffset)
						highestOffset=(DWORD)lo->getOffset();
					if (strcmp(lo->getSemantic(),"VERTEX")==0)
					{
						vector<vec3> &vve=vertices2[lo->getSource().getFragment()];
						vec3 v0=vve[(DWORD)pv[i+(DWORD)lo->getOffset()]];
						model->object[meshIndex].vertex.push_back(v0);				
					}
					if (strcmp(lo->getSemantic(),"NORMAL")==0)
					{				
					}
					if (strcmp(lo->getSemantic(),"TEXCOORD")==0)
					{				
					}
				}
				i+=highestOffset+1;
				cc++;
				if (cc==3)
				{
					cc=0;			
					Face f;
					f.v[0]=model->object[meshIndex].vertex.size()-3;
					f.v[1]=model->object[meshIndex].vertex.size()-2;
					f.v[2]=model->object[meshIndex].vertex.size()-1;
					model->object[meshIndex].mesh[triArrayIndex].face.push_back(f);
				}
			}
		}
	}
	model->UpdateBuffers();
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
		rot.x+=time*(lastPos.y-m.y)/2.0f;
		rot.y+=time*(lastPos.x-m.x)/2.0f;

		rot.x-=rot.x>=2*PI ? (float)(2*PI) : 0;
		rot.x+=rot.x<0 ? (float)(2*PI) : 0;
		rot.y-=rot.y>=2*PI ? (float)(2*PI) : 0;
		rot.y+=rot.y<0 ? (float)(2*PI) : 0;
	}
	if (GetAsyncKeyState(2))
		rot.z+=time*(lastPos.y-m.y)*5.0f;

	lastPos=m;
}