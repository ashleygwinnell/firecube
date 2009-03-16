#ifndef MODELLOADERS_H
#define MODELLOADERS_H

#pragma warning(push)
#pragma warning(disable:4251)

#define PRIMARY 0x4d4d
#define EDIT3DS 0x3d3d
#define KEYF3DS 0xb000

#define NAMED_OBJECT 0x4000
#define OBJ_MESH 0x4100
#define MESH_VERTICES 0x4110
#define MESH_FACES 0x4120
#define MESH_MATERIAL 0x4130
#define MESH_TEX_VERT 0x4140

#define MATERIAL 0xafff
#define MAT_NAME 0xa000
#define MAT_AMBIENT 0xa010
#define MAT_DIFFUSE 0xa020
#define MAT_SPECULAR 0xa030
#define MAT_SHININESS 0xa040
#define MAT_TEXMAP 0xa200
#define MAT_TEXFLNM 0xa300
#define MAT_SHIN2PCT 0xa041
#define MAT_SHIN3PCT 0xa042

class M3dsLoader
{
public:
	M3dsLoader(ModelResource *model);
	bool Load(const string &filename);
private:
	DWORD ProcessChunk(char *buffer);
	ModelResource *model;
};

class ColladaLoader
{
public:
	class Source
	{
	public:
		vector <float> floatArray;
		DWORD stride;
	};
	ColladaLoader(ModelResource *model);
	bool Load(const string &filename);
private:	
	Material LoadMaterial(const string &name,map<string,pair<string,DWORD>> &vertexInput);
	void LoadMesh(string &id,mat4 transform, map<string,string> &materialMap,map<string,pair<string,DWORD>> &vertexInput);
	void LoadNodes();
	void LoadImageMap();
	vec4 ReadColor(daeElement *elm);
	bool ReadTexture(daeElement *elm,map<string,pair<string,DWORD>> &vertexInput,string &file,DWORD &unit);
	mat4 ReadTransformation(domNode *node);
	Source ReadSource(domSourceRef source);
	vector<vec4> SourceToVecArray(const Source &source);	
	Material GetMaterialByName(string &name);	
	DAE colladaDom;
	ModelResource *model;
	map<string,string> imageMap;
	string basepath;
};

#pragma warning(pop)
#endif