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

	tempMap.clear();
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
				ReadTriangles(element,mesh);
			}
			else if (element->ValueStr()=="polylist")
			{
				ReadPolylist(element,mesh);
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
			ic.index=0;
	}
	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
		}
	}
}
void ColladaLoader::ReadTriangles(TiXmlNode *parent, Mesh &mesh)
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
void ColladaLoader::ReadPolylist(TiXmlNode *parent, Mesh &mesh)
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
	subMesh.primitiveType=PRIMITIVE_POLYLIST;

	for (node=parent->FirstChild();node!=NULL;node=node->NextSibling())
	{
		if (node->Type()==TiXmlNode::ELEMENT)
		{			
			element=node->ToElement();
			if (element->ValueStr()=="input")
				ReadInputChannel(element,primInputChannels);
			else if (element->ValueStr()=="vcount")
				ReadVCount(element,subMesh);
			else if (element->ValueStr()=="p")
				ReadPrimitives(element,mesh,subMesh,primInputChannels,count);
		}
	}
}
void ColladaLoader::ReadVCount(TiXmlNode *parent,SubMesh &subMesh)
{	
	TiXmlElement *element=parent->ToElement();
	subMesh.vcount.reserve(subMesh.numPrimtives);

	istringstream iss(element->GetText());
	while (!iss.eof())
	{
		int idx;
		iss >> idx;
		subMesh.vcount.push_back(idx);
	}
}
unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed )
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 

		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
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
	int expectedIndicesCount=0;
	if (subMesh.primitiveType==PRIMITIVE_TRIANGLES)
		expectedIndicesCount=count * 3;
	else if (subMesh.primitiveType==PRIMITIVE_POLYLIST)
	{
		for (unsigned int i=0;i<subMesh.vcount.size();i++)
			expectedIndicesCount+=subMesh.vcount[i];
	}


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
		else if (subMesh.primitiveType==PRIMITIVE_POLYLIST)
			pointCount=subMesh.vcount[i];
		if (pointCount!=3)
		{
			Logger::Write("Error in collada loader, polygons with more than 3 vertices are not implemented yet.\n");
			continue;
		}
		for (int j=0;j<pointCount;j++)
		{		
			int ind[15];
			for (int o=0;o<offset;o++)
			{
				ind[o]=*idx;
				idx++;
			}
			unsigned int h=MurmurHash2(ind,offset*4,0x1234);
			map<unsigned int,unsigned int>::iterator iter=tempMap.find(h);
			if (iter==tempMap.end())
			{
				for (unsigned int k=0;k<mesh.inputChannels.size();k++)
				{
					GetDataFromChannel(mesh.inputChannels[k],ind[vertexOffset],mesh);
				}
				for (unsigned int k=0;k<primInputChannels.size();k++)
				{
					GetDataFromChannel(primInputChannels[k],ind[primInputChannels[k].offset],mesh);
				}
				tempMap[h]=mesh.vertices.size()-1;
				subMesh.indices.push_back(mesh.vertices.size()-1);
			}
			else
				subMesh.indices.push_back(iter->second);


		}
	}

}
void ColladaLoader::GetDataFromChannel(InputChannel &ic,int index,Mesh &mesh)
{
	if (ic.type==INPUT_VERTEX)
		return;

	float *data=&ic.source->dataArray.floatData[0]+ic.source->accessor.offset+index*ic.source->accessor.stride;
	float values[4];
	for (int i=0;i<4;i++)
	{
		if (ic.source->accessor.componentsOffset[i]!= -1)
			values[i]=data[ic.source->accessor.componentsOffset[i]];
	}

	if (ic.type==INPUT_POSITION)	
		mesh.vertices.push_back(vec3(values[0],values[1],values[2]));
	else if (ic.type==INPUT_NORMAL)
		mesh.normals.push_back(vec3(values[0],values[1],values[2]));
	else if (ic.type==INPUT_TEXCOORD)
	{
		if (ic.index<4)
			mesh.texcoords[ic.index].push_back(vec2(values[0],1.0f-values[1]));
	}

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
			else if (element->ValueStr()=="rotate")
				ReadTransformation(element,node);
			else if (element->ValueStr()=="translate")
				ReadTransformation(element,node);
			else if (element->ValueStr()=="scale")
				ReadTransformation(element,node);
			else if (element->ValueStr()=="matrix")
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
mat4 ColladaLoader::CalculateTranformation(vector<Transform> &transformations)
{
	mat4 ret;
	for (unsigned int i=0;i<transformations.size();i++)
	{
		Transform &t=transformations[i];
		mat4 temp;
		if (t.type==TRANSFORM_TRANSLATE)
		{			
			temp.Translate(t.v[0],t.v[1],t.v[2]);
			ret*=temp;
		}
		else if (t.type==TRANSFORM_SCALE)
		{			
			temp.Scale(t.v[0],t.v[1],t.v[2]);
			ret*=temp;
		}
		else if (t.type==TRANSFORM_ROTATE)
		{			
			float angle=(float)(-t.v[3]/180.0f*PI);

			temp.Rotate(vec4(t.v[0],t.v[1],t.v[2],angle));
			ret*=temp;
		}
		else if (t.type==TRANSFORM_MATRIX)
		{						
			for (unsigned int j=0;j<16;j++)
				temp.m[j]=t.v[j];			
			ret*=temp;
		}
		else if (t.type==TRANSFORM_LOOKAT)
		{			
			vec3 pos(t.v[0],t.v[1],t.v[2]);
			vec3 dst(t.v[3],t.v[4],t.v[5]);
			vec3 up(t.v[6],t.v[7],t.v[8]);
			up.Normalize();
			temp.LookAt(pos,dst,up);
			ret*=temp;
		}
	}
	return ret;
}
vec3 ColladaLoader::GetTranslation(vector<Transform> &transformations)
{
	vec3 ret(0,0,0);
	for (unsigned int i=0;i<transformations.size();i++)
	{
		Transform &t=transformations[i];
		if (t.type==TRANSFORM_TRANSLATE)
		{			
			ret+=vec3(t.v[0],t.v[1],t.v[2]);			
		}		
	}
	return ret;
}
vec3 ColladaLoader::GetRotation(vector<Transform> &transformations)
{
	vec3 ret(0,0,0);
	for (unsigned int i=0;i<transformations.size();i++)
	{
		Transform &t=transformations[i];		
		if (t.type==TRANSFORM_ROTATE)
		{			
			float angle=(float)(-t.v[3]/180.0f*PI);			
			float c=cos(angle), s=sin(angle);
			float tt=1-c;
			float tx=tt*t.v[0];
			float ty=tt*t.v[1];
			float a11=tx*t.v[0]+c, a21=tx*t.v[1]+s*t.v[2], a31=tx*t.v[2]-s*t.v[1];
			float a12=tx*t.v[1]-s*t.v[2], a22=ty*t.v[1]+c, a32=ty*t.v[2]+s*t.v[0];
			float a13=tx*t.v[2]+s*t.v[1], a23=ty*t.v[2]-s*t.v[0], a33=tt*t.v[2]*t.v[2]+c;

			float angle_x,angle_y,angle_z;
			angle_y = asin(-a31);
			a31=cos(angle_y);
			if ( a31>1e-15 || -a31<-1e-15 ) {
				angle_x = atan2(a32, a33);
				angle_z = atan2(a21, a11);
			} else {
				angle_x = atan2(a12, a22);
				angle_z = 0;
			}
			ret+=vec3(angle_x,angle_y,angle_z);
		}		
	}
	return ret;
}
vec3 ColladaLoader::GetScale(vector<Transform> &transformations)
{
	vec3 ret(1,1,1);
	for (unsigned int i=0;i<transformations.size();i++)
	{
		Transform &t=transformations[i];
		if (t.type==TRANSFORM_SCALE)
		{					
			ret.x*=t.v[0];
			ret.y*=t.v[1];
			ret.z*=t.v[2];			
		}		
	}
	return ret;
}
mat4 ColladaLoader::GetTransformMatrix(vector<Transform> &transformations)
{
	mat4 ret;
	for (unsigned int i=0;i<transformations.size();i++)
	{
		Transform &t=transformations[i];
		mat4 temp;
		if (t.type==TRANSFORM_MATRIX)
		{						
			for (unsigned int j=0;j<16;j++)
				temp.m[j]=t.v[j];			
			ret*=temp;
		}
		else if (t.type==TRANSFORM_LOOKAT)
		{			
			vec3 pos(t.v[0],t.v[1],t.v[2]);
			vec3 dst(t.v[3],t.v[4],t.v[5]);
			vec3 up(t.v[6],t.v[7],t.v[8]);
			up.Normalize();
			temp.LookAt(pos,dst,up);
			ret*=temp;
		}
	}
	return ret;
}
void ColladaLoader::ApplySemanticMapping(Sampler &sampler,SemanticMapping &table)
{
	map<string,InputSemanticEntry>::iterator iter=table.inputMap.find(sampler.uvCoords);
	sampler.uvId=-1;
	if (iter!=table.inputMap.end())
	{
		if (iter->second.type==INPUT_TEXCOORD)
			sampler.uvId=iter->second.set;
	}			
}
string ColladaLoader::GetTextureFileNameFromSampler(Effect &effect,Sampler &sampler)
{
	string cur=sampler.name;
	map<string,EffectParam>::iterator i;
	while ((i=effect.effectParams.find(cur))!=effect.effectParams.end())
	{
		cur=i->second.reference;
	}
	map<string,ColladaLoader::Image>::iterator imgi=imageLibrary.find(cur);
	if (imgi==imageLibrary.end())
		return "";
	else
	{
		return imgi->second.initFrom;
	}
}
string ColladaLoader::FixFileName(string &filename)
{
	string::size_type i=0;
	string file=filename;
	while ((file[i]=='.') || (file[i]=='/'))
		i++;
	file=file.substr(i);
	i=file.find("file://");
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
	return file;
}
FireCube::Node ColladaLoader::GenerateSceneGraph(Node *node)
{
	FireCube::Node ret(node->name);
	vec3 translation=GetTranslation(node->transformations);
	vec3 rotation=GetRotation(node->transformations);
	vec3 scale=GetScale(node->transformations);
	mat4 transformation=GetTransformMatrix(node->transformations);
	ret.SetTranslation(translation);
	ret.SetRotation(rotation);
	ret.SetScale(scale);
	ret.SetMatrixTransformation(transformation);
	for (unsigned int i=0;i<node->geometryInstances.size();i++)
	{
		GeometryInstance &gi=node->geometryInstances[i];
		map<string,Geometry>::iterator iter=geometryLibrary.find(gi.url);
		if (iter==geometryLibrary.end())
			continue;
		Geometry &geom=iter->second;
		Mesh &mesh=geom.mesh;
		FireCube::Geometry geometry(new GeometryResource);
		ret.AddGeometry(geometry);
		geometry->vertex=mesh.vertices;
		for (unsigned int j=0;j<geometry->vertex.size();j++)
		{
			//geometry->vertex[j]=geometry->vertex[j]*transformation;
			if (upDirection==X_UP)
			{
				std::swap(geometry->vertex[j].x,geometry->vertex[j].y);
				geometry->vertex[j].x*=-1;
			}
			else if (upDirection==Z_UP)
			{
				std::swap(geometry->vertex[j].y,geometry->vertex[j].z);
				geometry->vertex[j].z*=-1;
			}
		}
		geometry->normal=mesh.normals;
		for (unsigned int j=0;j<geometry->normal.size();j++)
		{
			//geometry->normal[j]=geometry->normal[j].TransformNormal(transformation);
			if (upDirection==X_UP)
			{
				std::swap(geometry->normal[j].x,geometry->normal[j].y);
				geometry->normal[j].x*=-1;
			}
			else if (upDirection==Z_UP)
			{
				std::swap(geometry->normal[j].y,geometry->normal[j].z);
				geometry->normal[j].z*=-1;
			}
			geometry->normal[j].Normalize();
		}		
		unsigned int writePos=0;
		for (unsigned int j=0;j<4;j++)
		{
			if (mesh.texcoords[j].size()>0)
			{
				if (writePos!=j)
					mesh.texcoords[writePos]=mesh.texcoords[j];
				writePos++;
			}
		}

		unsigned int vertexIndex=0;
		for (unsigned int j=0;j<mesh.subMeshes.size();j++)
		{
			SubMesh &subMesh=mesh.subMeshes[j];
			map<string,SemanticMapping>::iterator iter=gi.materials.find(subMesh.material);
			SemanticMapping *table=NULL;
			if (iter!=gi.materials.end())
				table=&iter->second;
			Material &mat=materialLibrary[table->materialName];
			Effect &effect=effectLibrary[mat.effect];			
			geometry->surface.push_back(FireCube::Surface());
			FireCube::Surface &surface=geometry->surface.back();			
			FireCube::Material fmat=FireCube::Material(new MaterialResource());
			geometry->material.push_back(fmat);
			fmat->ambient=effect.ambientColor;
			fmat->diffuse=effect.diffuseColor;
			fmat->specular=effect.specularColor;			
			fmat->name=subMesh.material;
			surface.material=fmat;
			if (table)
			{
				ApplySemanticMapping(effect.ambientSampler,*table);
				ApplySemanticMapping(effect.diffuseSampler,*table);
				ApplySemanticMapping(effect.specularSampler,*table);
			}			
			if (effect.diffuseSampler.name!="")
			{
				unsigned int map=0;
				fmat->diffuseTexture=Renderer::GetTextureManager().Create(GetTextureFileNameFromSampler(effect,effect.diffuseSampler));
				if (effect.diffuseSampler.uvId!=-1)				
					map=effect.diffuseSampler.uvId;				
				else
				{
					string::iterator s=effect.diffuseSampler.uvCoords.begin();
					while (s!=effect.diffuseSampler.uvCoords.end() && (*s<'0' || *s>'9'))
						s++;
					map=0;
					while (s!=effect.diffuseSampler.uvCoords.end() && (*s>='0' && *s<='9'))
					{
						map=map*10+(*s-'0');
						s++;
					}
				}
				geometry->diffuseUV=mesh.texcoords[map];
			}			
			if (subMesh.primitiveType==PRIMITIVE_TRIANGLES)
			{
				for (int p=0;p<subMesh.numPrimtives;p++)
				{				
					Face f;										
					f.v[0]=subMesh.indices[p*3+0];
					f.v[1]=subMesh.indices[p*3+1];
					f.v[2]=subMesh.indices[p*3+2];
					surface.face.push_back(f);
					geometry->face.push_back(f);
				}
			}
			else if (subMesh.primitiveType==PRIMITIVE_POLYLIST)
			{
				for (int p=0;p<subMesh.numPrimtives;p++)
				{
					if (subMesh.vcount[p]!=3)
						continue;
					Face f;										
					f.v[0]=subMesh.indices[p*3+0];
					f.v[1]=subMesh.indices[p*3+1];
					f.v[2]=subMesh.indices[p*3+2];
					surface.face.push_back(f);
					geometry->face.push_back(f);
				}
			}

		}
		geometry->UpdateBuffers();
	}
	for (unsigned int i=0;i<node->children.size();i++)
	{
		FireCube::Node c=GenerateSceneGraph(node->children[i]);
		ret.AddChild(c);
	}

	return ret;
}
FireCube::Node ColladaLoader::GenerateSceneGraph()
{
	return GenerateSceneGraph(root);
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