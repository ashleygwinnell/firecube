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

using namespace FireCube;

CollisionTriangle::CollisionTriangle(vec3 p1, vec3 p2, vec3 p3) : p1(p1), p2(p2), p3(p3)
{

}

CollisionShape::CollisionShape(Engine *engine) : Component(engine), physicsWorld(nullptr), worldBoundingBoxChanged(false), isTrigger(false)
{

}

CollisionShape::CollisionShape(const CollisionShape &other) : Component(other), worldBoundingBoxChanged(true), type(other.type), physicsWorld(other.physicsWorld), mesh(other.mesh), plane(other.plane),
															  shapeBoundingBox(other.shapeBoundingBox), isTrigger(other.isTrigger)
{
	
}

CollisionShape::~CollisionShape()
{
	if (physicsWorld)
	{
		physicsWorld->RemoveCollisionShape(this);
	}
}

CollisionShapeType CollisionShape::GetShapeType() const
{
	return type;
}

void CollisionShape::MarkedDirty()
{
	// Update shape (if scale changed)
	worldBoundingBoxChanged = true;
}

void CollisionShape::NodeChanged()
{
	
}

void CollisionShape::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		physicsWorld = oldScene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->RemoveCollisionShape(this);
	}

	if (scene)
	{
		physicsWorld = scene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddCollisionShape(this);
	}
}

CollisionMesh *CollisionShape::GetMesh()
{
	return mesh;
}

void CollisionShape::SetMesh(Mesh *mesh)
{
	type = CollisionShapeType::TRIANGLE_MESH;
	this->mesh = new CollisionMesh;

	auto geometries = mesh->GetGeometries();
	for (auto geometry : geometries)
	{
		VertexBuffer *vertexBuffer = geometry->GetVertexBuffer();
		IndexBuffer *indexBuffer = geometry->GetIndexBuffer();
		auto &vertexData = vertexBuffer->GetShadowData();
		auto &indexData = indexBuffer->GetShadowData();
		if (indexData.empty() == false && vertexData.empty() == false)
		{
			unsigned int positionOffset = vertexBuffer->GetVertexAttributeOffset(VertexAttributeType::POSITION);
			for (unsigned int i = 0; i < indexData.size() / sizeof(unsigned int); i += 3)
			{
				unsigned int i0 = *((unsigned int *) &indexData[(i + 0) * sizeof(unsigned int)]);
				unsigned int i1 = *((unsigned int *) &indexData[(i + 1) * sizeof(unsigned int)]);
				unsigned int i2 = *((unsigned int *) &indexData[(i + 2) * sizeof(unsigned int)]);

				vec3 pos0 = *((vec3 *)&vertexData[i0 * vertexBuffer->GetVertexSize() + positionOffset]);
				vec3 pos1 = *((vec3 *)&vertexData[i1 * vertexBuffer->GetVertexSize() + positionOffset]);
				vec3 pos2 = *((vec3 *)&vertexData[i2 * vertexBuffer->GetVertexSize() + positionOffset]);

				this->mesh->triangles.push_back(CollisionTriangle(pos0, pos1, pos2));
				this->mesh->boundingBox.Expand(pos0);
				this->mesh->boundingBox.Expand(pos1);
				this->mesh->boundingBox.Expand(pos2);
			}
		}
	}	
	
	worldBoundingBoxChanged = true;
}

void CollisionShape::SetPlane(const Plane &plane)
{
	type = CollisionShapeType::PLANE;
	this->plane = plane;
	worldBoundingBoxChanged = true;
}

Plane CollisionShape::GetPlane() const
{
	return plane;
}

