#include <sstream>

#include "Scene/StaticModel.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Geometry/Mesh.h"
using namespace FireCube;

StaticModel::StaticModel(Engine *engine) : Renderable(engine)
{
	
}

StaticModel::StaticModel(const StaticModel &other) : Renderable(other), boundingBox(other.boundingBox), geometries(other.geometries), materials(other.materials), mesh(other.mesh)
{
	renderableParts = other.renderableParts;
}

StaticModel::StaticModel(Engine *engine, Mesh *mesh) : StaticModel(engine)
{
	CreateFromMesh(mesh);
}

void StaticModel::CreateFromMesh(Mesh *mesh)
{
	this->mesh = mesh;
	renderableParts.clear();
	geometries.clear();
	materials.clear();

	for (unsigned int i = 0; i < mesh->GetGeometries().size(); ++i)
	{					
		RenderablePart part;
		Geometry *geometry = mesh->GetGeometries()[i];
		Material *material = mesh->GetMaterials()[i];
		part.geometry = geometry;
		part.material = material;

		renderableParts.push_back(part);
		geometries.push_back(geometry);
		materials.push_back(material);
	}
	SetBoundingBox(mesh->GetBoundingBox());
}

void StaticModel::SetBoundingBox(BoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
	MarkedDirty();
}

std::vector<SharedPtr<Geometry>> &StaticModel::GetGeometries()
{
	return geometries;
}

std::vector<SharedPtr<Material>> &StaticModel::GetMaterials()
{
	return materials;
}

Mesh *StaticModel::GetMesh()
{
	return mesh;
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
			result.normal = minNormal.TransformNormal(node->GetWorldTransformation());
			rayQuery.results.push_back(result);
		}
	}	
}

Component *StaticModel::Clone() const
{
	StaticModel *clone = new StaticModel(*this);	
	return clone;
}