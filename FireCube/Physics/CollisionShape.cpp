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

CollisionShape::CollisionShape(Engine *engine) : Component(engine), physicsWorld(nullptr), worldBoundingBoxChanged(false), isTrigger(false), collisionMesh(nullptr), mesh(nullptr),
	octreeNode(nullptr), octreeNodeNeedsUpdate(false)
{

}

CollisionShape::CollisionShape(const CollisionShape &other) : Component(other), worldBoundingBoxChanged(true), type(other.type), physicsWorld(other.physicsWorld), collisionMesh(other.collisionMesh), mesh(other.mesh), plane(other.plane),
															  shapeBoundingBox(other.shapeBoundingBox), isTrigger(other.isTrigger), octreeNode(nullptr), octreeNodeNeedsUpdate(false)
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
	// Update shape (if scale changed)
	if (!octreeNodeNeedsUpdate && octreeNode)
	{
		octreeNode->GetOctree()->QueueUpdate(this);
	}
	worldBoundingBoxChanged = true;
}

void CollisionShape::NodeChanged()
{
	
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

Mesh *CollisionShape::GetMesh()
{
	return mesh;
}

void CollisionShape::SetMesh(Mesh *mesh)
{
	type = CollisionShapeType::TRIANGLE_MESH;
	collisionMesh = new CollisionMesh;
	this->mesh = mesh;
	if (!mesh)
		return;

	SetMesh(mesh->GetSkeletonRoot(), mat4::IDENTITY);
	
	MarkedDirty();
}

void CollisionShape::SetMesh(SkeletonNode &skeletonNode, mat4 transformation)
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
		SetMesh(c, modelTransformation);
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
	shapeBoundingBox = bbox;
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

void CollisionShape::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	if (type == CollisionShapeType::BOX)
	{
		vec3 p[8];
		p[0] = node->GetWorldTransformation() * collisionMesh->triangles[0].p1;
		p[1] = node->GetWorldTransformation() * collisionMesh->triangles[0].p2;
		p[2] = node->GetWorldTransformation() * collisionMesh->triangles[0].p3;
		p[3] = node->GetWorldTransformation() * collisionMesh->triangles[1].p3;
		p[4] = node->GetWorldTransformation() * collisionMesh->triangles[2].p1;
		p[5] = node->GetWorldTransformation() * collisionMesh->triangles[2].p2;
		p[6] = node->GetWorldTransformation() * collisionMesh->triangles[2].p3;
		p[7] = node->GetWorldTransformation() * collisionMesh->triangles[3].p3;
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
		boundingBox = collisionMesh->boundingBox;
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

BoundingBox CollisionShape::GetBox() const
{
	return shapeBoundingBox;
}

OctreeNode<CollisionShape> *CollisionShape::GetOctreeNode()
{
	return octreeNode;
}

void CollisionShape::SetOctreeNode(OctreeNode<CollisionShape> *octreeNode)
{
	this->octreeNode = octreeNode;
}

bool CollisionShape::GetOctreeNodeNeedsUpdate() const
{
	return octreeNodeNeedsUpdate;
}

void CollisionShape::SetOctreeNodeNeedsUpdate(bool octreeNodeNeedsUpdate)
{
	this->octreeNodeNeedsUpdate = octreeNodeNeedsUpdate;
}