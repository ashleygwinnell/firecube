#pragma once

#include "Core/Component.h"
#include "Math/Math.h"
#include "Math/Octree.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

template <class T>
class OctreeNode;
class PhysicsWorld;	
class CollisionShape;

enum class RigidBodyType
{
	DYNAMIC, STATIC
};

class RigidBody : public Component, public OctreeItem<RigidBody>
{
	friend PhysicsWorld;
	FIRECUBE_OBJECT(RigidBody)
public:
	RigidBody(Engine *engine);
	~RigidBody();

	void SetBodyType(RigidBodyType bodyType);
	RigidBodyType GetBodyType() const;
		
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	void SetVelocity(vec3 velocity);
	vec3 GetAngularVelocity() const;
	void SetAngularVelocity(vec3 angularVelocity);
	vec3 GetVelocity() const;
	quat GetRotation() const;
	vec3 GetPosition() const;
	void SetRotation(quat rotation);
	void SetPosition(vec3 position);
	virtual Component *Clone() const;
	vec3 PointToLocal(vec3 p);
	vec3 VectorToLocal(vec3 v);
	vec3 PointToWorld(vec3 p);
	vec3 VectorToWorld(vec3 v);

	void ApplyForce(vec3 force, vec3 relativePos);
	void ApplyLocalForce(vec3 force, vec3 v);
	void ApplyImpulse(vec3 impulse, vec3 relativePos);
	void ApplyLocalImpulse(vec3 impulse, vec3 v);
	void Integrate(float t);
	void UpdateInertiaWorld(bool force = false);
	void UpdateWorldBoundingBox();
	void UpdateMassProperties();
	void SetForce(vec3 force);
	vec3 GetForce() const;
	void SetTorque(vec3 torque);
	vec3 GetTorque() const;
	void SetMass(float mass);
	float GetInvMass() const;
	mat3 GetInvInertiaWorld() const;
	vec3 GetVelocityAtWorldPoint(vec3 worldPoint) const;
	std::vector<CollisionShape *> &GetCollisionShapes();
	void UpdateCollisionShapes();
	BoundingBox GetWorldBoundingBox();
	
private:

	RigidBody(const RigidBody &other);

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
		
	WeakPtr<PhysicsWorld> physicsWorld;
	std::vector<CollisionShape *> shapes;
	vec3 velocity;
	vec3 force;
	float mass;
	float invMass;
	RigidBodyType bodyType;
	vec3 torque;
	quat rotation;
	vec3 angularVelocity;
	vec3 inertia;
	vec3 invInertia;
	mat3 invInertiaWorld;
	vec3 position;
	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;
	
public:
	vec3 vlambda;
	vec3 wlambda;

};

}