#pragma once

#include "Scene/Renderable.h"
#include "Rendering/VertexBuffer.h"

namespace FireCube
{

/**
* This component is a Renderable whic is used to programatically construct geometries by adding vertices to it. 
*/
class CustomGeometry : public Renderable
{
	FIRECUBE_OBJECT(CustomGeometry)
public:
	CustomGeometry(Engine *engine);
	~CustomGeometry();
	
	/**
	* Sets the primitive type to render by this component
	* @param primitiveType The primitive type to render (triangles, lines, etc...)
	*/
	void SetPrimitiveType(PrimitiveType primitiveType);
	
	/**
	* Updates the underlying geomtry of this component. New vertices are not displayed until after this function is called
	*/
	void UpdateGeometry();
	
	/**
	* Adds a new vertex
	* @param position The position of the new vertex
	*/
	void AddVertex(vec3 position);
	
	/**
	* Sets the normal of the last added vertex
	* @param normal The normal to set
	*/
	void SetNormal(vec3 normal);
	
	/**
	* Sets the texture coordinate of the last added vertex
	* @param texcoord The texture coordinate to set
	*/
	void SetTexCoord(vec2 texcoord);
	
	/**
	* Sets the material to use to render the geometry
	* @param material The material to use
	*/
	void SetMaterial(Material *material);
	
	/**
	* Clears all the vertices
	*/
	void Clear();
	
	/**
	* Clones this component 
	*/
	virtual Component *Clone() const;
private:

	CustomGeometry(const CustomGeometry &other);

	class CustomVertex
	{
	public:
		vec3 position;
		vec3 normal;
		vec2 texcoord;
	};

	unsigned int currentVertex;
	VertexAttributeType vertexAttributes;
	virtual void UpdateWorldBoundingBox();
	BoundingBox boundingBox;
	Geometry *geometry;
	Material *material;
	std::vector<CustomVertex> vertices;
};

}