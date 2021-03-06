#include <sstream>

#include "Scene/StaticModel.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Geometry/Mesh.h"
#include "Core/Events.h"
using namespace FireCube;

StaticModel::StaticModel(Engine *engine) : Renderable(engine), boundingBox(vec3(0), vec3(0))
{
	
}

StaticModel::StaticModel(const StaticModel &other) : Renderable(other), boundingBox(other.boundingBox), geometries(other.geometries), materials(other.materials)
{
	renderableParts = other.renderableParts;
	renderablePartsTransformations = other.renderablePartsTransformations;
	
	std::map<Material *, Material *> clonedMaterialsMap;
	for (auto &mat : materials)
	{
		auto clonedMat = mat->Clone();
		clonedMaterialsMap[mat] = clonedMat;
		mat = clonedMat;
	}

	for (auto &renderablePart : renderableParts)
	{
		auto clonedMat = clonedMaterialsMap.find(renderablePart.material);
		if (clonedMat != clonedMaterialsMap.end())
		{
			renderablePart.material = clonedMat->second;
		}
		else
		{
			renderablePart.material = renderablePart.material->Clone();
		}
	}
}

StaticModel::StaticModel(Engine *engine, Mesh *mesh) : StaticModel(engine)
{
	CreateFromMesh(mesh);
}

void StaticModel::CreateFromMesh(Mesh *mesh)
{
	UnSubscribeFromEvent(Events::ResourceReloaded);

	renderableParts.clear();
	renderablePartsTransformations.clear();
	geometries.clear();
	materials.clear();
	boundingBox = BoundingBox(vec3(0), vec3(0));

	if (mesh)
	{
		if (mesh->GetFileName().empty() == false)
		{
			SubscribeToEvent(mesh, Events::ResourceReloaded, &StaticModel::MeshReloaded);
		}

		for (unsigned int i = 0; i < mesh->GetGeometries().size(); ++i)
		{
			Geometry *geometry = mesh->GetGeometries()[i];
			Material *material = mesh->GetMaterials()[i];

			geometries.push_back(geometry);
			materials.push_back(material);
		}

		MarkedDirty();

		CreateRenderableParts(mesh, mesh->GetSkeletonRoot(), renderableParts, mat4::IDENTITY);
	}
}

void StaticModel::CreateRenderableParts(Mesh *mesh, SkeletonNode &skeletonNode, std::vector<RenderablePart> &renderableParts, mat4 transformation)
{
	mat4 modelTransformation = transformation * skeletonNode.transformation;
	for (auto meshIndex : skeletonNode.meshes)
	{
		RenderablePart part;
		part.geometry = geometries[meshIndex];
		part.material = materials[meshIndex];		
		
		renderableParts.push_back(part);
		renderablePartsTransformations.push_back(modelTransformation);

		auto boundingBoxes = mesh->GetBoundingBoxes();
		BoundingBox box = boundingBoxes[meshIndex];
		box.Transform(modelTransformation);
		boundingBox.Expand(box);
	}

	for (auto &c : skeletonNode.children)
	{
		CreateRenderableParts(mesh, c, renderableParts, modelTransformation);
	}
}

void StaticModel::UpdateRenderableParts()
{
	for (unsigned int i = 0; i < renderableParts.size(); ++i)
	{
		renderableParts[i].transformation = node->GetWorldTransformation() * renderablePartsTransformations[i];
	}
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

void StaticModel::UpdateWorldBoundingBox()
{
	worldBoundingBox = boundingBox;
	if (worldBoundingBox.Valid())
	{
		worldBoundingBox.Transform(node->GetWorldTransformation());
	}
}

void StaticModel::IntersectRay(RayQuery &rayQuery)
{
	float distance;
	if (rayQuery.ray.IntersectBoundingBox(GetWorldBoundingBox(), distance) && distance <= rayQuery.maxDistance)
	{
		vec3 normal, minNormal;
		float minDistance;
		bool found = false;
		RenderablePart *minDistRenderablePart = nullptr;

		for (unsigned int i = 0; i < renderableParts.size(); ++i)
		{
			mat4 worldTransform = node->GetWorldTransformation() * renderablePartsTransformations[i];
			mat4 modelTransform = worldTransform;
			modelTransform.Inverse();
			Ray localRay = rayQuery.ray.Transformed(modelTransform);
			
			Geometry *geometry = renderableParts[i].geometry;
			
			if (geometry->IntersectRay(localRay, distance, normal) && distance <= rayQuery.maxDistance)
			{
				vec3 intersectionPoint = worldTransform * (localRay.origin + localRay.direction * distance);
				distance = (intersectionPoint - rayQuery.ray.origin).Length();
				if (!found || distance < minDistance)
				{
					found = true;
					minDistance = distance;
					minNormal = normal.TransformNormal(worldTransform);
					minDistRenderablePart = &renderableParts[i];
				}
			}			
		}

		if (found)
		{
			RayQueryResult result;
			result.distance = minDistance;
			result.renderable = this;
			result.normal = minNormal;
			result.renderablePart = minDistRenderablePart;
			rayQuery.results.push_back(result);
		}
	}	
}

void StaticModel::SetMaterial(Material *material)
{
	for (auto &renderablePart : renderableParts)
	{
		renderablePart.material = material;
	}

	materials = { material };
}

Component *StaticModel::Clone() const
{
	StaticModel *clone = new StaticModel(*this);	
	return clone;
}

void StaticModel::MeshReloaded(Resource *mesh)
{
	CreateFromMesh((Mesh *)mesh);
}

void StaticModel::SetRenderablePartMaterial(unsigned int index, Material *material)
{
	renderableParts[index].material = material;
}
