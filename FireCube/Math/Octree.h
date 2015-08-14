#pragma once

#include <array>

#include "Core/Object.h"
#include "Utils/utils.h"
#include "Scene/Renderable.h"
#include "Math/Math.h"
#include "Math/Frustum.h"
#include "Math/Ray.h"

namespace FireCube
{

template <class T>
class FIRECUBE_API Octree;

template <class T>
class FIRECUBE_API OctreeNode
{
	friend class Octree<T>;
public:
	OctreeNode(Octree<T> *octree, unsigned int level, OctreeNode<T> *root, const BoundingBox &boundingBox) : worldBoundingBox(boundingBox), 
		children({ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }), root(root), octree(octree), level(level)
	{
		halfSize = boundingBox.GetSize() * 0.5f;
		center = boundingBox.GetCenter();
		cullingBox.SetMin(boundingBox.GetMin() - halfSize);
		cullingBox.SetMax(boundingBox.GetMax() + halfSize);
	}

	OctreeNode(Octree<T> *octree, unsigned int level, const BoundingBox &boundingBox) : OctreeNode(octree, level, this, boundingBox)
	{
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


	bool Insert(T *object, const BoundingBox &boundingBox)
	{
		bool addedToChild = false;
		auto size = boundingBox.GetSize();
		if (level < octree->GetMaxLevel() && size.x < halfSize.x * 0.5f && size.y < halfSize.y * 0.5f && size.z < halfSize.z * 0.5f)
		{
			auto boxCenter = boundingBox.GetCenter();
			unsigned int x = boxCenter.x < center.x ? 0 : 1;
			unsigned int y = boxCenter.y < center.y ? 0 : 2;
			unsigned int z = boxCenter.z < center.z ? 0 : 4;

			auto child = children[x + y + z];
			if (!child)
			{
				vec3 childMin(x == 0 ? worldBoundingBox.GetMin().x : center.x, y == 0 ? worldBoundingBox.GetMin().y : center.y, z == 0 ? worldBoundingBox.GetMin().z : center.z);
				vec3 childMax(x == 0 ? center.x : worldBoundingBox.GetMax().x, y == 0 ? center.y : worldBoundingBox.GetMax().y, z == 0 ? center.z : worldBoundingBox.GetMax().z);
				child = children[x + y + z] = new OctreeNode(octree, level + 1, root, BoundingBox(childMin, childMax));
			}

			addedToChild = child->Insert(object, boundingBox);
		}

		if (!addedToChild)
		{
			if (this != root && (boundingBox.GetMin().x < cullingBox.GetMin().x || boundingBox.GetMin().y < cullingBox.GetMin().y || boundingBox.GetMin().z < cullingBox.GetMin().z ||
				boundingBox.GetMax().x > cullingBox.GetMax().x || boundingBox.GetMax().y > cullingBox.GetMax().y || boundingBox.GetMax().z > cullingBox.GetMax().z))
			{
				return false;
			}
			if (object->GetOctreeNode())
			{
				object->GetOctreeNode()->objects.erase(std::remove(object->GetOctreeNode()->objects.begin(), object->GetOctreeNode()->objects.end(), object), object->GetOctreeNode()->objects.end());
			}

			objects.push_back(object);
			object->SetOctreeNode(this);
		}

		return true;
	}

	void GetObjects(const Frustum &frustum, std::vector<T *> &objects)
	{
		if (this == root || frustum.Contains(cullingBox))
		{
			for (auto object : this->objects)
			{
				objects.push_back(object);
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

	void GetObjects(const Ray &ray, std::vector<T *> &objects)
	{
		if (ray.IntersectBoundingBox(cullingBox))
		{
			for (auto object : this->objects)
			{
				objects.push_back(object);
			}

			for (auto child : children)
			{
				if (child)
				{
					child->GetObjects(ray, objects);
				}
			}
		}
	}

	void GetObjects(const BoundingBox &boundingBox, std::vector<T *> &objects)
	{
		if (this == root || boundingBox.Intersects(cullingBox))
		{
			for (auto object : this->objects)
			{
				objects.push_back(object);
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
private:
	BoundingBox worldBoundingBox;
	BoundingBox cullingBox;
	vec3 halfSize;
	vec3 center;
	std::vector<T *> objects;
	std::array<OctreeNode<T> *, 8> children;
	OctreeNode *root;
	Octree<T> *octree;
	unsigned int level;
};

template <class T>
class FIRECUBE_API Octree : public Object
{
	OBJECT(Octree)
public:
	Octree(Engine *engine, vec3 initialSize, unsigned int maxLevel) : Object(engine), maxLevel(maxLevel)
	{
		root = new OctreeNode<T>(this, 0, BoundingBox(-initialSize * 0.5f, initialSize * 0.5f));
	}

	~Octree()
	{
		delete root;
	}

	void Insert(T *object)
	{
		auto boundingBox = object->GetWorldBoundingBox();
		auto size = boundingBox.GetSize();
		root->Insert(object, object->GetWorldBoundingBox());
	}

	void Remove(T *object)
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

	void GetObjects(const Ray &ray, std::vector<T *> &objects)
	{
		root->GetObjects(ray, objects);
	}

	void GetObjects(const BoundingBox &boundingBox, std::vector<T *> &objects)
	{
		root->GetObjects(boundingBox, objects);
	}

	void QueueUpdate(T *object)
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

	unsigned int GetMaxLevel() const
	{
		return maxLevel;
	}

private:
	OctreeNode<T> *root;
	std::vector<T *> pendingUpdate;
	unsigned int maxLevel;
};

}