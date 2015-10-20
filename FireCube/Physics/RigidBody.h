#pragma once

#include "Core/Component.h"
#include "Math/Math.h"

namespace FireCube
{

class PhysicsWorld;	

class RigidBody : public Component
{
	friend PhysicsWorld;
	FIRECUBE_OBJECT(RigidBody)
public:
	RigidBody(Engine *engine);
	~RigidBody();
		
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	void SetVelocity(vec3 velocity);
	vec3 GetVelocity() const;
	virtual Component *Clone() const;
private:

	RigidBody(const RigidBody &other);

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
		
	WeakPtr<PhysicsWorld> physicsWorld;
	vec3 velocity;
};

}