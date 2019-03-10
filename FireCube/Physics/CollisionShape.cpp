#include <sstream>

#include "Physics/CollisionShape.h"
#include "Scene/Node.h"
#include "Physics/PhysicsWorld.h"
#include "Geometry/Mesh.h"
#include "Geometry/Geometry.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/DebugRenderer.h"
#include "Scene/Scene.h"
#include "Physics/RigidBody.h"

using namespace FireCube;

CollisionTriangle::CollisionTriangle()
{

}

CollisionTriangle::CollisionTriangle(vec3 p0, vec3 p1, vec3 p2) : p0(p0), p1(p1), p2(p2)
{

}

CollisionShape::CollisionShape(Engine *engine) : Component(engine), physicsWorld(nullptr), worldBoundingBoxChanged(false), isTrigger(false), collisionMesh(nullptr), ownedByRigidBody(false)
{

}

CollisionShape::CollisionShape(const CollisionShape &other) : Component(other), worldBoundingBoxChanged(true), type(other.type), physicsWorld(other.physicsWorld), collisionMesh(other.collisionMesh), plane(other.plane),
															  box(other.box), isTrigger(other.isTrigger), ownedByRigidBody(other.ownedByRigidBody)
{
	
}

CollisionShape::~CollisionShape()
{

}

CollisionShapeType CollisionShape::GetShapeType() const
{
	return type;
}

void CollisionShape::MarkedDirty()
{
	worldBoundingBoxChanged = true;
	MarkForOctreeReinsertion();	
}

void CollisionShape::NodeChanged()
{
	Node *n = node;

	while (n)
	{
		auto rigidBody = n->GetComponent<RigidBody>();
		if (rigidBody)
		{
			rigidBody->UpdateCollisionShapes();
			break;
		}

		n = n->GetParent();
	}
}

void CollisionShape::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		if (physicsWorld.Expired() == false)
		{
			physicsWorld->RemoveCollisionShape(this);
		}				
	}

	if (scene)
	{
		physicsWorld = scene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddCollisionShape(this);
	}
}

CollisionMesh *CollisionShape::GetCollisionMesh()
{
	return collisionMesh;
}

void CollisionShape::SetMesh(Mesh *mesh)
{
	type = CollisionShapeType::TRIANGLE_MESH;
	collisionMesh = new CollisionMesh;
	if (!mesh)
		return;

	SetMesh(mesh, mesh->GetSkeletonRoot(), mat4::IDENTITY);
	
	MarkedDirty();
}

void CollisionShape::SetMesh(Mesh *mesh, SkeletonNode &skeletonNode, mat4 transformation)
{
	mat4 modelTransformation = transformation * skeletonNode.transformation;
	for (auto meshIndex : skeletonNode.meshes)
	{				
		Geometry *geometry = mesh->GetGeometries()[meshIndex];
		VertexBuffer *vertexBuffer = geometry->GetVertexBuffer();
		IndexBuffer *indexBuffer = geometry->GetIndexBuffer();
		auto &vertexData = vertexBuffer->GetShadowData();
		auto &indexData = indexBuffer->GetShadowData();
		if (indexData.empty() == false && vertexData.empty() == false)
		{
			unsigned int positionOffset = vertexBuffer->GetVertexAttributeOffset(VertexAttributeType::POSITION);
			for (unsigned int i = 0; i < indexData.size() / sizeof(unsigned int); i += 3)
			{
				unsigned int i0 = *((unsigned int *)&indexData[(i + 0) * sizeof(unsigned int)]);
				unsigned int i1 = *((unsigned int *)&indexData[(i + 1) * sizeof(unsigned int)]);
				unsigned int i2 = *((unsigned int *)&indexData[(i + 2) * sizeof(unsigned int)]);

				vec3 pos0 = modelTransformation * *((vec3 *)&vertexData[i0 * vertexBuffer->GetVertexSize() + positionOffset]);
				vec3 pos1 = modelTransformation * *((vec3 *)&vertexData[i1 * vertexBuffer->GetVertexSize() + positionOffset]);
				vec3 pos2 = modelTransformation * *((vec3 *)&vertexData[i2 * vertexBuffer->GetVertexSize() + positionOffset]);

				collisionMesh->triangles.push_back(CollisionTriangle(pos0, pos1, pos2));
				collisionMesh->boundingBox.Expand(pos0);
				collisionMesh->boundingBox.Expand(pos1);
				collisionMesh->boundingBox.Expand(pos2);
			}
		}
	}

	for (auto &c : skeletonNode.children)
	{
		SetMesh(mesh, c, modelTransformation);
	}
}


