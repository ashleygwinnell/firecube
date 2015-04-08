#pragma once

#include "Core/Component.h"

namespace FireCube
{

class CollisionShape;
class CharacterController;
class RigidBody;

class PhysicsWorld : public Component
{
	OBJECT(PhysicsWorld)
public:
	PhysicsWorld(Engine *engine);
	void AddCollisionShape(CollisionShape *collisionShape);
	void RemoveCollisionShape(CollisionShape *collisionShape);
	
	void AddCharacterController(CharacterController *characterController);
	void RemoveCharacterController(CharacterController *characterController);

	void AddRigidBody(RigidBody *rigidBody);
	void RemoveRigidBody(RigidBody *rigidBody);

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	virtual Component *Clone() const;
private:

	PhysicsWorld(const PhysicsWorld &other);

	void Update(float deltaTime);
	void MarkedDirty();
	void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);

	std::vector<CollisionShape *> collisionShapes;
	std::vector<CharacterController *> characterControllers;
	std::vector<RigidBody *> rigidBodies;
};

}