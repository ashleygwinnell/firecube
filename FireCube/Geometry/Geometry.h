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
#include "Rendering/RenderingTypes.h"
#include "Math/Ray.h"
#include "Core/Memory.h"

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

enum class GeometryType
{
	STATIC, SKINNED
};

/**
* A class representing a geometry.
* Geometries consists of vertices(position, normal, etc..), faces and a material.
*/
class FIRECUBE_API Geometry : public GraphicsResource, public RefCounted
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
	* Sets the primitive type to be rendered by this geometry.
	* @param primitiveType The type of the primitive.
	*/
	void SetPrimitiveType(PrimitiveType primitiveType);

	/**
	* @return The primitive type of this geometry.
	*/
	PrimitiveType GetPrimitiveType() const;

	/**
	* Sets the geometry type of this geometry.
	* @param primitiveType The type of the primitive.
	*/
	void SetGeometryType(GeometryType geometryType);

	/**
	* @return The geometry type of this geometry.
	*/
	GeometryType GetGeometryType() const;

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

	/**
	* Sets the vertex buffer of this geometry
	* @param vertexBuffer The vertex buffer to set
	*/
	void SetVertexBuffer(VertexBuffer *vertexBuffer);
	
	/**
	* @return The vertex buffer of this geometry
	*/
	VertexBuffer *GetVertexBuffer();
	
	/**
	* Sets the index buffer of this geometry
	* @param indexBuffer The index buffer to set
	*/
	void SetIndexBuffer(IndexBuffer *indexBuffer);
	
	/**
	* @return The index buffer of this geometry
	*/
	IndexBuffer *GetIndexBuffer();

	/**
	* Renders the geometry
	*/
	void Render();

	/**
	* Intersects a ray with this geometry
	* @param ray The intersection ray
	* @param distance Output parameter where the intersection distance between the ray and the geometry is stored
	* @param normal Output parameter where the normal at the intersection point is stored
	* @return Whether an intersection occured between the ray and the geometry
	*/
	bool IntersectRay(const Ray &ray, float &distance, vec3 &normal) const;

private:	
	VertexBuffer *vertexBuffer;	
	IndexBuffer *indexBuffer;	
	PrimitiveType primitiveType;
	GeometryType geometryType;
	unsigned int primitiveCount;	
};
}
#pragma warning(pop)
