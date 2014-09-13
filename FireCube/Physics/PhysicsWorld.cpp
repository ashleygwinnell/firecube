#include <algorithm>

#include "Physics/PhysicsWorld.h"
using namespace FireCube;

PhysicsWorld::PhysicsWorld(Engine *engine) : Component(engine)
{

}

void PhysicsWorld::AddCollisionShape(CollisionShape *collisionShape)
{
	collisionShapes.push_back(collisionShape);
}

void PhysicsWorld::RemoveCollisionShape(CollisionShape *collisionShape)
{
	collisionShapes.erase(std::remove(collisionShapes.begin(), collisionShapes.end(), collisionShape), collisionShapes.end());	
}