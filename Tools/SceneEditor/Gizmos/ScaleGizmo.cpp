#include "ScaleGizmo.h"
#include "../Types.h"
#include "../MathUtils.h"
#include "../Commands/TransformCommands.h"
#include "../Descriptors/NodeDescriptor.h"

using namespace FireCube;

ScaleGizmo::ScaleGizmo(FireCube::Engine *engine, FireCube::Node *parent) : Object(engine), snapToGrid(false)
{
	const float lineLength = 1.0f;
	const float boxSize = 0.2f;
	SharedPtr<Mesh> mesh;
	SharedPtr<Mesh> meshLine;
	SharedPtr<Mesh> meshIntersection;

	SharedPtr<Material> material = engine->GetResourceCache()->GetResource<Material>("Materials/Unlit.xml");
	node = parent->CreateChild("Editor_ScaleGizmo");
	StaticModel *staticModel;
	Node *child;

	// X Axis
	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 0.0f, 0.0f));

	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(boxSize)), BoundingBox(vec3(-boxSize), vec3(boxSize)), material);
	meshLine = new Mesh(engine);
	meshLine->AddGeometry(GeometryGenerator::GeneratePolyline(engine, { vec3(0.0f), vec3(lineLength, 0.0f, 0.0f) }), BoundingBox(vec3(-boxSize), vec3(boxSize)), material);
	meshIntersection = new Mesh(engine);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateCylinder(engine, boxSize * 0.5f, 0.0f, lineLength + boxSize, 2, 8), BoundingBox(vec3(-boxSize * 0.5f, 0.0f, -boxSize * 0.5f), vec3(boxSize * 0.5f, lineLength + boxSize, boxSize * 0.5f)), nullptr);
	
	Node *xAxis = node->CreateChild();
	staticModel = xAxis->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshLine);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = xAxis->CreateChild();
	child->Move(vec3(lineLength + boxSize * 0.5f, 0.0f, 0.0f));
	child->Rotate(vec3(0.0f, 0.0f, PI * 0.5f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);
	
	child = xAxis->CreateChild("XAxis");
	child->Rotate(vec3(0.0f, 0.0f, -PI * 0.5f));
	child->Move(vec3(lineLength + boxSize, 0.0f, 0.0f));	
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// Y Axis
	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 1.0f, 0.0f));
		
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(boxSize)), BoundingBox(vec3(-boxSize), vec3(boxSize)), material);
	meshLine = new Mesh(engine);
	meshLine->AddGeometry(GeometryGenerator::GeneratePolyline(engine, { vec3(0.0f), vec3(0.0f, lineLength, 0.0f) }), BoundingBox(vec3(-boxSize), vec3(boxSize)), material);
	meshIntersection = new Mesh(engine);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateCylinder(engine, boxSize * 0.5f, 0.0f, lineLength + boxSize, 2, 8), BoundingBox(vec3(-boxSize * 0.5f, 0.0f, -boxSize * 0.5f), vec3(boxSize * 0.5f, lineLength + boxSize, boxSize * 0.5f)), nullptr);

	Node *yAxis = node->CreateChild();
	staticModel = yAxis->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshLine);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = yAxis->CreateChild();
	child->Move(vec3(0.0f, lineLength + boxSize * 0.5f, 0.0f));	
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = yAxis->CreateChild("YAxis");	
	child->Rotate(vec3(PI, 0.0f, 0.0f));
	child->Move(vec3(0.0f, lineLength + boxSize * 0.5f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// Z Axis
	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 0.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(boxSize)), BoundingBox(vec3(-boxSize), vec3(boxSize)), material);
	meshLine = new Mesh(engine);
	meshLine->AddGeometry(GeometryGenerator::GeneratePolyline(engine, { vec3(0.0f), vec3(0.0f, 0.0f, lineLength) }), BoundingBox(vec3(-boxSize), vec3(boxSize)), material);
	meshIntersection = new Mesh(engine);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateCylinder(engine, boxSize * 0.5f, 0.0f, lineLength + boxSize, 2, 8), BoundingBox(vec3(-boxSize * 0.5f, 0.0f, -boxSize * 0.5f), vec3(boxSize * 0.5f, lineLength + boxSize, boxSize * 0.5f)), nullptr);

	Node *zAxis = node->CreateChild();
	staticModel = zAxis->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshLine);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = zAxis->CreateChild();
	child->Move(vec3(0.0f, 0.0f, lineLength + boxSize * 0.5f));
	child->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = zAxis->CreateChild("ZAxis");
	child->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));
	child->Move(vec3(0.0f, 0.0f, lineLength + boxSize));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// All axes
	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(boxSize)), BoundingBox(vec3(-boxSize * 0.5f), vec3(boxSize * 0.5f)), material);	
	child = node->CreateChild("AllAxes");	
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);
}

