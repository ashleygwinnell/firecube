#pragma once

#include "Core/Component.h"
#include "Math/Math.h"

namespace FireCube
{

class PhysicsWorld;	

class RigidBody : public Component
{
	friend PhysicsWorld;
	OBJECT(RigidBody)
public:
	RigidBody(Engine *engine);
	~RigidBody();
		
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	void SetVelocity(vec3 velocity);
	vec3 GetVelocity() const;
private:

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
		
	WeakPtr<PhysicsWorld> physicsWorld;
	vec3 velocity;
};

}