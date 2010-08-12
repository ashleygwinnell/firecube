#ifndef GEOMETRY_H
#define GEOMETRY_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
class GeometryResource;
class Geometry;
class Node;
namespace Renderer
{
	void FIRECUBE_API Render(Geometry geometry);
	void FIRECUBE_API Render(Node node);
}

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
class FIRECUBE_API Material
{
public:
	/*
	* Creates the material.
	*/
	void Create();
	string GetName();
	void SetName(const string &name);
	vec4 GetAmbientColor();
	void SetAmbientColor(vec4 color);
	vec4 GetDiffuseColor();
	void SetDiffuseColor(vec4 color);
	vec4 GetSpecularColor();
	void SetSpecularColor(vec4 color);
	float GetShininess();
	void SetShininess(float value);
	Texture GetDiffuseTexture();
	void SetDiffuseTexture(Texture texture);

	operator bool () const;
	bool operator== (const Material &material) const;
private:
	boost::shared_ptr<MaterialResource> resource;
};
/**
* Defines an edge in a geometry.
*/
class FIRECUBE_API Edge
{
public:
	unsigned int v[2];
};
/**
* Defines a face in a geometry.
*/
class FIRECUBE_API Face
{
public:
	Face();
	Face(unsigned int v0,unsigned int v1,unsigned int v2);
	~Face();

	unsigned int v[3];
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
class FIRECUBE_API Geometry
{
	friend void Renderer::Render(Geometry geometry);
	friend void Renderer::Render(Node node);
public:
	/**
	* Creates a new geometry
	*/
	void Create();
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
	/**
	* Returns the vertices of this geometry.
	*/
	vector<vec3> &GetVertices();
	/**
	* Returns the normals of this geometry.
	*/
	vector<vec3> &GetNormals();
	/**
	* Returns the faces of this geometry.
	*/
	vector<Face> &GetFaces();
	/**
	* Returns the diffuse uv of this geometry.
	*/
	vector<vec2> &GetDiffuseUV();
	/**
	* Returns the surfaces of this geometry.
	*/
	vector<Surface> &GetSurfaces();
	/**
	* Returns the materials of this geometry.
	*/
	vector<Material> &GetMaterials();

	operator bool () const;
	bool operator== (const Geometry &geometry) const;
private:
	boost::shared_ptr<GeometryResource> resource;
};
}
#pragma warning(pop)

#endif