void CollisionShape::SetPlane(const Plane &plane)
{
	type = CollisionShapeType::PLANE;
	this->plane = plane;	
	MarkedDirty();
}

Plane CollisionShape::GetPlane() const
{
	return plane;
}

void CollisionShape::SetBox(BoundingBox bbox)
{
	type = CollisionShapeType::BOX;
	box = bbox;
	vec3 bmin = bbox.GetMin();
	vec3 bmax = bbox.GetMax();	
	collisionMesh = new CollisionMesh;
	// Front
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmax.x, bmin.y, bmax.z), vec3(bmax.x, bmax.y, bmax.z)));
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmax.x, bmax.y, bmax.z), vec3(bmin.x, bmax.y, bmax.z)));
	// Back
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmin.z), vec3(bmin.x, bmax.y, bmin.z), vec3(bmax.x, bmax.y, bmin.z)));
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmin.z), vec3(bmax.x, bmax.y, bmin.z), vec3(bmax.x, bmin.y, bmin.z)));
	// Right
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmax.x, bmin.y, bmax.z), vec3(bmax.x, bmin.y, bmin.z), vec3(bmax.x, bmax.y, bmin.z)));
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmax.x, bmin.y, bmax.z), vec3(bmax.x, bmax.y, bmin.z), vec3(bmax.x, bmax.y, bmax.z)));
	// Left
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmin.x, bmax.y, bmax.z), vec3(bmin.x, bmax.y, bmin.z)));
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmin.x, bmax.y, bmin.z), vec3(bmin.x, bmin.y, bmin.z)));
	// Top
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmax.y, bmax.z), vec3(bmax.x, bmax.y, bmax.z), vec3(bmax.x, bmax.y, bmin.z)));
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmax.y, bmax.z), vec3(bmax.x, bmax.y, bmin.z), vec3(bmin.x, bmax.y, bmin.z)));
	// Bottom
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmin.x, bmin.y, bmin.z), vec3(bmax.x, bmin.y, bmin.z)));
	collisionMesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmax.x, bmin.y, bmin.z), vec3(bmax.x, bmin.y, bmax.z)));

	MarkedDirty();
}

void CollisionShape::SetSphere(float radius)
{
	type = CollisionShapeType::SPHERE;
	this->radius = radius;

	MarkedDirty();
}

