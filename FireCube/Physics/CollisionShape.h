#pragma once

#include "Math/Math.h"
#include "Core/Component.h"
#include "Core/Memory.h"
#include "Math/Plane.h"

namespace FireCube
{

class PhysicsWorld;
class StaticModel;

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
};


class CollisionShape : public Component
{
	OBJECT(CollisionShape)
public:
	CollisionShape(Engine *engine);
	~CollisionShape();
	
	void SetShapeType(CollisionShapeType type);
	void FromStaticModel(StaticModel *staticModel);
	CollisionShapeType GetShapeType() const;
	void SetPlane(const Plane &plane);
	void SetBox(vec3 size);

	Plane GetPlane() const;
	CollisionMesh *GetMesh();

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
private:
	virtual void MarkedDirty();
	virtual void NodeChanged();

	CollisionShapeType type;
	WeakPtr<PhysicsWorld> physicsWorld;

	SharedPtr<CollisionMesh> mesh;
	Plane plane;
	vec3 boxSize;
	
};

}