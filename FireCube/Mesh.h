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
#define MAT_SHIN2PCT 0xa041
#define MAT_SHIN3PCT 0xa042
class ModelResource;
typedef ResourceManager<ModelResource> ModelManager;
typedef boost::shared_ptr<ModelResource> Model;
const unsigned int MAX_TEXTURES=8;
/**
* Stores information about a material.
*/
class FIRECUBE_API MaterialResource
{
public:
	MaterialResource();
	~MaterialResource();
	
	string name;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	Texture texture[MAX_TEXTURES];
	Program program;
};
typedef boost::shared_ptr<MaterialResource> Material;
/**
* Defines an edge in a mesh.
*/
class FIRECUBE_API Edge
{
public:
	DWORD v[2];
};
/**
* Defines a face in a mesh.
*/
class FIRECUBE_API Face
{
public:
	Face();
	~Face();

	DWORD v[3];
	vec3 normal;
};
/**
* Stores a set of geometry sharing the same material.
*/
class FIRECUBE_API Mesh
{
public:
	Mesh();
	~Mesh();
	
	Material material;
	vector<Face> face;
	Buffer indexBuffer;
};
/**
* A 3d object.<br>
* The object is composed of various meshes.
*/
class FIRECUBE_API Object
{
public:
	string name;
	vector<vec3> vertex;
	vector<vec3> normal;
	vector<Mesh> mesh;
	vector<Face> face;
	vector<vec2> uv;
	Buffer vertexBuffer;
	Buffer uvBuffer;
	Buffer normalBuffer;
};
/**
* A class representing a model resource.
*/
class FIRECUBE_API ModelResource
{	
public:
	ModelResource();
	~ModelResource();
	
	/**
	* Loads a 3ds file.
	* @param filename The file to load.
	*/
	bool Load(const string &filename);	
	/**
	* Reduces the model by removing duplicated vertices.
	* @return The reduced mesh.
	*/
	Model Reduce();	
	/**
	* Calculates face and vertex normals.
	*/
	void CalculateNormals();
	/**
	* Applies a transformation to the model vertices.
	* @param transform The transformation matrix.
	*/
	void ApplyTransformation(mat4 &transform);
	/**
	* Sets a program to each material.
	* @param program The program to use.
	*/
	void SetProgram(const Program &program);
	vector<Object> object;
	vector<Material> material;	
	string name;
private:
	DWORD ProcessChunk(char *buffer);
	Material GetMaterialByName(const string &name);	
};

#pragma warning(pop)

#endif