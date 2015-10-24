#include "TranslateGizmo.h"
#include "../Types.h"
#include "../MathUtils.h"
#include "../Commands/TransformCommands.h"
#include "../Descriptors/NodeDescriptor.h"

using namespace FireCube;

TranslateGizmo::TranslateGizmo(FireCube::Engine *engine, FireCube::Node *parent) : Object(engine), snapToGrid(false)
{
	SharedPtr<Material> material = engine->GetResourceCache()->GetResource<Material>("Materials/Unlit.xml")->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 0.0f, 0.0f));

	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), material);	
	StaticModel *staticModel;
	Node *child;
	node = parent->CreateChild("Editor_TranslateGizmo");
	child = node->CreateChild("XAxis");
	child->Move(vec3(0.5f, 0.0f, 0.0f));
	child->Rotate(vec3(0.0f, 0.0f, PI * 0.5f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 1.0f, 0.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), material);	
	child = node->CreateChild("YAxis");
	child->Move(vec3(0.0f, 0.5f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 0.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.1f, 1.0f, 0.1f)), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), material);	
	child = node->CreateChild("ZAxis");
	child->Move(vec3(0.0f, 0.0f, -0.5f));
	child->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);
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

bool TranslateGizmo::CheckOperationStart(FireCube::Scene *scene, NodeDescriptor *currentNode, FireCube::Ray ray, FireCube::vec2 mousePos)
{
	RayQuery query(ray, 10e4);

	scene->IntersectRay(query, GIZMO_GEOMETRY);
	if (query.results.empty() == false)
	{
		auto &result = query.results.front();
		Node *node = result.renderable->GetNode();		
		if (currentNode->GetNode()->GetParent())
		{
			parentRotationMatrix = currentNode->GetNode()->GetParent()->GetWorldRotation();
		}
		else
		{
			parentRotationMatrix.Identity();
		}
		currentAxis = node->GetName();
		dragStart = query.ray.origin + query.ray.direction * result.distance;
		startPosition = currentNode->GetTranslation();
		return true;
	}

	return false;
}

void TranslateGizmo::PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, NodeDescriptor *currentNode)
{
	vec3 intersectionPoint;
	vec3 translation;
	bool moved = false;
	if (currentAxis == "XAxis")
	{		
		if (::MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(1, 0, 0), ray.origin, ray.direction))
		{
			translation = vec3((intersectionPoint.x - dragStart.x), 0.0f, 0.0f);
			moved = true;
		}
	}
	else if (currentAxis == "YAxis")
	{
		vec3 intersectionPoint;
		if (::MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(0, 1, 0), ray.origin, ray.direction))
		{
			translation = vec3(0.0f, (intersectionPoint.y - dragStart.y), 0.0f);
			moved = true;
		}
	}
	else if (currentAxis == "ZAxis")
	{
		vec3 intersectionPoint;
		if (::MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, vec3(0, 0, 1), ray.origin, ray.direction))
		{
			translation = vec3(0.0f, 0.0f, (intersectionPoint.z - dragStart.z));
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

		mat4 temp = parentRotationMatrix;
		temp.Inverse();
		translation = temp * translation;
		endPosition = startPosition + translation;
		currentNode->SetTranslation(endPosition);		
		node->SetTranslation(currentNode->GetNode()->GetWorldPosition());
	}
}

void TranslateGizmo::SetSnapToGrid(bool snap)
{
	snapToGrid = snap;
}

Command *TranslateGizmo::GetCommand(EditorState *editorState, NodeDescriptor *nodeDesc)
{
	return new SetTranslationCommand(editorState, "Translate", nodeDesc, startPosition, endPosition);
}

void TranslateGizmo::UpdateTransformation(FireCube::Camera *camera, NodeDescriptor *currentNode)
{
	node->SetTranslation(currentNode->GetNode()->GetWorldPosition());
	node->SetScale((camera->GetNode()->GetWorldPosition() - currentNode->GetNode()->GetWorldPosition()).Length() * 0.1f);
}