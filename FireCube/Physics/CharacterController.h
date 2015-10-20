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
	FIRECUBE_OBJECT(CharacterController)
public:
	CharacterController(Engine *engine);
	~CharacterController();
	void SetRadius(float radius);
	float GetRadius() const;
	void SetHeight(float height);
	float GetHeight() const;
	void SetVelocity(vec3 velocity);
	vec3 GetVelocity() const;
	bool IsOnGround() const;
	void SetContactOffset(float contactOffset);
	float GetContactOffset() const;
	void SetStepOffset(float stepOffset);
	float GetStepOffset() const;

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
	float contactOffset;
	float stepOffset;
	
	std::vector<CollisionContact> contacts;
	bool onGround;	
	WeakPtr<PhysicsWorld> physicsWorld;
};

}