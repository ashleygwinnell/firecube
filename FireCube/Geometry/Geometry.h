#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <string>
#include <vector>

#include "ThirdParty/GLEW/glew.h"
#include "Math/Math.h"
#include "Math/BoundingBox.h"
#include "Rendering/GraphicsResource.h"
#include "Math/Ray.h"

namespace FireCube
{

// Forward declarations.
class Node;
class RenderQueue;
class VertexBuffer;
class IndexBuffer;
class Geometry;
class ProgramUniformsList;
class Texture;
class Renderer;

/**
* Specifies the kind of primitives to render.
*/
enum PrimitiveType
{
	POINTS, LINES, TRIANGLES, TRIANGLE_STRIP, QUADS, LINE_LOOP, LINE_STRIP, TRIANGLE_FAN
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
	//MaterialPtr GetMaterial();

	/**
	* Sets the material of this geometry.
	* @param material The material to assign to this geometry.
	*/
	//void SetMaterial(MaterialPtr material);

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
	* Clones this Geometry.
	* @return A new cloned Geometry of this one.
	*/
	Geometry *Clone();

	void SetVertexBuffer(VertexBuffer *vertexBuffer);
	VertexBuffer *GetVertexBuffer();
	void SetIndexBuffer(IndexBuffer *indexBuffer);
	IndexBuffer *GetIndexBuffer();

	void Render();

	bool IntersectRay(const Ray &ray, float &distance, vec3 &normal) const;

private:	
	VertexBuffer *vertexBuffer;	
	IndexBuffer *indexBuffer;	
	PrimitiveType primitiveType;
	unsigned int primitiveCount;	
};
}
#pragma warning(pop)
