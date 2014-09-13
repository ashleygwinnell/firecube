#include <sstream>

#include "Physics/CollisionShape.h"
#include "Scene/Node.h"
#include "Physics/PhysicsWorld.h"

using namespace FireCube;

CollisionShape::CollisionShape(Engine *engine) : Component(engine), physicsWorld(nullptr)
{

}

CollisionShape::~CollisionShape()
{
	if (physicsWorld)
	{
		physicsWorld->RemoveCollisionShape(this);
	}
}

void CollisionShape::SetShapeType(CollisionShapeType type)
{
	this->type = type;
}

CollisionShapeType CollisionShape::GetShapeType() const
{
	return type;
}

void CollisionShape::MarkedDirty()
{

}
void CollisionShape::NodeChanged()
{
	if (node)
	{
		physicsWorld = node->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddCollisionShape(this);
	}
	else
	{

	}
}