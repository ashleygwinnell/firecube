#pragma once

#include "Core/Component.h"

namespace FireCube
{

class CollisionShape;

class PhysicsWorld : public Component
{
	OBJECT(PhysicsWorld)
public:
	PhysicsWorld(Engine *engine);
	void AddCollisionShape(CollisionShape *collisionShape);
	void RemoveCollisionShape(CollisionShape *collisionShape);
	
private:
	std::vector<CollisionShape *> collisionShapes;
};

}