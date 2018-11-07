#pragma once

#include <array>

#include "Core/Object.h"
#include "Utils/utils.h"
#include "Math/Math.h"
#include "Math/Frustum.h"
#include "Math/Ray.h"
#include "Rendering/DebugRenderer.h"

namespace FireCube
{

template <class T>
class FIRECUBE_API Octree;

template <class T>
class FIRECUBE_API OctreeItem;

template <class T>
class FIRECUBE_API OctreeNode
{
	friend class Octree<T>;
public:
	OctreeNode(Octree<T> *octree, const BoundingBox &boundingBox) : worldBoundingBox(boundingBox), 
		children({ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }), octree(octree)
	{
		halfSize = boundingBox.GetSize() * 0.5f;
		center = boundingBox.GetCenter();
		cullingBox.SetMin(boundingBox.GetMin() - halfSize);
		cullingBox.SetMax(boundingBox.GetMax() + halfSize);
	}

	~OctreeNode()
	{
		for (auto object : objects)
		{
			object->SetOctreeNode(nullptr);
		}

		for (auto child : children)
		{
			delete child;
		}
	}


	void Insert(OctreeItem<T> *object, const BoundingBox &boundingBox)
	{
		unsigned int childIndex;

		if (!HasChildren())
		{
			if (objects.size() < octree->GetMaxObjects() || halfSize.x < octree->GetMinSize())
			{
				AddObject(object);
				return;
			}

			for (int i = objects.size() - 1; i >= 0; --i)
			{
				auto existingObject = objects[i];
				auto objectBoundingBox = static_cast<T *>(existingObject)->GetWorldBoundingBox();
				auto boxCenter = objectBoundingBox.GetCenter();

				auto child = GetClosestChild(boxCenter, childIndex);
				if (!child)
				{
					child = CreateChild(childIndex);
				}

				if (child->cullingBox.Contains(boundingBox))
				{
					objects.erase(objects.begin() + i);
					child->Insert(existingObject, boundingBox);
				}
			}
		}

		auto boxCenter = boundingBox.GetCenter();
		auto child = GetClosestChild(boxCenter, childIndex);
		if (!child)
		{
			child = CreateChild(childIndex);
		}

		if (child->cullingBox.Contains(boundingBox))
		{
			child->Insert(object, boundingBox);
		}
		else
		{
			AddObject(object);
		}
	}

	OctreeNode<T> *GetClosestChild(vec3 boxCenter, unsigned int &index)
	{
		unsigned int x = boxCenter.x < center.x ? 0 : 1;
		unsigned int y = boxCenter.y < center.y ? 0 : 2;
		unsigned int z = boxCenter.z < center.z ? 0 : 4;
		index = x + y + z;
		return children[index];
	}

	OctreeNode<T> *CreateChild(unsigned int index)
	{
		unsigned int x = index & 1;
		unsigned int y = index & 2;
		unsigned int z = index & 4;

		vec3 childMin(x == 0 ? worldBoundingBox.GetMin().x : center.x, y == 0 ? worldBoundingBox.GetMin().y : center.y, z == 0 ? worldBoundingBox.GetMin().z : center.z);
		vec3 childMax(x == 0 ? center.x : worldBoundingBox.GetMax().x, y == 0 ? center.y : worldBoundingBox.GetMax().y, z == 0 ? center.z : worldBoundingBox.GetMax().z);
		children[index] = new OctreeNode(octree, BoundingBox(childMin, childMax));
		return children[index];
	}

	void AddObject(OctreeItem<T> *object)
	{
		if (object->GetOctreeNode())
		{
			object->GetOctreeNode()->objects.erase(std::remove(object->GetOctreeNode()->objects.begin(), object->GetOctreeNode()->objects.end(), object), object->GetOctreeNode()->objects.end());
		}

		objects.push_back(object);
		object->SetOctreeNode(this);
	}

	void GetObjects(const Frustum &frustum, std::vector<T *> &objects)
	{
		if (frustum.Contains(cullingBox))
		{
			for (auto object : this->objects)
			{
				objects.push_back(static_cast<T *>(object));
			}

			for (auto child : children)
			{
				if (child)
				{
					child->GetObjects(frustum, objects);
				}
			}
		}
	}

	void GetObjects(const Ray &ray, float maxDistance, std::vector<T *> &objects)
	{
		float distance;
		if (ray.IntersectBoundingBox(cullingBox, distance) && distance < maxDistance)
		{
			for (auto object : this->objects)
			{
				objects.push_back(static_cast<T *>(object));
			}

			for (auto child : children)
			{
				if (child)
				{
					child->GetObjects(ray, maxDistance, objects);
				}
			}
		}
	}

	void GetObjects(const BoundingBox &boundingBox, std::vector<T *> &objects)
	{
		if (boundingBox.Intersects(cullingBox))
		{
			for (auto object : this->objects)
			{
				objects.push_back(static_cast<T *>(object));
			}

			for (auto child : children)
			{
				if (child)
				{
					child->GetObjects(boundingBox, objects);
				}
			}
		}
	}

	Octree<T> *GetOctree()
	{
		return octree;
	}

	void RenderDebugGeometry(DebugRenderer *debugRenderer)
	{
		bool hasChildren = false;

		for (auto child : children)
		{
			if (child)
			{
				hasChildren = true;
			}
		}

		if (hasChildren || objects.empty() == false)
		{
			debugRenderer->AddBoundingBox(worldBoundingBox, vec3(0, 1, 0));
		}

		for (auto child : children)
		{
			if (child)
			{
				child->RenderDebugGeometry(debugRenderer);
			}
		}
	}

