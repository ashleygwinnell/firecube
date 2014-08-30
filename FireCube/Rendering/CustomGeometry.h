#pragma once

#include "Rendering/Renderable.h"
#include "Rendering/VertexBuffer.h"

namespace FireCube
{

class CustomGeometry : public Renderable
{
public:
	CustomGeometry(Engine *engine);
	~CustomGeometry();
	void SetPrimitiveType(PrimitiveType primitiveType);
	void UpdateGeometry();
	void AddVertex(vec3 position);
	void SetNormal(vec3 normal);
	void SetTexCoord(vec2 texcoord);
	void SetMaterial(Material *material);
private:
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