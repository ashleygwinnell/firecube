#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <string>
#include <vector>

#include "ThirdParty/GLEW/glew.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Rendering/GraphicsResource.h"

namespace FireCube
{

// Forward declarations.
class Node;
typedef std::shared_ptr<Node> NodePtr;
class RenderQueue;
class VertexBuffer;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
class IndexBuffer;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
class Geometry;
class ProgramUniformsList;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
class Material;
class Renderer;

/**
* Specifies the kind of primitives to render.
*/
enum PrimitiveType
{
	POINTS, LINES, TRIANGLES, TRIANGLE_STRIP, QUADS, LINE_LOOP, LINE_STRIP, TRIANGLE_FAN
};


/**
* A shared pointer to a Geometry.
*/
typedef std::shared_ptr<Geometry> GeometryPtr;

/**
* A shared pointer to a Material.
*/
typedef std::shared_ptr<Material> MaterialPtr;

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
* A class representing a geometry.
* Geometries consists of vertices(position, normal, etc..), faces and a material.
*/
class FIRECUBE_API Geometry : public GraphicsResource
{    
	friend class Renderer;	
public:
	Geometry(Renderer *renderer);

	~Geometry();

	/**
	* Updates the vertex buffers.
	* Copies the local copies of the vertices attributes (position, normals, etc) and indices
	* to the appropriate vertex buffers in this geometry.
	*/
	void Update();	
		
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
	* Sets the number of vertices to be rendered by this geometry.<br>	
	* @param vertexCount The number of vertices.
	*/
	void SetVertexCount(unsigned int vertexCount);

	/**
	* @return The number of vertices to be rendered by this geometry.	
	*/
	unsigned int GetVertexCount() const;

	/**
	* Clones this Geometry.
	* @return A new cloned Geometry of this one.
	*/
	GeometryPtr Clone();

	void SetVertexBuffer(VertexBufferPtr vertexBuffer);

	void SetIndexBuffer(IndexBufferPtr indexBuffer);

	void Render();

private:	
	VertexBufferPtr vertexBuffer;	
	IndexBufferPtr indexBuffer;
	MaterialPtr material;
	PrimitiveType primitiveType;
	unsigned int primitiveCount;
	unsigned int vertexCount;	
};
}
#pragma warning(pop)
