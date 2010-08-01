#ifndef GEOMETRY_H
#define GEOMETRY_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
class GeometryResource;
typedef boost::shared_ptr<GeometryResource> Geometry;

const unsigned int MAX_TEXTURES=6;
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
	Texture diffuseTexture;	
};
typedef boost::shared_ptr<MaterialResource> Material;
/**
* Defines an edge in a geometry.
*/
class FIRECUBE_API Edge
{
public:
	DWORD v[2];
};
/**
* Defines a face in a geometry.
*/
class FIRECUBE_API Face
{
public:
	Face();
	Face(DWORD v0,DWORD v1,DWORD v2);
	~Face();

	DWORD v[3];
	vec3 normal;
};

class FIRECUBE_API Surface
{
public:
	Material material;
	vector<Face> face;
	Buffer indexBuffer;
};

class FIRECUBE_API GeometryResource
{
public:
	~GeometryResource();
	/**
	* Calculates the bounding box of this model.
	*/
	void CalculateBoundingBox();
	/**
	* Returns the bounding box of this model.
	*/
	BoundingBox GetBoundingBox();
	/**
	* Applies a transformation to the model vertices.
	* @param transform The transformation matrix.
	*/
	void ApplyTransformation(mat4 &transform);
	/**
	* Searches for a marerial by name.
	* @param name The name of the material.
	* @return The material.
	*/
	Material GetMaterialByName(const string &name);	
	/**
	* Reduces the geometry by removing duplicated vertices.
	* @return The reduced geometry.
	*/
	Geometry Reduce();	
	/**
	* Calculates face and vertex normals.
	*/
	void CalculateNormals();	
	/**
	* Creates hard normals for the mesh.
	*/
	void CreateHardNormals();
	/**
	* Updates the buffers.
	*/
	void UpdateBuffers();

	vector<vec3> vertex;
	vector<vec3> normal;
	vector<Surface> surface;
	vector<Face> face;	
	vector<vec2> diffuseUV;
	Buffer vertexBuffer;	
	Buffer diffuseUVBuffer;
	Buffer normalBuffer;
	vector<Material> material;
	BoundingBox bbox;
};
}
#pragma warning(pop)

#endif