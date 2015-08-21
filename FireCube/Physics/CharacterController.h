#pragma once

#include "Core/Component.h"
#include "Math/Math.h"
#include "Math/Plane.h"
#include "Physics/PhysicsWorld.h"

namespace FireCube
{

class PhysicsWorld;
class CollisionMesh;

class CollisionResult;

class CharacterController : public Component
{
	friend PhysicsWorld;
	OBJECT(CharacterController)
public:
	CharacterController(Engine *engine);
	~CharacterController();
	void CheckCollisionWithMesh(const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result) const;
	void CheckCollisionWithPlane(const Plane &plane, mat4 transform, CollisionResult &result) const;
	void UpdateTransformedState();
	void UpdateFromTransformedState();
	void SetRadius(float radius);
	float GetRadius() const;
	void SetHeight(float height);
	float GetHeight() const;
	void SetVelocity(vec3 velocity);
	vec3 GetVelocity() const;
	bool IsOnGround() const;

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	virtual Component *Clone() const;
private:
	
	CharacterController(const CharacterController &other);

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
	
	vec3 position;
	vec3 velocity;
	float radius;
	float height;

	bool collisionFound;
	float nearestTime;
	float nearestDistance;
	vec3 nearestNormal;
	vec3 nearestIntersectionPoint;
	std::vector<CollisionContact> contacts;
	bool onGround;
	bool finishedMovement;
	WeakPtr<PhysicsWorld> physicsWorld;
};

}