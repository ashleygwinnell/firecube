#pragma once

#include "Rendering/Renderable.h"

namespace FireCube
{

class CustomGeometry : public Renderable
{
public:
	CustomGeometry(Engine *engine);
	void SetPrimitiveType(PrimitiveType primitiveType);
	void UpdateGeometry();
	void AddVertex(vec3 position);
	void SetNormal(vec3 normal);
	void SetTexCoord(vec2 texcoord);
	void SetMaterial(MaterialPtr material);
private:
	class CustomVertex
	{
	public:
		vec3 position;
		vec3 normal;
		vec2 texcoord;
	};

	unsigned int currentVertex;
	unsigned int vertexAttributes;
	virtual void UpdateWorldBoundingBox();
	BoundingBox boundingBox;
	GeometryPtr geometry;
	MaterialPtr material;
	std::vector<CustomVertex> vertices;
};

}