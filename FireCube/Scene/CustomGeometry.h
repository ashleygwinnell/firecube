#pragma once

#include "Scene/Renderable.h"
#include "Rendering/VertexBuffer.h"

namespace FireCube
{

class CustomGeometry : public Renderable
{
	FIRECUBE_OBJECT(CustomGeometry)
public:
	CustomGeometry(Engine *engine);
	~CustomGeometry();
	void SetPrimitiveType(PrimitiveType primitiveType);
	void UpdateGeometry();
	void AddVertex(vec3 position);
	void SetNormal(vec3 normal);
	void SetTexCoord(vec2 texcoord);
	void SetMaterial(Material *material);
	void Clear();
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