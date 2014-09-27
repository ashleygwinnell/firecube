#pragma once

#include "Core/Component.h"
#include "Math/Math.h"
#include "Math/Plane.h"

namespace FireCube
{

class PhysicsWorld;
class CollisionMesh;

class CollisionEntry
{
public:
	bool operator < (const CollisionEntry &entry);
	float time;
	float distance;
	vec3 normal;
	vec3 intersectionPoint;
};

class CharacterController : public Component
{
	friend PhysicsWorld;
	OBJECT(CharacterController)
public:
	CharacterController(Engine *engine);
	~CharacterController();
	void CheckCollisionWithMesh(const CollisionMesh &collisionMesh, mat4 transform);
	void CheckCollisionWithPlane(const Plane &plane, mat4 transform);
	void UpdateTransformedState();
	void UpdateFromTransformedState();
	void SetRadius(vec3 radius);
	vec3 GetRadius() const;
	void SetVelocity(vec3 velocity);
	vec3 GetVelocity() const;
	bool IsOnGround() const;

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
private:
	
	virtual void MarkedDirty();
	virtual void NodeChanged();

	
	vec3 velocity;
	vec3 radius;
	vec3 transformedPosition;
	vec3 transformedVelocity;
	vec3 normalizedTransformedVelocity;
	mat4 inverseTransposedTransform;
	mat4 scaleMatrix;

	bool collisionFound;
	float nearestTime;
	float nearestDistance;
	vec3 nearestNormal;
	vec3 nearestIntersectionPoint;
	std::vector<CollisionEntry> collisions;
	bool onGround;
	bool finishedMovement;
	WeakPtr<PhysicsWorld> physicsWorld;
};

}