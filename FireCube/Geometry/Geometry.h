#ifndef GEOMETRY_H
#define GEOMETRY_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
class Node;
typedef boost::shared_ptr<Node> NodePtr;
class RenderQueue;
class Buffer;
typedef boost::shared_ptr<Buffer> BufferPtr;
class Geometry;
class ProgramUniformsList;
class Texture;
typedef boost::shared_ptr<Texture> TexturePtr;

/**
* Specifies the kind of primitives to render.
*/
enum PrimitiveType
{
	POINTS, LINES, TRIANGLES, TRIANGLE_STRIP, QUADS, LINE_LOOP, TRIANGLE_FAN
};


/**
* A shared pointer to a GeometryResource.
*/
typedef boost::shared_ptr<Geometry> GeometryPtr;

namespace Renderer
{
void FIRECUBE_API Render(GeometryPtr geometry);
void FIRECUBE_API Render(NodePtr node);
void FIRECUBE_API Render(NodePtr node, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);
void FIRECUBE_API Render(RenderQueue &renderQueue, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);
void FIRECUBE_API Render(RenderQueue &renderQueue);
}

/**
* A class representing a material.
*/
class FIRECUBE_API Material
{
public:
    Material();
    ~Material();


    /**
    * Gets the name of this material.
    * @return The name of the material.
    */
    std::string GetName() const;

    /**
    * Sets the name of the material.
    * @param name The name of the material.
    */
    void SetName(const std::string &name);

    /**
    * Gets the ambient color of this material.
    * @return The ambient color of this material.
    */
    vec4 GetAmbientColor() const;

    /**
    * Sets the ambient color of this material.
    * @param color The color to set the ambient to.
    */
    void SetAmbientColor(const vec4 &color);

    /**
    * Gets the diffuse color of this material.
    * @return The diffuse color of this material.
    */
    vec4 GetDiffuseColor() const;

    /**
    * Sets the diffuse color of this material.
    * @param color The color to set the diffuse to.
    */
    void SetDiffuseColor(const vec4 &color);

    /**
    * Gets the specular color of this material.
    * @return The specular color of this material.
    */
    vec4 GetSpecularColor() const;

    /**
    * Sets the specular color of this material.
    * @param color The color to set the specular to.
    */
    void SetSpecularColor(const vec4 &color);

    /**
    * Gets the shininess factor of this material.
    * @return The shininess factor of this material.
    */
    float GetShininess() const;

    /**
    * Sets the shininess factor of this material.
    * @param value The shininess factor.
    */
    void SetShininess(float value);

    /**
    * Gets the diffuse texture of this material.
    * @return The diffuse texture of this material.
    */
    TexturePtr GetDiffuseTexture();

    /**
    * Sets the diffuse texture of this material.
    * @param texture The new diffuse texture map.
    */
    void SetDiffuseTexture(TexturePtr texture);

    /**
    * Gets the normal texture of this material.
    * @return The normal texture of this material.
    */
    TexturePtr GetNormalTexture();

    /**
    * Sets the normal texture of this material.
    * @param texture The new normal texture map.
    */
    void SetNormalTexture(TexturePtr texture);

private:

    std::string name;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    TexturePtr diffuseTexture;
    TexturePtr normalTexture;
};

/**
* A shared pointer to a MaterialResource.
*/
typedef boost::shared_ptr<Material> MaterialPtr;

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
    Face(unsigned int v0, unsigned int v1, unsigned int v2);
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
* A class representing a geometry.
* Geometries consists of vertices(position, normal, etc..), faces and a material.
*/
class FIRECUBE_API Geometry
{
    friend void Renderer::Render(GeometryPtr geometry);
    friend void Renderer::Render(NodePtr node);
    friend void Renderer::Render(NodePtr node, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);
	friend void Renderer::Render(RenderQueue &renderQueue, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);
    friend void Renderer::Render(RenderQueue &renderQueue);
public:
    ~Geometry();

    /**
    * Calculates the bounding box of this model.
    */
    void CalculateBoundingBox();
    /**
    * Returns the bounding box of this model.
    */
    BoundingBox GetBoundingBox() const;
    /**
    * Applies a transformation to the model vertices.
    * @param transform The transformation matrix.
    */
    void ApplyTransformation(const mat4 &transform);
    /**
    * Reduces the geometry by removing duplicated vertices.
    * @return The reduced geometry.
    */
    GeometryPtr Reduce() const;
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
    * Returns the indices of this geometry.
    */
    std::vector<unsigned int> &GetIndices();
    
	/**
	* Copies the triangles defined in the faces of this geometry to the index buffer.
	*/
	void CopyFacesToIndexBuffer();

	/**
    * Returns the material of this geometry.
    */
    MaterialPtr GetMaterial();

	/**
	* Sets the material of this geometry.
	* @param material The material to assign to this geometry.
	*/
	void SetMaterial(MaterialPtr material);

	/**
	* Sets the primitive type to be rendered by this geometry.
	* @param primitiveType The type of the primitive.
	*/
	void SetPrimitiveType(PrimitiveType primitiveType);

	/**
	* @return The primitive type of this geometry.
	*/
	PrimitiveType GetPrimitiveType() const;

	/**
	* Sets the amount of primitives to be rendered by this geometry.
	* @param primitiveCount The number of primitives.
	*/
	void SetPrimitiveCount(unsigned int primitiveCount);

	/**
	* @return The amount of primitives rendered by this geometry.
	*/
	unsigned int GetPrimitiveCount() const;

	/**
	* Sets the number of elements contained in the index buffer.
	* @param indexCount The number of indices.
	*/
	void SetIndexCount(unsigned int indexCount);

	/**
	* @return The number of elements contained in the index buffer.	
	*/
	unsigned int GetIndexCount() const;

private:
    std::vector<vec3> vertex;
    std::vector<vec3> normal;
    std::vector<vec3> tangent;
    std::vector<vec3> bitangent;    
    std::vector<Face> face;
	std::vector<unsigned int> indices;
    std::vector<vec2> diffuseUV;
    BufferPtr vertexBuffer;
    BufferPtr diffuseUVBuffer;
    BufferPtr normalBuffer;
    BufferPtr tangentBuffer;
    BufferPtr bitangentBuffer;
	BufferPtr indexBuffer;
    MaterialPtr material;
	PrimitiveType primitiveType;
	unsigned int primitiveCount;
	unsigned int indexCount;
    BoundingBox bbox;
};
}
#pragma warning(pop)

#endif