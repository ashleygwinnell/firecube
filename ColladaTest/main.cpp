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
	Program program=Program(new ProgramResource);
	program->Create(Renderer::GetShaderManager()->Create("plainColor.vshader"),Renderer::GetShaderManager()->Create("plainColor.fshader"));	
	SetTitle("ColladaTest");
	font=Renderer::GetFontManager()->Create("c:\\windows\\fonts\\arial.ttf",18);
	if (!font)
		return false;
	LoadCollada("cube.dae");
	model->SetProgram(program);
	//return false;	
	return true;
}
void App::LoadMaterials()
{
	vector<domMaterial*> materials=colladaDom.getDatabase()->typeLookup<domMaterial>();	
	for (DWORD matIndex=0;matIndex<materials.size();matIndex++)
	{
		Material curMat=Material(new MaterialResource);
		model->material.push_back(curMat);
		domMaterial *mat=materials[matIndex];
		curMat->name=mat->getName();
		daeElement *elm=mat->getInstance_effect()->getUrl().getElement();
		domEffect *effect=(domEffect*)elm;
		domFx_profile_abstract_Array &fxaa=effect->getFx_profile_abstract_array();		
		for (DWORD i=0;i<fxaa.getCount();i++)
		{
			domFx_profile_abstractRef pa=fxaa.get(i);
			if (strcmp(pa->getTypeName(),"profile_COMMON")==0)
			{				
				daeElement *techniqueElm=pa->getChild("technique");
				daeElement *phongElm=techniqueElm->getChild("phong");
				daeElement *blinnElm=techniqueElm->getChild("phong");

				if (phongElm)
				{				
					string data;
					daeElement *colorElm=NULL;

					colorElm=phongElm->getChild("ambient")->getChild("color");
					if (colorElm)
					{
						colorElm->getCharData(data);
						istringstream ass(data);
						ass >> curMat->ambient.x >> curMat->ambient.y >> curMat->ambient.z >> curMat->ambient.w;
					}

					
					colorElm=phongElm->getChild("diffuse")->getChild("color");
					if (colorElm)
					{
						colorElm->getCharData(data);
						istringstream dss(data);
						dss >> curMat->diffuse.x >> curMat->diffuse.y >> curMat->diffuse.z >> curMat->diffuse.w;
					}

					colorElm=phongElm->getChild("specular")->getChild("color");
					if (colorElm)
					{
						colorElm->getCharData(data);
						istringstream sss(data);
						sss >> curMat->specular.x >> curMat->specular.y >> curMat->specular.z >> curMat->specular.w;
					}

					phongElm->getChild("shininess")->getChild("float")->getCharData(data);				
					istringstream shss(data);
					shss >> curMat->shininess;
				}
				else if (blinnElm)
				{				
					string data;
					daeElement *colorElm=NULL;

					colorElm=blinnElm->getChild("ambient")->getChild("color");
					if (colorElm)
					{
						colorElm->getCharData(data);
						istringstream ass(data);
						ass >> curMat->ambient.x >> curMat->ambient.y >> curMat->ambient.z >> curMat->ambient.w;
					}


					colorElm=blinnElm->getChild("diffuse")->getChild("color");
					if (colorElm)
					{
						colorElm->getCharData(data);
						istringstream dss(data);
						dss >> curMat->diffuse.x >> curMat->diffuse.y >> curMat->diffuse.z >> curMat->diffuse.w;
					}

					colorElm=blinnElm->getChild("specular")->getChild("color");
					if (colorElm)
					{
						colorElm->getCharData(data);
						istringstream sss(data);
						sss >> curMat->specular.x >> curMat->specular.y >> curMat->specular.z >> curMat->specular.w;
					}

					blinnElm->getChild("shininess")->getChild("float")->getCharData(data);				
					istringstream shss(data);
					shss >> curMat->shininess;
				}

			}
		}
	}
}
void App::LoadMesh(string &id,Model model,map<string,Material> &materialMap)
{	
	map<string,vector <vec3>> vertices2;
	vector<domGeometry *> geometries=colladaDom.getDatabase()->typeLookup<domGeometry>();
	for (DWORD geometryIndex=0;geometryIndex<geometries.size();geometryIndex++)
	{		
		domGeometry *geometry=geometries[geometryIndex];		
		if (geometry->getID()!=id)
			continue;		
		model->object.push_back(Object());				
		model->object[model->object.size()-1].name=geometry->getID();
		domMeshRef thisMesh=geometry->getMesh();		
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
			model->object[model->object.size()-1].mesh.push_back(Mesh());
			Material mat=materialMap[tris->getMaterial()];
			if (mat)							
				model->object[model->object.size()-1].mesh[triArrayIndex].material=mat;
			
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
						model->object[model->object.size()-1].vertex.push_back(v0);				
					}
					if (strcmp(lo->getSemantic(),"NORMAL")==0)
					{
						daeElement *e=lo->getSource().getElement();
						domSource *so=(domSource*)e;
						domListOfFloats &lof=so->getFloat_array()->getValue();
						vec3 n0;
						n0.x=(float)lof.get((DWORD)pv[i+(DWORD)lo->getOffset()]+0);
						n0.y=(float)lof.get((DWORD)pv[i+(DWORD)lo->getOffset()]+1);
						n0.z=(float)lof.get((DWORD)pv[i+(DWORD)lo->getOffset()]+2);
						model->object[model->object.size()-1].normal.push_back(n0);
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
					f.v[0]=model->object[model->object.size()-1].vertex.size()-3;
					f.v[1]=model->object[model->object.size()-1].vertex.size()-2;
					f.v[2]=model->object[model->object.size()-1].vertex.size()-1;
					model->object[model->object.size()-1].mesh[triArrayIndex].face.push_back(f);					

				}
			}
		}		
	}			
}
void App::LoadNodes()
{
	vector<domNode*> nodes = colladaDom.getDatabase()->typeLookup<domNode>();
	for (DWORD i=0;i<nodes.size();i++)
	{
		domNode *curNode=nodes[i];
		domInstance_geometry_Array &iga=curNode->getInstance_geometry_array();
		for (DWORD j=0;j<iga.getCount();j++)
		{
			map<string,Material> materialMap;
			domInstance_geometryRef ig=iga.get(j);
			domInstance_material_Array &ima=ig->getBind_material()->getTechnique_common()->getInstance_material_array();
			for (DWORD k=0;k<ima.getCount();k++)
			{
				domInstance_materialRef im=ima.get(k);
				Material mat=GetMaterialByName(string(im->getTarget().getFragment()));
				if (mat)
					materialMap[im->getSymbol()]=mat;

			}
			LoadMesh(string(ig->getUrl().getFragment()),model,materialMap);
		}
	}
}
bool App::LoadCollada(const string &filename)
{
	
	daeInt result = colladaDom.load(filename.c_str());

	if(result != DAE_OK) {		
		return false;
	}
	
	LoadMaterials();
	LoadNodes();
	
	
	model->UpdateBuffers();
	return true;
}
Material App::GetMaterialByName(string &name)
{
	for (DWORD i=0;i<model->material.size();i++)
	{
		if (model->material[i]->name==name)
			return model->material[i];
	}
	return Material();
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