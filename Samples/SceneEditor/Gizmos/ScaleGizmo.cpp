#include "ScaleGizmo.h"
#include "../Types.h"
#include "../MathUtils.h"
#include "../Commands/TransformCommands.h"
using namespace FireCube;

ScaleGizmo::ScaleGizmo(FireCube::Engine *engine, FireCube::Node *parent) : Object(engine), snapToGrid(false)
{
	SharedPtr<Material> material = engine->GetResourceCache()->GetResource<Material>("Materials/Gizmo.xml")->Clone();
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

	material = material->Clone();
	material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(0.2f, 0.2f, 0.2f)), material);
	mesh->SetBoundingBox(BoundingBox(vec3(-0.1f, -0.1f, -0.1f), vec3(0.1f, 0.1f, 0.1f)));
	child = node->CreateChild("AllAxes");	
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);
}

void ScaleGizmo::SetPosition(vec3 position)
{
	node->SetTranslation(position);
}

void ScaleGizmo::SetRotation(mat4 rotation)
{
	node->SetRotation(rotation);
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

bool ScaleGizmo::CheckOperationStart(FireCube::Scene *scene, FireCube::Node *currentNode, FireCube::Ray ray, vec2 mousePos)
{
	RayQuery query(ray, 10e4);

	scene->IntersectRay(query, GIZMO_GEOMETRY);
	if (query.results.empty() == false)
	{
		auto &result = query.results.front();
		Node *node = result.renderable->GetNode();
		currentAxis = node->GetName();
		lastMousePos = mousePos;
		startScale = currentNode->GetScale();
		return true;
	}

	return false;
}

void ScaleGizmo::PerformOperation(FireCube::Ray ray, vec2 mousePos, FireCube::Node *currentNode)
{
	vec3 axis;
	if (currentAxis == "XAxis")
	{		
		axis = vec3(1.0f, 0.0f, 0.0f);
	}
	else if (currentAxis == "YAxis")
	{
		axis = vec3(0.0f, 1.0f, 0.0f);
	}
	else if (currentAxis == "ZAxis")
	{
		axis = vec3(0.0f, 0.0f, 1.0f);
	}
	else if (currentAxis == "AllAxes")
	{
		axis = vec3(1.0f, 1.0f, 1.0f);
	}
	
	endScale = currentNode->GetScale() + axis * (mousePos.x - lastMousePos.x) * 0.01f;
	currentNode->SetScale(endScale);
	
	lastMousePos = mousePos;


}

void ScaleGizmo::SetSnapToGrid(bool snap)
{
	snapToGrid = snap;
}

void ScaleGizmo::SetScale(float scale)
{
	node->SetScale(vec3(scale));
}

Command *ScaleGizmo::GetCommand(EditorState *editorState, Node *node)
{
	return new SetScaleCommand(editorState, node, startScale, endScale);
}