void CollisionShape::SetBox(BoundingBox bbox)
{
	type = CollisionShapeType::BOX;
	shapeBoundingBox = bbox;
	vec3 bmin = bbox.GetMin();
	vec3 bmax = bbox.GetMax();	
	mesh = new CollisionMesh;
	// Front
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmax.x, bmin.y, bmax.z), vec3(bmax.x, bmax.y, bmax.z)));
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmax.x, bmax.y, bmax.z), vec3(bmin.x, bmax.y, bmax.z)));
	// Back
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmin.z), vec3(bmin.x, bmax.y, bmin.z), vec3(bmax.x, bmax.y, bmin.z)));
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmin.z), vec3(bmax.x, bmax.y, bmin.z), vec3(bmax.x, bmin.y, bmin.z)));
	// Right
	mesh->triangles.push_back(CollisionTriangle(vec3(bmax.x, bmin.y, bmax.z), vec3(bmax.x, bmin.y, bmin.z), vec3(bmax.x, bmax.y, bmin.z)));
	mesh->triangles.push_back(CollisionTriangle(vec3(bmax.x, bmin.y, bmax.z), vec3(bmax.x, bmax.y, bmin.z), vec3(bmax.x, bmax.y, bmax.z)));
	// Left
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmin.x, bmax.y, bmax.z), vec3(bmin.x, bmax.y, bmin.z)));
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmin.x, bmax.y, bmin.z), vec3(bmin.x, bmin.y, bmin.z)));
	// Top
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmax.y, bmax.z), vec3(bmax.x, bmax.y, bmax.z), vec3(bmax.x, bmax.y, bmin.z)));
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmax.y, bmax.z), vec3(bmax.x, bmax.y, bmin.z), vec3(bmin.x, bmax.y, bmin.z)));
	// Bottom
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmin.x, bmin.y, bmin.z), vec3(bmax.x, bmin.y, bmin.z)));
	mesh->triangles.push_back(CollisionTriangle(vec3(bmin.x, bmin.y, bmax.z), vec3(bmax.x, bmin.y, bmin.z), vec3(bmax.x, bmin.y, bmax.z)));

	worldBoundingBoxChanged = true;
}

void CollisionShape::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	if (type == CollisionShapeType::BOX)
	{
		vec3 p[8];
		p[0] = node->GetWorldTransformation() * mesh->triangles[0].p1;
		p[1] = node->GetWorldTransformation() * mesh->triangles[0].p2;
		p[2] = node->GetWorldTransformation() * mesh->triangles[0].p3;
		p[3] = node->GetWorldTransformation() * mesh->triangles[1].p3;
		p[4] = node->GetWorldTransformation() * mesh->triangles[2].p1;
		p[5] = node->GetWorldTransformation() * mesh->triangles[2].p2;
		p[6] = node->GetWorldTransformation() * mesh->triangles[2].p3;
		p[7] = node->GetWorldTransformation() * mesh->triangles[3].p3;
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
		for (auto &t : mesh->triangles)
		{
			vec3 p1 = node->GetWorldTransformation() * t.p1;
			vec3 p2 = node->GetWorldTransformation() * t.p2;
			vec3 p3 = node->GetWorldTransformation() * t.p3;
			debugRenderer->AddLine(p1, p2, vec3(0, 1, 0));
			debugRenderer->AddLine(p2, p3, vec3(0, 1, 0));
			debugRenderer->AddLine(p3, p1, vec3(0, 1, 0));
		}		
	}
	else if (type == CollisionShapeType::PLANE)
	{
		const float cellSize = 1.0f;
		const int numCells = 20;
		const float halfSize = numCells * cellSize * 0.5f;
		vec3 up(0, 1, 0);
		if (Dot(up, plane.GetNormal()) >= 1.0f - 0.0001f)
			up = vec3(0, 1, 0);
		up = (up - plane.GetNormal() * Dot(up, plane.GetNormal())).Normalized();
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
		boundingBox = mesh->boundingBox;
		break;
	case CollisionShapeType::PLANE:
		boundingBox = BoundingBox(vec3(-10e6), vec3(10e6)); // In case of a plane, return a large bounding box
		break;
	case CollisionShapeType::BOX:
		boundingBox = shapeBoundingBox;
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