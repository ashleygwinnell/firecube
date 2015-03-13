#pragma once

#include "Math/Math.h"
#include "Core/Component.h"
#include "Core/Memory.h"
#include "Math/Plane.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

class PhysicsWorld;
class Mesh;

enum class CollisionShapeType
{
	TRIANGLE_MESH, PLANE, BOX
};

class CollisionTriangle
{
public:
	CollisionTriangle(vec3 p1, vec3 p2, vec3 p3);

	vec3 p1, p2, p3;
};

class CollisionMesh : public RefCounted
{
public:
	std::vector<CollisionTriangle> triangles;
	BoundingBox boundingBox;
};


class CollisionShape : public Component
{
	OBJECT(CollisionShape)
public:
	CollisionShape(Engine *engine);
	~CollisionShape();
		
	void SetMesh(Mesh *mesh);
	CollisionShapeType GetShapeType() const;
	void SetPlane(const Plane &plane);
	void SetBox(BoundingBox bbox);

	Plane GetPlane() const;
	CollisionMesh *GetMesh();

	BoundingBox GetWorldBoundingBox();

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);	
private:
	virtual void MarkedDirty();
	virtual void NodeChanged();
	void UpdateWorldBoundingBox();

	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;

	CollisionShapeType type;
	WeakPtr<PhysicsWorld> physicsWorld;

	SharedPtr<CollisionMesh> mesh;
	Plane plane;
	BoundingBox shapeBoundingBox;
	
};

}