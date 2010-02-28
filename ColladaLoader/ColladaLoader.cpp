#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "tinyxml.h"
#include "ColladaLoader.h"

ColladaLoader::ColladaLoader(const std::string &filename) : xmlDocument(filename)
{	
}
ColladaLoader::~ColladaLoader()
{
	map<string,Node *>::iterator i=nodeLibrary.begin();
	for (;i!=nodeLibrary.end();i++)		
		DeleteNodes(i->second);
}
bool ColladaLoader::Load()
{	
	xmlDocument.LoadFile();	
	TiXmlElement *e=GetChildElement(&xmlDocument,"COLLADA");
	if (e==NULL)
		return false;
	
	string version=e->Attribute("version");
	if (version.substr(0,3)=="1.4")
		ReadLibraries(e);

	return true;
}
void ColladaLoader::ReadLibraries(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="asset")
				ReadAsset(element);
			else if (element->ValueStr()=="library_images")
				ReadImageLibrary(element);
			else if (element->ValueStr()=="library_materials")
				ReadMaterialLibrary(element);
			else if (element->ValueStr()=="library_effects")
				ReadEffectLibrary(element);
			else if (element->ValueStr()=="library_geometries")
				ReadGeometryLibrary(element);
			else if (element->ValueStr()=="library_visual_scenes")
				ReadSceneLibrary(element);
			else if (element->ValueStr()=="library_nodes")
				ReadSceneNode(element,NULL);
			else if (element->ValueStr()=="scene")
				ReadScene(element);
		}
	}
}
void ColladaLoader::ReadAsset(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="unit")
			{				
				if (element->QueryFloatAttribute("meter",&unit)!=TIXML_SUCCESS)
					unit=1.0f;
			}
			else if (element->ValueStr()=="up_axis")
			{
				string v=element->FirstChild()->ToText()->ValueStr();
				if (v=="X_UP")
					upDirection=X_UP;
				else if (v=="Y_UP")
					upDirection=Y_UP;
				else if (v=="Z_UP")
					upDirection=Z_UP;
			}
		}
	}
}
void ColladaLoader::ReadImageLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="image")
			{
				string id=element->Attribute("id");
				imageLibrary[id]=Image();
				ReadImage(element,imageLibrary[id]);
			}
		}
	}
}
void ColladaLoader::ReadImage(TiXmlNode *parent,Image &img)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="init_from")
			{
				img.initFrom=element->GetText();
			}
		}
	}
}
void ColladaLoader::ReadMaterialLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="material")
			{
				string id=element->Attribute("id");
				materialLibrary[id]=Material();
				ReadMaterial(element,materialLibrary[id]);
			}
		}
	}
}
void ColladaLoader::ReadMaterial(TiXmlNode *parent,Material &mat)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="instance_effect")
			{
				string url=element->Attribute("url");
				mat.effect=url.substr(1);
			}
		}
	}
}
void ColladaLoader::ReadEffectLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="effect")
			{
				string id=element->Attribute("id");
				effectLibrary[id]=Effect();
				ReadEffect(element,effectLibrary[id]);
			}
		}
	}
}
void ColladaLoader::ReadEffect(TiXmlNode *parent,Effect &effect)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="profile_COMMON")
				ReadEffectProfileCommon(element,effect);
		}
	}
}
void ColladaLoader::ReadEffectProfileCommon(TiXmlNode *parent,Effect &effect)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="technique")
				ReadEffectProfileCommon(element,effect);
			else if (element->ValueStr()=="newparam")
			{
				string sid=element->Attribute("sid");
				effect.effectParams[sid]=EffectParam();
				ReadEffectParam(element,effect.effectParams[sid]);
			}
			else if (element->ValueStr()=="phong")
			{
				effect.shadingType=SHADING_PHONG;
				ReadEffectProfileCommon(element,effect);
			}
			else if (element->ValueStr()=="lambert")
			{
				effect.shadingType=SHADING_LAMBERT;
				ReadEffectProfileCommon(element,effect);
			}
			else if (element->ValueStr()=="blinn")
			{
				effect.shadingType=SHADING_BLINN;
				ReadEffectProfileCommon(element,effect);
			}
			else if (element->ValueStr()=="constant")
			{
				effect.shadingType=SHADING_CONSTANT;
				ReadEffectProfileCommon(element,effect);
			}
			else if (element->ValueStr()=="ambient")
				ReadEffectColor(element,effect.ambientColor,effect.ambientSampler);
			else if (element->ValueStr()=="diffuse")
				ReadEffectColor(element,effect.diffuseColor,effect.diffuseSampler);
			else if (element->ValueStr()=="specular")
				ReadEffectColor(element,effect.specularColor,effect.specularSampler);
		}
	}
}
void ColladaLoader::ReadEffectParam(TiXmlNode *parent,EffectParam &effectParam)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="surface")
			{
				TiXmlElement *e=element->FirstChildElement();
				if (e->ValueStr()=="init_from")
				{
					effectParam.type=PARAM_SURFACE;
					effectParam.reference=e->GetText();
				}
			}
			else if (element->ValueStr()=="sampler2D")
			{
				TiXmlElement *e=element->FirstChildElement();
				if (e->ValueStr()=="source")
				{
					effectParam.type=PARAM_SAMPLER_2D;
					effectParam.reference=e->GetText();
				}
			}
		}
	}
}
void ColladaLoader::ReadEffectColor(TiXmlNode *parent,vec4 &color,Sampler &sampler)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="color")
			{
				string data=element->GetText();
				istringstream iss(data);
				iss >> color.x >> color.y >> color.z >> color.w;
			}
			else if (element->ValueStr()=="texture")
			{				
				sampler.name=element->Attribute("texture");
				sampler.uvCoords=element->Attribute("texcoord");
			}
		}
	}
}
void ColladaLoader::ReadGeometryLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="geometry")
			{
				string id=element->Attribute("id");
				geometryLibrary[id]=Geometry();
				ReadGeometry(element,geometryLibrary[id]);
			}
		}
	}
}
void ColladaLoader::ReadGeometry(TiXmlNode *parent,Geometry &geometry)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="mesh")
				ReadMesh(element,geometry.mesh);
		}
	}
}
void ColladaLoader::ReadMesh(TiXmlNode *parent,Mesh &mesh)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{
			element=node->ToElement();
			if (element->ValueStr()=="source")
			{
				string id=element->Attribute("id");
				sources[id]=Source();
				ReadSource(element,sources[id]);
			}
			else if (element->ValueStr()=="vertices")
			{
				ReadVertexData(element,mesh);
			}
			else if (element->ValueStr()=="triangles")
			{
				ReadIndexData(element,mesh);
			}
		}
	}
}
void ColladaLoader::ReadSource(TiXmlNode *parent,Source &source)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
			if (element->ValueStr()=="float_array")
			{
				ReadDataArray(element,source.dataArray);
			}
			else if (element->ValueStr()=="technique_common")
				ReadSource(element,source);
			else if (element->ValueStr()=="accessor")
			{
				ReadAccessor(element,source.accessor);
			}
		}
	}
}
void ColladaLoader::ReadDataArray(TiXmlNode *parent,DataArray &dataArray)
{
	TiXmlElement *element=parent->ToElement();
	int count;
	if (element->QueryIntAttribute("count",&count) != TIXML_SUCCESS)
		return;

	istringstream iss(element->GetText());
	dataArray.floatData.reserve(count);
	for (int i=0;i<count;i++)
	{
		float v;
		iss >> v;
		dataArray.floatData.push_back(v);
	}
}
void ColladaLoader::ReadAccessor(TiXmlNode *parent,Accessor &accesor)
{
	TiXmlNode *node;
	TiXmlElement *element=parent->ToElement();
	int count,stride,offset=0;
	if (element->QueryIntAttribute("count",&count) != TIXML_SUCCESS)
		return;
	if (element->QueryIntAttribute("stride",&stride) != TIXML_SUCCESS)
		return;
	element->QueryIntAttribute("offset",&offset);
	accesor.count=count;
	accesor.offset=offset;
	accesor.stride=stride;
	accesor.componentsOffset[0]=accesor.componentsOffset[1]=accesor.componentsOffset[2]=accesor.componentsOffset[3]=-1;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
			if (element->ValueStr()=="param")
			{
				string name=element->Attribute("name");
				if (name=="X")
					accesor.componentsOffset[0]=accesor.params.size();
				else if (name=="Y")
					accesor.componentsOffset[1]=accesor.params.size();
				else if (name=="Z")
					accesor.componentsOffset[2]=accesor.params.size();
				
				else if (name=="R")
					accesor.componentsOffset[0]=accesor.params.size();
				else if (name=="G")
					accesor.componentsOffset[1]=accesor.params.size();
				else if (name=="B")
					accesor.componentsOffset[2]=accesor.params.size();
				else if (name=="A")
					accesor.componentsOffset[3]=accesor.params.size();

				else if (name=="S")
					accesor.componentsOffset[0]=accesor.params.size();
				else if (name=="T")
					accesor.componentsOffset[1]=accesor.params.size();
				else if (name=="P")
					accesor.componentsOffset[2]=accesor.params.size();
				else if (name=="Q")
					accesor.componentsOffset[3]=accesor.params.size();

				accesor.params.push_back(name);
			}
		}
	}
}
void ColladaLoader::ReadVertexData(TiXmlNode *parent,Mesh &mesh)
{
	TiXmlNode *node;
	TiXmlElement *element=parent->ToElement();
	
	mesh.vertexId=element->Attribute("id");
	
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
			if (element->ValueStr()=="input")
			{
				ReadInputChannel(element,mesh.inputChannels);
			}
		}
	}
}
void ColladaLoader::ReadInputChannel(TiXmlNode *parent,vector<InputChannel> &inputChannels)
{
	TiXmlNode *node;
	TiXmlElement *element=parent->ToElement();
	inputChannels.push_back(InputChannel());
	InputChannel &ic=inputChannels[inputChannels.size()-1];
	
	string semantic=element->Attribute("semantic");
	ic.type=SemanticToInputType(semantic);
	string source=element->Attribute("source");
	ic.sourceName=source.substr(1);	
	if (element->QueryIntAttribute("offset",&ic.offset) != TIXML_SUCCESS)
		ic.offset=0;
	
	if ((ic.type==INPUT_TEXCOORD) || (ic.type==INPUT_COLOR))
	{
		if (element->QueryIntAttribute("set",&ic.index) != TIXML_SUCCESS)
			ic.index=-1;
	}
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
		}
	}
}
void ColladaLoader::ReadIndexData(TiXmlNode *parent, Mesh &mesh)
{
	TiXmlNode *node;
	TiXmlElement *element=parent->ToElement();

	vector<InputChannel> primInputChannels;
	int count;
	mesh.subMeshes.push_back(SubMesh());
	SubMesh &subMesh=mesh.subMeshes[mesh.subMeshes.size()-1];
	element->QueryIntAttribute("count",&count);
	subMesh.material=element->Attribute("material");	
	subMesh.numPrimtives=count;
	if (element->ValueStr()=="triangles")
		subMesh.primitiveType=PRIMITIVE_TRIANGLES;
		
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
			if (element->ValueStr()=="input")
				ReadInputChannel(element,primInputChannels);
			else if (element->ValueStr()=="p")
				ReadPrimitives(element,mesh,subMesh,primInputChannels,count);
		}
	}
}
void ColladaLoader::ReadPrimitives(TiXmlNode *parent,Mesh &mesh,SubMesh &subMesh,vector<InputChannel> primInputChannels,int count)
{	
	TiXmlElement *element=parent->ToElement();
	vector<int> indices;
	int offset=1;
	int vertexOffset=0;
	for (unsigned int i=0;i<primInputChannels.size();i++)
	{
		offset=max(offset,primInputChannels[i].offset+1);
		if (primInputChannels[i].type==INPUT_VERTEX)
			vertexOffset=primInputChannels[i].offset;
	}
	int expectedIndicesCount;
	if (subMesh.primitiveType==PRIMITIVE_TRIANGLES)
		expectedIndicesCount=count * 3;
	
	indices.reserve(expectedIndicesCount*offset);

	istringstream iss(element->GetText());
	while (!iss.eof())
	{
		int idx;
		iss >> idx;
		indices.push_back(idx);
	}
	for (unsigned int i=0;i<mesh.inputChannels.size();i++)
	{
		if (mesh.inputChannels[i].source==NULL)
		{
			mesh.inputChannels[i].source=&ResolveLibraryReference(sources,mesh.inputChannels[i].sourceName);
		}
	}
	for (unsigned int i=0;i<primInputChannels.size();i++)
	{
		if (primInputChannels[i].source==NULL)
		{
			if (primInputChannels[i].type!=INPUT_VERTEX)
				primInputChannels[i].source=&ResolveLibraryReference(sources,primInputChannels[i].sourceName);
		}
	}
	vector<int>::iterator idx=indices.begin();
	for (int i=0;i<count;i++)
	{
		int pointCount=0;
		if (subMesh.primitiveType==PRIMITIVE_TRIANGLES)
			pointCount=3;
		for (int j=0;j<pointCount;j++)
		{		
			int ind[15];
			for (int o=0;o<offset;o++)
			{
				ind[o]=*idx;
				idx++;
			}
			for (unsigned int k=0;k<mesh.inputChannels.size();k++)
			{
				GetDataFromChannel(mesh.inputChannels[k],ind[vertexOffset],mesh);
			}
			for (unsigned int k=0;k<primInputChannels.size();k++)
			{
				GetDataFromChannel(primInputChannels[k],ind[primInputChannels[k].offset],mesh);
			}
		}
	}
	
}
void ColladaLoader::GetDataFromChannel(InputChannel &ic,int index,Mesh &mesh)
{
	if (ic.type==INPUT_VERTEX)
		return;
	
	float *data=&ic.source->dataArray.floatData[0]+ic.offset+index*ic.source->accessor.stride;
	float values[4];
	for (int i=0;i<4;i++)
	{
		if (ic.source->accessor.componentsOffset[i]!= -1)
			values[i]=data[ic.source->accessor.componentsOffset[i]];
	}

	if (ic.type==INPUT_POSITION)	
		vertices.push_back(vec3(values[0],values[1],values[2]));
	else if (ic.type==INPUT_NORMAL)
		normals.push_back(vec3(values[0],values[1],values[2]));
}
void ColladaLoader::ReadSceneLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
			if (element->ValueStr()=="visual_scene")
			{
				Node *n=new Node();
				n->parent=NULL;
				n->id=element->Attribute("id");
				n->name=element->Attribute("name");
				nodeLibrary[n->id]=n;				
				ReadSceneNode(element,n);
			}
		}
	}
}
void ColladaLoader::ReadSceneNode(TiXmlNode *parent,Node *node)
{
	TiXmlNode *tnode;
	TiXmlElement *element;
	for (tnode=parent->FirstChild();tnode!=NULL;tnode=tnode->NextSibling())
	{
		if (tnode->Type()==TiXmlNode::ELEMENT)
		{			
			element=tnode->ToElement();
			if (element->ValueStr()=="node")
			{
				Node *n=new Node();				
				n->id=element->Attribute("id");
				n->name=element->Attribute("name");
				n->parent=NULL;
				if (node)
				{				
					node->children.push_back(n);					
					n->parent=node;					
				}
				else
					nodeLibrary[n->id]=n;

				ReadSceneNode(element,n);								
				continue;
			}
			
			if (element->ValueStr()=="instance_node")
			{
				node->nodeInstances.push_back(NodeInstance());
				string url=element->Attribute("url");
				node->nodeInstances[node->nodeInstances.size()-1].url=url.substr(1);
			}
			else if (element->ValueStr()=="instance_geometry")
			{
				string url=element->Attribute("url");
				GeometryInstance gi;
				gi.url=url.substr(1);
				ReadNodeGeometry(element,gi,node);
				node->geometryInstances.push_back(gi);
			}
			else if (element->ValueStr()=="lookat")
				ReadTransformation(element,node);
		}
	}
}
void ColladaLoader::ReadNodeGeometry(TiXmlNode *parent,GeometryInstance &gi,Node *node)
{
	TiXmlNode *tnode;
	TiXmlElement *element;

	
	for (tnode=parent->FirstChild();tnode!=NULL;tnode=tnode->NextSibling())
	{
		if (tnode->Type()==TiXmlNode::ELEMENT)
		{			
			element=tnode->ToElement();
			if (element->ValueStr()=="bind_material")
				ReadNodeGeometry(element,gi,node);
			else if (element->ValueStr()=="technique_common")
				ReadNodeGeometry(element,gi,node);
			else if (element->ValueStr()=="instance_material")
			{
				string symbol=element->Attribute("symbol");
				string target=element->Attribute("target");
				gi.materials[symbol]=SemanticMapping();
				SemanticMapping &sm=gi.materials[symbol];
				sm.materialName=target.substr(1);
				
				ReadMaterialVertexInputBinding(element,sm);
			}
		}
	}	
}
void ColladaLoader::ReadMaterialVertexInputBinding(TiXmlNode *parent,SemanticMapping &sm)
{
	TiXmlNode *tnode;
	TiXmlElement *element;


	for (tnode=parent->FirstChild();tnode!=NULL;tnode=tnode->NextSibling())
	{
		if (tnode->Type()==TiXmlNode::ELEMENT)
		{			
			element=tnode->ToElement();
			if (element->ValueStr()=="bind_vertex_input")
			{
				InputSemanticEntry is;
				string s=element->Attribute("semantic");
				string n=element->Attribute("input_semantic");
				is.type=SemanticToInputType(n);				
				if (element->QueryIntAttribute("input_set",&is.set) != TIXML_SUCCESS)
					is.set=-1;
				sm.inputMap[s]=is;
			}
		}
	}
}
void ColladaLoader::ReadScene(TiXmlNode *parent)
{
	TiXmlNode *tnode;
	TiXmlElement *element;


	for (tnode=parent->FirstChild();tnode!=NULL;tnode=tnode->NextSibling())
	{
		if (tnode->Type()==TiXmlNode::ELEMENT)
		{			
			element=tnode->ToElement();
			if (element->ValueStr()=="instance_visual_scene")
			{
				string url=element->Attribute("url");
				url=url.substr(1);
				map<string,Node*>::iterator i=nodeLibrary.find(url);
				if (i!=nodeLibrary.end())
					root=i->second;
			}
		}
	}
}
void ColladaLoader::ReadTransformation(TiXmlNode *parent,Node *node)
{
	TiXmlElement *element=parent->ToElement();
	Transform t;
	if (element->ValueStr()=="lookat")
		t.type=TRANSFORM_LOOKAT;
	else if (element->ValueStr()=="rotate")
		t.type=TRANSFORM_ROTATE;
	else if (element->ValueStr()=="translate")
		t.type=TRANSFORM_TRANSLATE;
	else if (element->ValueStr()=="scale")
		t.type=TRANSFORM_SCALE;
	else if (element->ValueStr()=="matrix")
		t.type=TRANSFORM_MATRIX;
	istringstream iss(parent->ToElement()->GetText());
	if (t.type==TRANSFORM_LOOKAT)
	{
		for (int i=0;i<9;i++)
			iss >> t.v[i];
	}
	else if (t.type==TRANSFORM_ROTATE)
	{
		for (int i=0;i<4;i++)
			iss >> t.v[i];
	}
	else if (t.type==TRANSFORM_TRANSLATE)
	{
		for (int i=0;i<3;i++)
			iss >> t.v[i];
	}
	else if (t.type==TRANSFORM_SCALE)
	{
		for (int i=0;i<3;i++)
			iss >> t.v[i];
	}
	else if (t.type==TRANSFORM_MATRIX)
	{
		for (int i=0;i<16;i++)
			iss >> t.v[i];
	}
	node->transformations.push_back(t);
}
void ColladaLoader::DeleteNodes(Node *node)
{
	for (unsigned int i=0;i<node->children.size();i++)
		DeleteNodes(node->children[i]);
	delete node;
}
TiXmlElement *ColladaLoader::GetChildElement(TiXmlNode *node,const string &elmName)
{
	TiXmlNode *n=node->FirstChild(elmName);
	if (n && n->Type()==TiXmlNode::ELEMENT)
		return n->ToElement();
	return NULL;
}
ColladaLoader::InputType ColladaLoader::SemanticToInputType(const string &semantic)
{
	if (semantic=="POSITION")
		return INPUT_POSITION;
	else if (semantic=="VERTEX")
		return INPUT_VERTEX;
	else if (semantic=="NORMAL")
		return INPUT_NORMAL;
	else if (semantic=="TEXCOORD")
		return INPUT_TEXCOORD;
	else if (semantic=="COLOR")
		return INPUT_COLOR;
	else
		return INPUT_UNKNOWN;
}