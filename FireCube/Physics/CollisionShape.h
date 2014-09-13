#pragma once

#include "Core/Component.h"

namespace FireCube
{

class PhysicsWorld;

enum class CollisionShapeType
{
	TRIANGLE_MESH
};

class CollisionShape : public Component
{
	OBJECT(CollisionShape)
public:
	CollisionShape(Engine *engine);
	~CollisionShape();
	
	void SetShapeType(CollisionShapeType type);
	CollisionShapeType GetShapeType() const;
private:
	virtual void MarkedDirty();
	virtual void NodeChanged();

	CollisionShapeType type;
	PhysicsWorld *physicsWorld;
};

}