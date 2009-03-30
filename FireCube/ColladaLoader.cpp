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
#include "FireCube.h"
using namespace FireCube;
#include "ModelLoaders.h"


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
			if (samplerElm->getElementType()==COLLADA_TYPE::COMMON_NEWPARAM_TYPE)
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
			else if (samplerElm->getElementType()==COLLADA_TYPE::IMAGE)
			{
				domImage *img=(domImage*)samplerElm;				
				string f=img->getInit_from()->getValue().getOriginalURI();	
				file=f;
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
		if (mat->getID()==name)
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
						domElement *transparency=shadingElm->getChild("transparency");
						if (transparency)
						{
							domElement *f=transparency->getChild("float");
							if (f)
							{							
								string data;
								f->getCharData(data);
								istringstream iss(data);
								float v;
								iss >> v;
								curMat->diffuse.w=v;
							}
						}
						if (ReadTexture(shadingElm->getChild("diffuse"),vertexInput,file,unit))
						{
							string::size_type i=file.find("file://");
							if (i!=string::npos)
							{
								file.erase(i,i+7);
							}
							i=file.find("%20");
							while (i!=string::npos)
							{
								file.replace(i,3," ");								
								i=file.find("%20");
							}
							
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
	map<string,vector<vec4>> vecArray;	
	vector<domGeometry *> geometries=colladaDom.getDatabase()->typeLookup<domGeometry>();
	for (DWORD geometryIndex=0;geometryIndex<geometries.size();geometryIndex++)
	{		
		domGeometry *geometry=geometries[geometryIndex];		
		if (geometry->getID()!=id)
			continue;		
		model->object.push_back(Object());				
		model->object[model->object.size()-1].name=geometry->getID();

		domMeshRef thisMesh=geometry->getMesh();		

		// Read all sources for this mesh.
		domSource_Array &sourceArray=thisMesh->getSource_array();
		for (DWORD sourceIndex=0;sourceIndex<sourceArray.getCount();sourceIndex++)
		{			
			domSourceRef source=sourceArray.get(sourceIndex);
			sourcesArray[source->getID()]=ReadSource(source);
		}

		// Read vertices.
		domVerticesRef vert=thisMesh->getVertices();
		domInputLocal_Array &v=vert->getInput_array();
		for (unsigned int i=0;i<v.getCount();i++)
		{
			if  (strcmp(v[i]->getSemantic(),"POSITION")==0)
			{
				vecArray[vert->getId()]=SourceToVecArray(sourcesArray[v[i]->getSource().getFragment()]);
			}
		}
		// Read triangles lists.
		domTriangles_Array &triArray=thisMesh->getTriangles_array();
		for (DWORD triArrayIndex=0;triArrayIndex<triArray.getCount();triArrayIndex++)
		{
			domTrianglesRef tris=triArray.get(triArrayIndex);
			model->object[model->object.size()-1].mesh.push_back(Mesh());
			// Load the material.
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
			map<string,DWORD> deindexMap;
			DWORD v[3];
			for (DWORD i=0;i<pv.getCount();)
			{				
				DWORD highestOffset=0;
				ostringstream vertexId;
				// Generate current vertex id based on its indices to the input buffers.
				for (DWORD cs=0;cs<tv.getCount();cs++)
				{
					domInputLocalOffsetRef lo=tv.get(cs);
					if (lo->getOffset()>highestOffset)
						highestOffset=(DWORD)lo->getOffset();
					vertexId << (DWORD)pv[i+(DWORD)lo->getOffset()];
					vertexId << " ";
				}
				if (deindexMap.find(vertexId.str())==deindexMap.end()) // If it's a new vertex, read it.
				{				
					for (DWORD cs=0;cs<tv.getCount();cs++)
					{
						domInputLocalOffsetRef lo=tv.get(cs);						
						if (strcmp(lo->getSemantic(),"VERTEX")==0) // Get position from previously read source.
						{
							vector<vec4> &vve=vecArray[lo->getSource().getFragment()];
							vec3 v0=vve[(DWORD)pv[i+(DWORD)lo->getOffset()]];
							v0=v0*transform;
							if (upAxis==0)
							{
								std::swap(v0.x,v0.y);
								v0.x*=-1;
							}
							else if (upAxis==2)
							{
								std::swap(v0.y,v0.z);
								v0.z*=-1;
							}
							model->object[model->object.size()-1].vertex.push_back(v0);				
						}
						if (strcmp(lo->getSemantic(),"NORMAL")==0) // Get normal from previously read source.
						{						
							if (vecArray.find(lo->getSource().getFragment())==vecArray.end())
							{							
								vecArray[lo->getSource().getFragment()]=SourceToVecArray(sourcesArray[lo->getSource().getFragment()]);
							}						
							vec3 n=vecArray[lo->getSource().getFragment()][(DWORD)pv[i+(DWORD)lo->getOffset()]];
							n=n.TransformNormal(transform);
							if (upAxis==0)
							{
								std::swap(n.x,n.y);
								n.x*=-1;
							}
							else if (upAxis==2)
							{
								std::swap(n.y,n.z);
								n.z*=-1;
							}
							n.Normalize();
							model->object[model->object.size()-1].normal.push_back(n);
						}
						if (strcmp(lo->getSemantic(),"TEXCOORD")==0) // Get uv from previously read source.
						{
							if (vecArray.find(lo->getSource().getFragment())==vecArray.end())
							{							
								vecArray[lo->getSource().getFragment()]=SourceToVecArray(sourcesArray[lo->getSource().getFragment()]);
							}
							DWORD unit=(DWORD)lo->getSet();
							vec2 uv=vecArray[lo->getSource().getFragment()][(DWORD)pv[i+(DWORD)lo->getOffset()]];
							model->object[model->object.size()-1].uv[unit].push_back(vec2(uv.x,1.0f-uv.y));
						}						
					}
					v[cc]=model->object[model->object.size()-1].vertex.size()-1; // Remember this vertex index.
					deindexMap[vertexId.str()]=v[cc];
				}
				else
					v[cc]=deindexMap[vertexId.str()]; // Already read this vertex, just get its index.

				i+=highestOffset+1;
				cc++;
				if (cc==3)
				{
					cc=0;			
					Face f;
					f.v[0]=v[0];
					f.v[1]=v[1];
					f.v[2]=v[2];
					vec3 faceNormal=Cross(model->object[model->object.size()-1].vertex[f.v[1]]-model->object[model->object.size()-1].vertex[f.v[0]],model->object[model->object.size()-1].vertex[f.v[2]]-model->object[model->object.size()-1].vertex[f.v[0]]);
					faceNormal.Normalize();
					f.normal=faceNormal;
					model->object[model->object.size()-1].mesh[triArrayIndex].face.push_back(f);
					model->object[model->object.size()-1].face.push_back(f);
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
	vector<domAsset*> assets = colladaDom.getDatabase()->typeLookup<domAsset>();
	for (DWORD i=0;i<assets.size();i++)
	{
		domAsset *asset=assets[i];
		domAsset::domUp_axisRef up=asset->getUp_axis();
		if (up->getValue()==UPAXISTYPE_X_UP)
			upAxis=0;
		else if (up->getValue()==UPAXISTYPE_Y_UP)
			upAxis=1;
		else if (up->getValue()==UPAXISTYPE_Z_UP)
			upAxis=2;
		domAsset::domUnitRef unit=asset->getUnit();
		scale=vec3((float)unit->getMeter(),(float)unit->getMeter(),(float)unit->getMeter());
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
vector<vec4> ColladaLoader::SourceToVecArray(const Source &source)
{
	vector<vec4> ret;	
	vec4 v;
	for (DWORD i=0;i<source.floatArray.size();i+=source.stride)
	{
		for (DWORD j=0;j<source.stride;j++)
		{
			if (j==0)
				v.x=source.floatArray[i+j];
			if (j==1)
				v.y=source.floatArray[i+j];
			if (j==2)
				v.z=source.floatArray[i+j];
			if (j==3)
				v.w=source.floatArray[i+j];
		}
		ret.push_back(v);
	}		
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
	daeTArray<daeSmartRef<daeElement>> children;
	node->getChildren(children);
	for (DWORD i=0;i<children.getCount();i++)
	{
		daeSmartRef<domElement> elm=children.get(i);
		if (elm->getElementType()==COLLADA_TYPE::MATRIX)
		{
			mat4 t;
			domMatrix *mat=(domMatrix*)elm.cast();
			for (DWORD j=0;j<16;j++)
				t.m[j]=(float)mat->getValue().get(j);
			t.Transpose();
			transform*=t;
		}
		else if (elm->getElementType()==COLLADA_TYPE::TRANSLATE)
		{
			domTranslate *t=(domTranslate*)elm.cast();			
			vec3 tr((float)t->getValue().get(0),(float)t->getValue().get(1),(float)t->getValue().get(2));
			mat4 trans;
			trans.Translate(tr);
			transform*=trans;
		}
		else if (elm->getElementType()==COLLADA_TYPE::ROTATE)
		{
			domRotate *r=(domRotate*)elm.cast();			
			vec4 rt((float)r->getValue().get(0),(float)r->getValue().get(1),(float)r->getValue().get(2),(float)(-r->getValue().get(3)/180.0f * PI));
			mat4 rot;
			rot.Rotate(rt);
			transform*=rot;
		}
		else if (elm->getElementType()==COLLADA_TYPE::SCALE)
		{
			domScale *s=(domScale*)elm.cast();			
			vec3 sr((float)s->getValue().get(0),(float)s->getValue().get(1),(float)s->getValue().get(2));
			mat4 scale;
			scale.Scale(sr.x,sr.y,sr.z);
			transform*=scale;
		}
	}	
		
	if (node->getParent())
		if (node->getParent()->getElementType()==COLLADA_TYPE::NODE)		
			return ReadTransformation((domNode*)node->getParent())*transform;
	return transform;
}