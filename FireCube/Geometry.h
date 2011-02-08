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
	
	std::string name;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;	
	Texture diffuseTexture;
	Texture normalTexture;
};
/**
* A class representing a material.
*/
class FIRECUBE_API Material
{
public:
	/**
	* Creates the material.
	*/
	void Create();

	/**
	* Gets the name of this material.
	* @return The name of the material.
	*/
	std::string GetName();

	/**
	* Sets the name of the material.
	* @param name The name of the material.
	*/
	void SetName(const std::string &name);

	/**
	* Gets the ambient color of this material.
	* @return The ambient color of this material.
	*/
	vec4 GetAmbientColor();

	/**
	* Sets the ambient color of this material.
	* @param color The color to set the ambient to.
	*/
	void SetAmbientColor(vec4 color);

	/**
	* Gets the diffuse color of this material.
	* @return The diffuse color of this material.
	*/
	vec4 GetDiffuseColor();
	
	/**
	* Sets the diffuse color of this material.
	* @param color The color to set the diffuse to.
	*/
	void SetDiffuseColor(vec4 color);

	/**
	* Gets the specular color of this material.
	* @return The specular color of this material.
	*/
	vec4 GetSpecularColor();

	/**
	* Sets the specular color of this material.
	* @param color The color to set the specular to.
	*/
	void SetSpecularColor(vec4 color);

	/**
	* Gets the shininess factor of this material.
	* @return The shininess factor of this material.
	*/
	float GetShininess();

	/**
	* Sets the shininess factor of this material.
	* @param value The shininess factor.
	*/
	void SetShininess(float value);

	/**
	* Gets the diffuse texture of this material.
	* @return The diffuse texture of this material.
	*/
	Texture GetDiffuseTexture();

	/**
	* Sets the diffuse texture of this material.
	* @param texture The new diffuse texture map.
	*/
	void SetDiffuseTexture(Texture texture);

	/**
	* Gets the normal texture of this material.
	* @return The normal texture of this material.
	*/
	Texture GetNormalTexture();

	/**
	* Sets the normal texture of this material.
	* @param texture The new normal texture map.
	*/
	void SetNormalTexture(Texture texture);

	/**
	* @return True if points to an initialized resource, false otherwise.
	*/
	operator bool () const;

	/**
	* @return True of both objects points to the same resource, false otherwise.
	*/
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
	
	/**
	* The two indices of the edge.
	*/
	unsigned int v[2];
};
/**
* Defines a face in a geometry.
*/
class FIRECUBE_API Face
{
public:
	Face();

	/**
	* Constructs a face from three indices.
	* @param v0 The first index.
	* @param v1 The second index.
	* @param v2 The third index.
	*/
	Face(unsigned int v0,unsigned int v1,unsigned int v2);
	~Face();

	/**
	* The three indices of the face.
	*/
	unsigned int v[3];

	/**
	* The face's normal vector.
	*/
	vec3 normal;
};

/**
* Defines a surface in a geometry.
* A surface is a list of faces sharing the same material.
*/
class FIRECUBE_API Surface
{
public:	
	/**
	* The material associated with the surface.
	*/
	Material material;

	/**
	* The list of faces of this surface.
	*/
	std::vector<Face> face;

	/**
	* An index buffer used for rendering.
	*/
	Buffer indexBuffer;
};

class FIRECUBE_API GeometryResource
{
public:
	~GeometryResource();	

	std::vector<vec3> vertex;
	std::vector<vec3> normal;
	std::vector<vec3> tangent;
	std::vector<vec3> bitangent;
	std::vector<Surface> surface;
	std::vector<Face> face;	
	std::vector<vec2> diffuseUV;
	Buffer vertexBuffer;	
	Buffer diffuseUVBuffer;
	Buffer normalBuffer;
	Buffer tangentBuffer;
	Buffer bitangentBuffer;
	std::vector<Material> material;
	BoundingBox bbox;
};

/**
* A class representing a geometry.
* Geometries consists of vertices(position, normal, etc..), surfaces and materials.
*/
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
	Material GetMaterialByName(const std::string &name);	
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
	* Calculates tangents and bitangents.
	*/	
	void CalculateTangents();
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
	std::vector<vec3> &GetVertices();
	/**
	* Returns the normals of this geometry.
	*/
	std::vector<vec3> &GetNormals();
	/**
	* Returns the normals of this geometry.
	*/
	std::vector<vec3> &GetTangents();
	/**
	* Returns the normals of this geometry.
	*/
	std::vector<vec3> &GetBitangents();
	/**
	* Returns the faces of this geometry.
	*/
	std::vector<Face> &GetFaces();
	/**
	* Returns the diffuse uv of this geometry.
	*/
	std::vector<vec2> &GetDiffuseUV();
	/**
	* Returns the surfaces of this geometry.
	*/
	std::vector<Surface> &GetSurfaces();
	/**
	* Returns the materials of this geometry.
	*/
	std::vector<Material> &GetMaterials();

	/**
	* @return True if points to an initialized resource, false otherwise.
	*/
	operator bool () const;

	/**
	* @return True of both objects points to the same resource, false otherwise.
	*/
	bool operator== (const Geometry &geometry) const;
private:
	boost::shared_ptr<GeometryResource> resource;
};
}
#pragma warning(pop)

#endif