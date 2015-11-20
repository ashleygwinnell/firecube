#include "CollisionShapeDescriptor.h"

using namespace FireCube;

CollisionShapeDescriptor::CollisionShapeDescriptor() : ComponentDescriptor(ComponentType::COLLISION_SHAPE)
{

}

void CollisionShapeDescriptor::CreateComponent(FireCube::Node *node, FireCube::Engine *engine)
{
	auto collisionShape = node->CreateComponent<CollisionShape>();
	collisionShape->SetIsTrigger(isTrigger);

	switch (collisionShapeType)
	{
	case FireCube::CollisionShapeType::TRIANGLE_MESH:
		collisionShape->SetMesh(engine->GetResourceCache()->GetResource<Mesh>(meshFilename));
		break;
	case FireCube::CollisionShapeType::PLANE:
		collisionShape->SetPlane(plane);
		break;
	case FireCube::CollisionShapeType::BOX:
		collisionShape->SetBox(box);
		break;
	case FireCube::CollisionShapeType::SPHERE:
		collisionShape->SetSphere(radius);
	default:
		break;
	}

	component = collisionShape;
}

ComponentDescriptor *CollisionShapeDescriptor::Clone()
{
	CollisionShapeDescriptor *clone = new CollisionShapeDescriptor();
	clone->collisionShapeType = collisionShapeType;
	clone->meshFilename = meshFilename;
	clone->plane = plane;
	clone->box = box;
	clone->isTrigger = isTrigger;

	return clone;
}

CollisionShapeDescriptor::~CollisionShapeDescriptor()
{

}

FireCube::CollisionShapeType CollisionShapeDescriptor::GetShapeType() const
{
	return collisionShapeType;
}

void CollisionShapeDescriptor::SetMesh(const std::string &meshFilename, FireCube::Engine *engine)
{
	this->meshFilename = meshFilename;
	collisionShapeType = CollisionShapeType::TRIANGLE_MESH;
	if (component)
	{
		((CollisionShape *)component)->SetMesh(engine->GetResourceCache()->GetResource<Mesh>(meshFilename));
	}
}

std::string CollisionShapeDescriptor::GetMeshFilename() const
{
	return meshFilename;
}

void CollisionShapeDescriptor::SetPlane(FireCube::Plane plane)
{
	this->plane = plane;
	collisionShapeType = CollisionShapeType::PLANE;
	if (component)
	{
		((CollisionShape *)component)->SetPlane(plane);
	}
}

FireCube::Plane CollisionShapeDescriptor::GetPlane() const
{
	return plane;
}

void CollisionShapeDescriptor::SetBox(FireCube::BoundingBox box)
{
	this->box = box;
	collisionShapeType = CollisionShapeType::BOX;
	if (component)
	{
		((CollisionShape *)component)->SetBox(box);
	}
}

FireCube::BoundingBox CollisionShapeDescriptor::GetBox() const
{
	return box;
}

void CollisionShapeDescriptor::SetSphere(float radius)
{
	this->radius = radius;
	collisionShapeType = CollisionShapeType::SPHERE;
	if (component)
	{
		((CollisionShape *)component)->SetSphere(radius);
	}
}

float CollisionShapeDescriptor::GetRadius() const
{
	return radius;
}

void CollisionShapeDescriptor::SetIsTrigger(bool isTrigger)
{
	this->isTrigger = isTrigger;
}

bool CollisionShapeDescriptor::IsTrigger() const
{
	return isTrigger;
}