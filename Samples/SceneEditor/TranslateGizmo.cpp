#include "TranslateGizmo.h"
#include "Types.h"
#include "MathUtils.h"
using namespace FireCube;

TranslateGizmo::TranslateGizmo(FireCube::Engine *engine, FireCube::Node *parent) : Object(engine), snapToGrid(false)
{
	SharedPtr<Material> material = engine->GetResourceCache()->GetResource<Material>("Materials/TerrainNoTexture.xml")->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 0.0f, 0.0f, 1.0f));

	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), material);
	mesh->SetBoundingBox(BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)));
	StaticModel *staticModel;
	Node *child;
	node = parent->CreateChild("TranslateGizmo");
	child = node->CreateChild("XAxis");
	child->Move(vec3(0.5f, 0.0f, 0.0f));
	child->Rotate(vec3(0.0f, 0.0f, PI * 0.5f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), material);
	mesh->SetBoundingBox(BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)));
	child = node->CreateChild("YAxis");
	child->Move(vec3(0.0f, 0.5f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(0.0f, 0.0f, 1.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), material);
	mesh->SetBoundingBox(BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)));
	child = node->CreateChild("ZAxis");
	child->Move(vec3(0.0f, 0.0f, -0.5f));
	child->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);
}

void TranslateGizmo::SetPosition(FireCube::vec3 position)
{
	node->SetTranslation(position);
}

void TranslateGizmo::Show()
{
	std::vector<StaticModel *> components;
	node->GetComponents<StaticModel>(components, true);
	for (auto c : components)
	{
		c->SetEnabled(true);
	}
}

void TranslateGizmo::Hide()
{
	std::vector<StaticModel *> components;
	node->GetComponents<StaticModel>(components, true);
	for (auto c : components)
	{
		c->SetEnabled(false);
	}
}

bool TranslateGizmo::CheckOperationStart(FireCube::Scene *scene, FireCube::Node *currentNode, FireCube::Ray ray)
{
	RayQuery query(ray, 10e4);

	scene->IntersectRay(query, GIZMO_GEOMETRY);
	if (query.results.empty() == false)
	{
		auto &result = query.results.front();
		Node *node = result.renderable->GetNode();		
		currentAxis = node->GetName();
		dragStart = query.ray.origin + query.ray.direction * result.distance;
		startPosition = currentNode->GetTranslation();
		return true;
	}

	return false;
}

void TranslateGizmo::PerformOperation(FireCube::Ray ray, FireCube::Node *currentNode)
{
	vec3 intersectionPoint;
	vec3 translation;
	bool moved = false;
	if (currentAxis == "XAxis")
	{		
		if (MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(1, 0, 0), ray.origin, ray.direction))
		{
			translation = vec3(startPosition.x + (intersectionPoint.x - dragStart.x), startPosition.y, startPosition.z);
			moved = true;
		}
	}
	else if (currentAxis == "YAxis")
	{
		vec3 intersectionPoint;
		if (MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(0, 1, 0), ray.origin, ray.direction))
		{
			translation = vec3(startPosition.x, startPosition.y + (intersectionPoint.y - dragStart.y), startPosition.z);
			moved = true;
		}
	}
	else if (currentAxis == "ZAxis")
	{
		vec3 intersectionPoint;
		if (MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(0, 0, 1), ray.origin, ray.direction))
		{
			translation = vec3(startPosition.x, startPosition.y, startPosition.z + (intersectionPoint.z - dragStart.z));
			moved = true;
		}
	}

	if (moved)
	{
		if (snapToGrid)
		{
			std::modf(translation.x, &translation.x);
			std::modf(translation.y, &translation.y);
			std::modf(translation.z, &translation.z);
		}
		currentNode->SetTranslation(translation);
		this->SetPosition(currentNode->GetWorldPosition());
	}
}

void TranslateGizmo::SetSnapToGrid(bool snap)
{
	snapToGrid = snap;
}