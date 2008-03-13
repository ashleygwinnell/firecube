#ifndef MESH_H
#define MESH_H

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

class FIRECUBE_API Material
{
public:
	Material();
	~Material();
	
	string name;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	GLuint tex;
};
class FIRECUBE_API Face
{
public:
	Face();
	~Face();

	DWORD v[3];
};
class FIRECUBE_API Mesh
{
public:
	Mesh();
	~Mesh();
	
	Material *material;
	vector<Face> face;
	Buffer indexBuffer;
};
class FIRECUBE_API Model
{
public:
	Model();
	~Model();
	
	bool Load3ds(const string &filename,const string &objName);
	DWORD ProcessChunk(char *buffer,const string &objName);
	Material *GetMaterialByName(const string &name);

	string name;
	vector<vec3> vertex;
	vector<Mesh> mesh;
	vector<Face> face;
	vector<Material> material;
	vector<vec2> uv;
	Buffer vertexBuffer;
	Buffer uvBuffer;
};
#pragma warning(pop)

#endif