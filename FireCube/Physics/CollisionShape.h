#pragma once

#include "Math/Math.h"
#include "Core/Component.h"
#include "Core/Memory.h"
#include "Math/Plane.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

template <class T>
class OctreeNode;
class PhysicsWorld;
class Mesh;
class SkeletonNode;

enum class CollisionShapeType
{
	TRIANGLE_MESH, PLANE, BOX, SPHERE
};

class CollisionTriangle
{
public:
	CollisionTriangle();
	CollisionTriangle(vec3 p0, vec3 p1, vec3 p2);

	vec3 p0, p1, p2;
};

class CollisionMesh : public RefCounted
{
public:
	std::vector<CollisionTriangle> triangles;
	BoundingBox boundingBox;
};


class CollisionShape : public Component
{
	FIRECUBE_OBJECT(CollisionShape)
public:
	CollisionShape(Engine *engine);
	~CollisionShape();
		
	void SetMesh(Mesh *mesh);
	CollisionShapeType GetShapeType() const;
	void SetPlane(const Plane &plane);
	void SetBox(BoundingBox bbox);
	void SetSphere(float radius);

	Plane GetPlane() const;
	CollisionMesh *GetCollisionMesh();
	BoundingBox GetBox() const;	
	float GetRadius() const;

	BoundingBox GetWorldBoundingBox();

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);	
	Component *Clone() const;
	
	bool IsTrigger() const;
	void SetIsTrigger(bool isTrigger);

	void SetOwnedByRigidBody(bool ownedByRigidBody);
	bool IsOwnedByRigidBody() const;

	OctreeNode<CollisionShape> *GetOctreeNode();
	void SetOctreeNode(OctreeNode<CollisionShape> *octreeNode);
	bool GetOctreeNodeNeedsUpdate() const;
	void SetOctreeNodeNeedsUpdate(bool octreeNodeNeedsUpdate);

private:
	
	CollisionShape(const CollisionShape &other);

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
	void UpdateWorldBoundingBox();
	void SetMesh(Mesh *mesh, SkeletonNode &skeletonNode, mat4 transformation);

	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;
	bool isTrigger;
	
	CollisionShapeType type;
	WeakPtr<PhysicsWorld> physicsWorld;

	SharedPtr<CollisionMesh> collisionMesh;	
	Plane plane;
	BoundingBox box;
	float radius;

	bool ownedByRigidBody;

	OctreeNode<CollisionShape> *octreeNode;
	bool octreeNodeNeedsUpdate;
	
};

}