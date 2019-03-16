#pragma once

#include "Math/Math.h"
#include "Math/Octree.h"
#include "Core/Component.h"
#include "Core/Memory.h"
#include "Math/Plane.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

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


/**
* This class represents a collision shape component
*/
class CollisionShape : public Component, public OctreeItem<CollisionShape>
{
	FIRECUBE_OBJECT(CollisionShape)
public:
	CollisionShape(Engine *engine);
	~CollisionShape();
	
	/**
	* Sets the shape of the collision geometry from a mesh
	* @param mesh The mesh to use as a collision shape
	*/	
	void SetMesh(Mesh *mesh);
	
	/**
	* @returns The shape type of this collision shape(box, mesh, sphere, ...)
	*/
	CollisionShapeType GetShapeType() const;
	
	/**
	* Sets the shape of the collision geometry to be a plane
	* @param plane The plane to use as a collision shape 
	*/
	void SetPlane(const Plane &plane);
	
	/**
	* Sets the shape of the collision geometry to be a box
	* @param bbox The box to use as a collision shape 
	*/
	void SetBox(BoundingBox bbox);
	
	/**
	* Sets the shape of the collision geometry to be a sphere
	* @param radius The radius of the sphere 
	*/
	void SetSphere(float radius);

	/**
	* @returns The plane of this shape (if this shape is actually a plane)
	*/
	Plane GetPlane() const;
	
	/**
	* @returns The collision mesh of this shape (if this shape is actually a mesh)
	*/
	CollisionMesh *GetCollisionMesh();
	
	/**
	* @returns The box of this shape (if this shape is actually a box)
	*/
	BoundingBox GetBox() const;	
	
	/**
	* @returns The radius of this shape (if this shape is actually a sphere)
	*/
	float GetRadius() const;

	/**
	* @returns The world bounding box of the collision shape
	*/
	BoundingBox GetWorldBoundingBox();

	/**
	* Renders debug geometry for this shape
	* @param debugRenderer The debug renderer
	*/
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	/**
	* Clones this collision shape
	*/	
	Component *Clone() const;
	
	/**
	* @returns Whether this shape is used as a trigger
	*/
	bool IsTrigger() const;
	
	/**
	* Sets whether this shape is used as a trigger. Trigger collision shapes doen't affect the motion of objects they collide with but rather emit an event upon collision
	* @param isTrigger Whether to use this shape as a trigger
	*/
	void SetIsTrigger(bool isTrigger);
	
	/**
	* Sets whther this shape is owned by a rigid body (shapes associates with rigid bodies require different handling since they might be a part of a compound shape).
	* @param ownedByRigidBody Whether this shape is owned by a rigid body
	*/
	void SetOwnedByRigidBody(bool ownedByRigidBody);
	
	/**
	* @returns Whether this shape is owned by a ridig body
	*/
	bool IsOwnedByRigidBody() const;

private:
	
	CollisionShape(const CollisionShape &other);

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
	void UpdateWorldBoundingBox();
	void SetMesh(Mesh *mesh, SkeletonNode &skeletonNode, mat4 transformation);
	virtual void EnabledChanged() override;

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

};

}