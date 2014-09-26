#pragma once

#include "Core/Component.h"

namespace FireCube
{

class CollisionShape;
class CharacterController;

class PhysicsWorld : public Component
{
	OBJECT(PhysicsWorld)
public:
	PhysicsWorld(Engine *engine);
	void AddCollisionShape(CollisionShape *collisionShape);
	void RemoveCollisionShape(CollisionShape *collisionShape);
	
	void AddCharacterController(CharacterController *characterController);
	void RemoveCharacterController(CharacterController *characterController);
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
private:
	void Update(float deltaTime);
	void MarkedDirty();
	void NodeChanged();

	std::vector<CollisionShape *> collisionShapes;
	std::vector<CharacterController *> characterControllers;
};

}