	bool HasChildren() const
	{
		return children[0] != nullptr || children[1] != nullptr || children[2] != nullptr || children[3] != nullptr ||
			children[4] != nullptr || children[5] != nullptr || children[6] != nullptr || children[7] != nullptr;
	}
private:
	BoundingBox worldBoundingBox;
	BoundingBox cullingBox;
	vec3 halfSize;
	vec3 center;
	std::vector<OctreeItem<T> *> objects;
	std::array<OctreeNode<T> *, 8> children;
	Octree<T> *octree;
};

template <class T>
class FIRECUBE_API OctreeItem
{
public:

	OctreeItem() : octree(nullptr), octreeNode(nullptr), octreeNodeNeedsUpdate(false)
	{

	}

	Octree<T> *GetOctree()
	{
		return octree;
	}

	void SetOctree(Octree<T> *octree)
	{
		this->octree = octree;
	}
	
	OctreeNode<T> *GetOctreeNode()
	{
		return octreeNode;
	}

	void SetOctreeNode(OctreeNode<T> *octreeNode)
	{
		this->octreeNode = octreeNode;
	}

	bool GetOctreeNodeNeedsUpdate() const
	{
		return octreeNodeNeedsUpdate;
	}

	void SetOctreeNodeNeedsUpdate(bool octreeNodeNeedsUpdate)
	{
		this->octreeNodeNeedsUpdate = octreeNodeNeedsUpdate;
	}

	void MarkForOctreeReinsertion()
	{
		if (!octreeNodeNeedsUpdate && octree && static_cast<T *>(this)->GetWorldBoundingBox().Valid())
		{
			octree->QueueUpdate(this);
		}
	}

	Octree<T> *octree;
	OctreeNode<T> *octreeNode;
	bool octreeNodeNeedsUpdate;
};


template <class T>
class FIRECUBE_API Octree : public Object
{
	FIRECUBE_OBJECT(Octree)
public:
	Octree(Engine *engine, float initialSize, unsigned int maxObjects, float minSize) : Object(engine), maxObjects(maxObjects), minSize(minSize)
	{
		root = new OctreeNode<T>(this, BoundingBox(vec3(-initialSize * 0.5f), vec3(initialSize * 0.5f)));
	}

	~Octree()
	{
		delete root;
	}

	void Insert(OctreeItem<T> *object)
	{
		const unsigned int maxAttempts = 10;
		auto boundingBox = static_cast<T *>(object)->GetWorldBoundingBox();
		unsigned int attempts = 0;
		while (root->cullingBox.Contains(boundingBox) == false)
		{
			Grow(boundingBox.GetCenter() - root->center);
			if (++attempts > maxAttempts)
			{
				return;
			}
		}
		root->Insert(object, boundingBox);
	}

	void Remove(OctreeItem<T> *object)
	{
		if (object->GetOctreeNode()->GetOctree() == this)
		{
			object->GetOctreeNode()->objects.erase(std::remove(object->GetOctreeNode()->objects.begin(), object->GetOctreeNode()->objects.end(), object), object->GetOctreeNode()->objects.end());
			object->SetOctreeNode(nullptr);			
			if (object->GetOctreeNodeNeedsUpdate())
			{
				object->SetOctreeNodeNeedsUpdate(false);
				pendingUpdate.erase(std::remove(pendingUpdate.begin(), pendingUpdate.end(), object), pendingUpdate.end());
			}

		}
	}

	void GetObjects(const Frustum &frustum, std::vector<T *> &objects)
	{
		root->GetObjects(frustum, objects);
	}

	void GetObjects(const Ray &ray, float maxDistance, std::vector<T *> &objects)
	{
		root->GetObjects(ray, maxDistance, objects);
	}

	void GetObjects(const BoundingBox &boundingBox, std::vector<T *> &objects)
	{
		root->GetObjects(boundingBox, objects);
	}

	void QueueUpdate(OctreeItem<T> *object)
	{
		pendingUpdate.push_back(object);
		object->SetOctreeNodeNeedsUpdate(true);
	}

	void Update()
	{
		for (auto object : pendingUpdate)
		{
			Insert(object);
			object->SetOctreeNodeNeedsUpdate(false);
		}

		pendingUpdate.clear();
	}

	unsigned int GetMaxObjects() const
	{
		return maxObjects;
	}

	float GetMinSize() const
	{
		return minSize;
	}

	void RenderDebugGeometry(DebugRenderer *debugRenderer)
	{
		root->RenderDebugGeometry(debugRenderer);
	}

private:

	void Grow(vec3 direction)
	{
		unsigned int x = direction.x > 0 ? 0 : 1;
		unsigned int y = direction.y > 0 ? 0 : 2;
		unsigned int z = direction.z > 0 ? 0 : 4;

		BoundingBox rootBoundingBox = root->worldBoundingBox;
		vec3 size = rootBoundingBox.GetSize();

		vec3 newMin(x != 0 ? rootBoundingBox.GetMin().x - size.x : rootBoundingBox.GetMin().x,
			y != 0 ? rootBoundingBox.GetMin().y - size.y : rootBoundingBox.GetMin().y,
			z != 0 ? rootBoundingBox.GetMin().z - size.z : rootBoundingBox.GetMin().z);
		BoundingBox newBoundingBox(newMin, newMin + size * 2.0);
		OctreeNode<T> *newRoot = new OctreeNode<T>(this, newBoundingBox);

		newRoot->children[x + y + z] = root;
		root = newRoot;
	}

	OctreeNode<T> *root;
	std::vector<OctreeItem<T> *> pendingUpdate;
	unsigned int maxObjects;
	float minSize;
};

}