void CollisionShape::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	if (type == CollisionShapeType::BOX)
	{
		vec3 p[8];
		p[0] = node->GetWorldTransformation() * collisionMesh->triangles[0].p0;
		p[1] = node->GetWorldTransformation() * collisionMesh->triangles[0].p1;
		p[2] = node->GetWorldTransformation() * collisionMesh->triangles[0].p2;
		p[3] = node->GetWorldTransformation() * collisionMesh->triangles[1].p2;
		p[4] = node->GetWorldTransformation() * collisionMesh->triangles[2].p0;
		p[5] = node->GetWorldTransformation() * collisionMesh->triangles[2].p1;
		p[6] = node->GetWorldTransformation() * collisionMesh->triangles[2].p2;
		p[7] = node->GetWorldTransformation() * collisionMesh->triangles[3].p2;
		debugRenderer->AddLine(p[0], p[1], vec3(0, 1, 0));
		debugRenderer->AddLine(p[1], p[2], vec3(0, 1, 0));
		debugRenderer->AddLine(p[2], p[3], vec3(0, 1, 0));
		debugRenderer->AddLine(p[3], p[0], vec3(0, 1, 0));
		debugRenderer->AddLine(p[4], p[5], vec3(0, 1, 0));
		debugRenderer->AddLine(p[5], p[6], vec3(0, 1, 0));
		debugRenderer->AddLine(p[6], p[7], vec3(0, 1, 0));
		debugRenderer->AddLine(p[7], p[4], vec3(0, 1, 0));
		debugRenderer->AddLine(p[0], p[4], vec3(0, 1, 0));
		debugRenderer->AddLine(p[3], p[5], vec3(0, 1, 0));
		debugRenderer->AddLine(p[1], p[7], vec3(0, 1, 0));
		debugRenderer->AddLine(p[2], p[6], vec3(0, 1, 0));
	}
	else if (type == CollisionShapeType::TRIANGLE_MESH)
	{
		for (auto &t : collisionMesh->triangles)
		{
			vec3 p0 = node->GetWorldTransformation() * t.p0;
			vec3 p1 = node->GetWorldTransformation() * t.p1;
			vec3 p2 = node->GetWorldTransformation() * t.p2;
			debugRenderer->AddLine(p0, p1, vec3(0, 1, 0));
			debugRenderer->AddLine(p1, p2, vec3(0, 1, 0));
			debugRenderer->AddLine(p2, p0, vec3(0, 1, 0));
		}		
	}
	else if (type == CollisionShapeType::PLANE)
	{
		const float cellSize = 1.0f;
		const int numCells = 20;
		const float halfSize = numCells * cellSize * 0.5f;
		vec3 up(0, 0, 1);
		vec3 normalizedPlaneNormal = plane.GetNormal().Normalized();
		if (Dot(up, normalizedPlaneNormal) >= 1.0f - 0.0001f)
			up = vec3(0, 1, 0);
		up = (up - normalizedPlaneNormal * Dot(up, normalizedPlaneNormal)).Normalized();
		vec3 right = Cross(up, plane.GetNormal()).Normalized();
		vec3 center = plane.GetNormal() * plane.GetDistance();
		for (int i = 0; i < numCells + 1; ++i)
		{
			vec3 p1 = node->GetWorldTransformation() * (center + ((float) i * cellSize - halfSize) * up - halfSize * right);
			vec3 p2 = node->GetWorldTransformation() * (center + ((float) i * cellSize - halfSize) * up + halfSize * right);
			vec3 p3 = node->GetWorldTransformation() * (center + ((float) i * cellSize - halfSize) * right - halfSize * up);
			vec3 p4 = node->GetWorldTransformation() * (center + ((float) i * cellSize - halfSize) * right + halfSize * up);
			debugRenderer->AddLine(p1, p2, vec3(0, 1, 0));
			debugRenderer->AddLine(p3, p4, vec3(0, 1, 0));
		}
	}
	else if (type == CollisionShapeType::SPHERE)
	{
		debugRenderer->AddSphere(node->GetWorldPosition(), radius, 16, 16, vec3(0.0f, 1.0f, 0.0f));
	}
}

BoundingBox CollisionShape::GetWorldBoundingBox()
{
	if (worldBoundingBoxChanged)
	{
		worldBoundingBoxChanged = false;
		UpdateWorldBoundingBox();
	}
	return worldBoundingBox;
}

void CollisionShape::UpdateWorldBoundingBox()
{
	BoundingBox boundingBox;

	switch (type)
	{
	case CollisionShapeType::TRIANGLE_MESH:
		boundingBox = collisionMesh->boundingBox;
		break;
	case CollisionShapeType::PLANE:
		boundingBox = BoundingBox(vec3(-10e6), vec3(10e6)); // In case of a plane, return a large bounding box
		break;
	case CollisionShapeType::BOX:
		boundingBox = box;
		break;
	case CollisionShapeType::SPHERE:
		boundingBox = BoundingBox(vec3(-radius), vec3(radius));
		break;
	default:
		break;
	}

	worldBoundingBox = boundingBox;
	worldBoundingBox.Transform(node->GetWorldTransformation());	
}

Component *CollisionShape::Clone() const
{
	CollisionShape *clone = new CollisionShape(*this);
	return clone;
}

bool CollisionShape::IsTrigger() const 
{ 
	return isTrigger; 
}

void CollisionShape::SetIsTrigger(bool isTrigger)
{ 
	this->isTrigger = isTrigger;
}

BoundingBox CollisionShape::GetBox() const
{
	return box;
}

float CollisionShape::GetRadius() const
{
	return radius;
}

void CollisionShape::SetOwnedByRigidBody(bool ownedByRigidBody)
{
	this->ownedByRigidBody = ownedByRigidBody;
}

bool CollisionShape::IsOwnedByRigidBody() const
{
	return ownedByRigidBody;
}
