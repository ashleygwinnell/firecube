#include <sstream>

#include "Rendering/StaticModel.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Geometry/Mesh.h"
using namespace FireCube;

StaticModel::StaticModel(Engine *engine) : Renderable(engine)
{
	
}

void StaticModel::CreateFromMesh(MeshPtr mesh)
{
	for (auto &geometry : mesh->GetGeometries())
	{					
		RenderablePart part;
		part.geometry = geometry.get();
		renderableParts.push_back(part);
		geometries.push_back(geometry);
	}
	SetBoundingBox(mesh->GetBoundingBox());
}

void StaticModel::SetBoundingBox(BoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
	MarkedDirty();
}

void StaticModel::AddRenderablePart(Geometry *geometry)
{
	RenderablePart part;
	part.geometry = geometry;
	renderableParts.push_back(part);
	geometries.push_back(GeometryPtr(geometry));
}

std::vector<GeometryPtr> &StaticModel::GetGeometries()
{
	return geometries;
}

void StaticModel::UpdateWorldBoundingBox()
{
	worldBoundingBox = boundingBox;
	worldBoundingBox.Transform(node->GetWorldTransformation());
}

void StaticModel::IntersectRay(RayQuery &rayQuery)
{
	float distance;
	if (rayQuery.ray.IntersectBoundingBox(GetWorldBoundingBox(), distance) && distance <= rayQuery.maxDistance)
	{
		mat4 worldTransform = node->GetWorldTransformation();
		worldTransform.Inverse();
		Ray localRay = rayQuery.ray.Transformed(worldTransform);
		vec3 normal, minNormal;
		float minDistance;
		bool found = false;
		for (auto geometry : geometries)
		{
			if (geometry->IntersectRay(localRay, distance, normal) && distance <= rayQuery.maxDistance)
			{
				if (!found || distance < minDistance)
				{
					found = true;
					minDistance = distance;
					minNormal = normal;
				}				
			}
		}		
		if (found)
		{
			RayQueryResult result;
			result.distance = minDistance;
			result.renderable = this;
			result.normal = minNormal;
			rayQuery.results.push_back(result);
		}
	}	
}