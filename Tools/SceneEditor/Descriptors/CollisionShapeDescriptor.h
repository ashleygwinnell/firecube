#pragma once

#include "ComponentDescriptor.h"

class CollisionShapeDescriptor : public ComponentDescriptor
{
public:
	CollisionShapeDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~CollisionShapeDescriptor();
	
	FireCube::CollisionShapeType GetShapeType() const;
	void SetMesh(const std::string &meshFilename, FireCube::Engine *engine);
	std::string GetMeshFilename() const;
	void SetPlane(FireCube::Plane plane);
	FireCube::Plane GetPlane() const;
	void SetBox(FireCube::BoundingBox box);
	FireCube::BoundingBox GetBox() const;
	void SetSphere(float radius);
	float GetRadius() const;
	void SetIsTrigger(bool isTrigger);
	bool IsTrigger() const;
private:
	FireCube::CollisionShapeType collisionShapeType;
	std::string meshFilename;
	FireCube::Plane plane;
	FireCube::BoundingBox box;
	float radius;
	bool isTrigger;
};
