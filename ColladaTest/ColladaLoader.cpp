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

#include "ColladaLoader.h"

ColladaLoader::ColladaLoader(ModelResource *model) : model(model)
{

}

vec4 ColladaLoader::ReadColor(daeElement *elm)
{
	string data;
	vec4 ret(1,1,1,1);
	daeElement *colorElm=elm->getChild("color");
	if (colorElm)
	{
		colorElm->getCharData(data);
		istringstream iss(data);
		iss >> ret.x >> ret.y >> ret.z >> ret.w;
	}
	return ret;
}
bool ColladaLoader::ReadTexture(daeElement *elm,map<string,pair<string,DWORD>> &vertexInput,string &file,DWORD &unit)
{
	daeElement *textureElm=elm->getChild("texture");
	if (textureElm)
	{
		string texture=textureElm->getAttribute("texture");
		unit=vertexInput[textureElm->getAttribute("texcoord")].second;
		daeSidRef sidSampler(texture, textureElm->getAncestor("effect"));
		daeElement* samplerElm = sidSampler.resolve().elt;
		if (samplerElm)
		{
			string srf;
			samplerElm->getChild("sampler2D")->getChild("source")->getCharData(srf);
			daeSidRef sidSurface(srf,samplerElm->getAncestor("effect"));
			daeElement *surfaceElm=sidSurface.resolve().elt;
			if (surfaceElm)
			{
				string image;
				surfaceElm->getChild("surface")->getChild("init_from")->getCharData(image);
				file=imageMap[image];
				return true;
			}
		}
	}
	return false;
}
Material ColladaLoader::LoadMaterial(const string &name,map<string,pair<string,DWORD>> &vertexInput)
{
	vector<domMaterial*> materials=colladaDom.getDatabase()->typeLookup<domMaterial>();	
	for (DWORD matIndex=0;matIndex<materials.size();matIndex++)
	{
		domMaterial *mat=materials[matIndex];
		if (mat->getName()==name)
		{	
			Material curMat=Material(new MaterialResource);		
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
					daeElement *shadingElm=techniqueElm->getChild("phong");
					if (!shadingElm) 
						shadingElm=techniqueElm->getChild("blinn");

					if (shadingElm)
					{				
						string data;
						string file;
						DWORD unit;

						curMat->ambient=ReadColor(shadingElm->getChild("ambient"));
						curMat->diffuse=ReadColor(shadingElm->getChild("diffuse"));
						curMat->specular=ReadColor(shadingElm->getChild("specular"));
						if (ReadTexture(shadingElm->getChild("diffuse"),vertexInput,file,unit))
						{
							curMat->texture[unit]=Renderer::GetTextureManager()->Create(file);
						}						
						shadingElm->getChild("shininess")->getChild("float")->getCharData(data);				
						istringstream shss(data);
						shss >> curMat->shininess;
					}					
				}
			}
			return curMat;
		}
	}
	return Material();
}
void ColladaLoader::LoadMesh(string &id,mat4 transform,map<string,string> &materialMap,map<string,pair<string,DWORD>> &vertexInput)
{	
	map<string,Source> sourcesArray;
	map<string,vector<vec3>> vec3Array;
	map<string,vector<vec2>> vec2Array;
	vector<domGeometry *> geometries=colladaDom.getDatabase()->typeLookup<domGeometry>();
	for (DWORD geometryIndex=0;geometryIndex<geometries.size();geometryIndex++)
	{		
		domGeometry *geometry=geometries[geometryIndex];		
		if (geometry->getID()!=id)
			continue;		
		model->object.push_back(Object());				
		model->object[model->object.size()-1].name=geometry->getID();
		
		domMeshRef thisMesh=geometry->getMesh();		
		
		domSource_Array &sourceArray=thisMesh->getSource_array();
		for (DWORD sourceIndex=0;sourceIndex<sourceArray.getCount();sourceIndex++)
		{			
			domSourceRef source=sourceArray.get(sourceIndex);			
			sourcesArray[source->getID()]=ReadSource(source);
		}
		domVerticesRef vert=thisMesh->getVertices();
		domInputLocal_Array &v=vert->getInput_array();		
		for (unsigned int i=0;i<v.getCount();i++)
		{
			if  (strcmp(v[i]->getSemantic(),"POSITION")==0)
			{
				vec3Array[vert->getId()]=FloatArray2Vec3Array(sourcesArray[v[i]->getSource().getFragment()].floatArray,sourcesArray[v[i]->getSource().getFragment()].stride);
			}
		}

		domTriangles_Array &triArray=thisMesh->getTriangles_array();
		for (DWORD triArrayIndex=0;triArrayIndex<triArray.getCount();triArrayIndex++)
		{
			domTrianglesRef tris=triArray.get(triArrayIndex);
			model->object[model->object.size()-1].mesh.push_back(Mesh());
			Material mat=LoadMaterial(materialMap[tris->getMaterial()],vertexInput);
			if (mat)
			{
				model->material.push_back(mat);
				model->object[model->object.size()-1].mesh[triArrayIndex].material=mat;
			}

			domPRef p=tris->getP();
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
						vector<vec3> &vve=vec3Array[lo->getSource().getFragment()];
						vec3 v0=vve[(DWORD)pv[i+(DWORD)lo->getOffset()]];
						model->object[model->object.size()-1].vertex.push_back(v0*transform);				
					}
					if (strcmp(lo->getSemantic(),"NORMAL")==0)
					{						
						if (vec3Array.find(lo->getSource().getFragment())==vec3Array.end())						
						{							
							vec3Array[lo->getSource().getFragment()]=FloatArray2Vec3Array(sourcesArray[lo->getSource().getFragment()].floatArray,sourcesArray[lo->getSource().getFragment()].stride);
						}						
						vec3 n=vec3Array[lo->getSource().getFragment()][(DWORD)pv[i+(DWORD)lo->getOffset()]];
						n=n.TransformNormal(transform);
						model->object[model->object.size()-1].normal.push_back(n);
					}
					if (strcmp(lo->getSemantic(),"TEXCOORD")==0)
					{
						if (vec2Array.find(lo->getSource().getFragment())==vec2Array.end())
						{							
							vec2Array[lo->getSource().getFragment()]=FloatArray2Vec2Array(sourcesArray[lo->getSource().getFragment()].floatArray,sourcesArray[lo->getSource().getFragment()].stride);
						}
						DWORD unit=(DWORD)lo->getSet();
						vec2 uv=vec2Array[lo->getSource().getFragment()][(DWORD)pv[i+(DWORD)lo->getOffset()]];
						model->object[model->object.size()-1].uv[unit].push_back(vec2(uv.x,1.0f-uv.y));
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
					vec3 faceNormal=Cross(model->object[model->object.size()-1].vertex[f.v[1]]-model->object[model->object.size()-1].vertex[f.v[0]],model->object[model->object.size()-1].vertex[f.v[2]]-model->object[model->object.size()-1].vertex[f.v[0]]);
					faceNormal.Normalize();
					f.normal=faceNormal;
					model->object[model->object.size()-1].mesh[triArrayIndex].face.push_back(f);					

				}
			}
		}		
	}			
}
void ColladaLoader::LoadNodes()
{
	vector<domNode*> nodes = colladaDom.getDatabase()->typeLookup<domNode>();
	for (DWORD i=0;i<nodes.size();i++)
	{
		mat4 transform;
		domNode *curNode=nodes[i];		
		transform=ReadTransformation(curNode);
		domInstance_geometry_Array &iga=curNode->getInstance_geometry_array();
		for (DWORD j=0;j<iga.getCount();j++)
		{
			map<string,string> materialMap;
			map<string,pair<string,DWORD>> vertexInput;
			domInstance_geometryRef ig=iga.get(j);
			domInstance_material_Array &ima=ig->getBind_material()->getTechnique_common()->getInstance_material_array();
			for (DWORD k=0;k<ima.getCount();k++)
			{
				domInstance_materialRef im=ima.get(k);
				domInstance_material::domBind_vertex_input_Array &via = im->getBind_vertex_input_array();
				for (DWORD l=0;l<via.getCount();l++)				
					vertexInput[via.get(l)->getSemantic()]=make_pair<string,unsigned int>(via.get(l)->getInput_semantic(),(unsigned int)via.get(l)->getInput_set());
				materialMap[im->getSymbol()]=string(im->getTarget().getFragment());

			}
			LoadMesh(string(ig->getUrl().getFragment()),transform,materialMap,vertexInput);
		}
	}
}
bool ColladaLoader::Load(const string &filename)
{

	daeInt result = colladaDom.load(filename.c_str());

	if(result != DAE_OK) {		
		return false;
	}

	LoadImageMap();	
	LoadNodes();


	model->UpdateBuffers();
	return true;
}
ColladaLoader::Source ColladaLoader::ReadSource(domSourceRef source)
{
	Source ret;
	ret.stride=(int)source->getTechnique_common()->getAccessor()->getStride();
	if (source->getFloat_array())
	{	
		for (unsigned int iv=0;iv<source->getFloat_array()->getValue().getCount();iv++)
		{
			ret.floatArray.push_back((float)source->getFloat_array()->getValue()[iv]);
		}
	}	
	return ret;
}
Material ColladaLoader::GetMaterialByName(string &name)
{
	for (DWORD i=0;i<model->material.size();i++)
	{
		if (model->material[i]->name==name)
			return model->material[i];
	}
	return Material();
}
vector<vec3> ColladaLoader::FloatArray2Vec3Array(const vector<float> &fa,int stride)
{
	vector<vec3> ret;	
	for (unsigned int i=0;i<fa.size();i+=stride)
		ret.push_back(vec3(fa[i+0],fa[i+1],fa[i+2]));
	return ret;
}
vector<vec2> ColladaLoader::FloatArray2Vec2Array(const vector<float> &fa,int stride)
{
	vector<vec2> ret;	
	for (unsigned int i=0;i<fa.size();i+=stride)
		ret.push_back(vec2(fa[i+0],fa[i+1]));
	return ret;
}
void ColladaLoader::LoadImageMap()
{
	vector<domImage*> images = colladaDom.getDatabase()->typeLookup<domImage>();
	for (DWORD i=0;i<images.size();i++)
	{
		domImage *curImage=images[i];
		string file=curImage->getInit_from()->getValue().getOriginalURI();
		imageMap[curImage->getId()]=file;
	}
}
mat4 ColladaLoader::ReadTransformation(domNode *node)
{
	mat4 transform;

	domTranslate_Array &ta=node->getTranslate_array();
	for (DWORD j=0;j<ta.getCount();j++)
	{
		domTranslateRef t=ta.get(j);
		domFloat3 &tv=t->getValue();
		vec3 tr((float)tv.get(0),(float)tv.get(1),(float)tv.get(2));
		mat4 trans;
		trans.Translate(tr);
		transform*=trans;
	}
	domRotate_Array &ra=node->getRotate_array();
	for (DWORD j=0;j<ra.getCount();j++)
	{
		domRotateRef r=ra.get(j);			
		domFloat4 &rv=r->getValue();
		vec4 rt((float)rv.get(0),(float)rv.get(1),(float)rv.get(2),(float)(-rv.get(3)/180.0f * PI));
		mat4 rot;
		rot.Rotate(rt);
		transform*=rot;
	}	
	domScale_Array &sa=node->getScale_array();
	for (DWORD j=0;j<sa.getCount();j++)
	{
		domScaleRef s=sa.get(j);			
		domFloat3 &sv=s->getValue();
		vec3 sr((float)sv.get(0),(float)sv.get(1),(float)sv.get(2));
		mat4 scale;
		scale.Scale(sr.x,sr.y,sr.z);
		transform*=scale;
	}	
	return transform;
}