void ScaleGizmo::Show()
{
	std::vector<StaticModel *> components;
	node->GetComponents<StaticModel>(components, true);
	for (auto c : components)
	{
		c->SetEnabled(true);
	}
}

void ScaleGizmo::Hide()
{
	std::vector<StaticModel *> components;
	node->GetComponents<StaticModel>(components, true);
	for (auto c : components)
	{
		c->SetEnabled(false);
	}
}

bool ScaleGizmo::CheckOperationStart(FireCube::Scene *scene, NodeDescriptor *currentNode, FireCube::Ray ray, vec2 mousePos)
{
	RayQuery query(ray, 10e4);

	scene->IntersectRay(query, GIZMO_GEOMETRY);
	if (query.results.empty() == false)
	{
		auto &result = query.results.front();
		Node *node = result.renderable->GetNode();
		currentAxis = node->GetName();
		startMousePos = mousePos;
		dragStart = query.ray.origin + query.ray.direction * result.distance;
		startScale = currentNode->GetScale();
		return true;
	}

	return false;
}

void ScaleGizmo::PerformOperation(FireCube::Ray ray, vec2 mousePos, NodeDescriptor *currentNode)
{
	bool scaled = false;
	vec3 intersectionPoint;
	vec3 scaleDelta;	

	if (currentAxis == "XAxis")
	{		
		vec3 dragDir = (currentNode->GetNode()->GetWorldRotation() * vec3(1, 0, 0)).Normalized();
		if (::MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, dragDir, ray.origin, ray.direction))
		{
			scaleDelta = vec3((intersectionPoint - dragStart).Dot(dragDir), 0.0f, 0.0f);			
			scaled = true;
		}
	}
	else if (currentAxis == "YAxis")
	{
		vec3 dragDir = (currentNode->GetNode()->GetWorldRotation() * vec3(0, 1, 0)).Normalized();
		if (::MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, dragDir, ray.origin, ray.direction))
		{
			scaleDelta = vec3(0.0f, (intersectionPoint - dragStart).Dot(dragDir), 0.0f);
			scaled = true;
		}
	}
	else if (currentAxis == "ZAxis")
	{
		vec3 dragDir = (currentNode->GetNode()->GetWorldRotation() * vec3(0, 0, 1)).Normalized();
		if (::MathUtils::ClosestPointsOnTwoLines(intersectionPoint, dragStart, dragDir, ray.origin, ray.direction))
		{
			scaleDelta = vec3(0.0f, 0.0f, (intersectionPoint - dragStart).Dot(dragDir));
			scaled = true;
		}
	}
	else if (currentAxis == "AllAxes")
	{
		scaleDelta = vec3((startMousePos.y - mousePos.y) * 0.01f);
		scaled = true;		
	}
		
	if (scaled)
	{
		endScale = startScale * (vec3(1.0f) + scaleDelta);
		currentNode->SetScale(endScale);
	}
}

void ScaleGizmo::SetSnapToGrid(bool snap)
{
	snapToGrid = snap;
}

Command *ScaleGizmo::GetCommand(EditorState *editorState, NodeDescriptor *nodeDesc)
{
	return new SetScaleCommand(editorState, "Scale", nodeDesc, startScale, endScale);
}

void ScaleGizmo::UpdateTransformation(FireCube::Camera *camera, NodeDescriptor *currentNode)
{
	node->SetTranslation(currentNode->GetNode()->GetWorldPosition());
	node->SetScale((camera->GetNode()->GetWorldPosition() - currentNode->GetNode()->GetWorldPosition()).Length() * 0.1f);
	node->SetRotation(currentNode->GetNode()->GetWorldRotation());
}