#ifndef MESH_H
#define MESH_H

#pragma warning(push)
#pragma warning(disable:4251)

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
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
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
	Object();
	string name;
	vector<vec3> vertex;
	vector<vec3> normal;
	vector<Mesh> mesh;
	vector<Face> face;
	vector<vector<vec2>> uv;
	Buffer vertexBuffer;
	vector<Buffer> uvBuffer;
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
	* Loads a file.
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
	/**
	* Creates hard normals for the mesh.
	*/
	void CreateHardNormals();
	/**
	* Updates the buffers.
	*/
	void UpdateBuffers();
	/**
	* Searches for a marerial by name.
	* @param name The name of the material.
	* @return The material.
	*/
	Material GetMaterialByName(const string &name);	

	vector<Object> object;
	vector<Material> material;	
	string name;
private:	
	
};

#pragma warning(pop)

#endif