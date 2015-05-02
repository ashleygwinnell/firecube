#include "RotateGizmo.h"
#include "../Types.h"
#include "../MathUtils.h"
#include "../Commands/TransformCommands.h"
using namespace FireCube;

RotateGizmo::RotateGizmo(FireCube::Engine *engine, FireCube::Node *parent) : Object(engine), snapToGrid(false)
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
}

void RotateGizmo::SetPosition(vec3 position)
{
	node->SetTranslation(position);
}

void RotateGizmo::SetRotation(mat4 rotation)
{
	node->SetRotation(rotation);
}

void RotateGizmo::Show()
{
	std::vector<StaticModel *> components;
	node->GetComponents<StaticModel>(components, true);
	for (auto c : components)
	{
		c->SetEnabled(true);
	}
}

void RotateGizmo::Hide()
{
	std::vector<StaticModel *> components;
	node->GetComponents<StaticModel>(components, true);
	for (auto c : components)
	{
		c->SetEnabled(false);
	}
}

float GetAng(vec3 n, vec3 v1, vec3 v2)
{
	float det = (n.x * v1.y * v2.z + v1.x * v2.y * n.x + v2.x * n.y * v1.z) - (v2.x * v1.y * n.z + v1.x * n.y * v2.z + n.x * v2.y * v1.z);
	float dot = Dot(v1, v2);
	return std::atan2(dot, det);
}

bool RotateGizmo::CheckOperationStart(FireCube::Scene *scene, FireCube::Node *currentNode, FireCube::Ray ray, vec2 mousePos)
{
	RayQuery query(ray, 10e4);

	scene->IntersectRay(query, GIZMO_GEOMETRY);
	if (query.results.empty() == false)
	{
		auto &result = query.results.front();
		Node *node = result.renderable->GetNode();
		currentAxis = node->GetName();				
		lastMousePos = mousePos;
		startRotation = currentNode->GetRotation();
		return true;		
	}

	return false;
}

void RotateGizmo::PerformOperation(FireCube::Ray ray, vec2 mousePos, FireCube::Node *currentNode)
{
	vec3 axis;
	if (currentAxis == "XAxis")
	{
		axis = vec3(0.0f, 1.0f, 0.0f);
	}
	else if (currentAxis == "YAxis")
	{
		axis = vec3(1.0f, 0.0f, 0.0f);
	}
	else if (currentAxis == "ZAxis")
	{
		axis = vec3(0.0f, 0.0f, 1.0f);
	}
	
	mat3 rotMat = currentNode->GetRotation().ToMat3();
	rotMat.Inverse();	
	axis = (rotMat * axis).Normalized();	
	mat4 rotationMatrix = mat4::IDENTITY;
	rotationMatrix.Rotate(axis, (lastMousePos.x - mousePos.x) * 0.01f);
	endRotation = currentNode->GetRotation() * rotationMatrix;
	currentNode->SetRotation(endRotation);
	lastMousePos = mousePos;
	
	
}

void RotateGizmo::SetSnapToGrid(bool snap)
{
	snapToGrid = snap;
}

void RotateGizmo::SetScale(float scale)
{
	node->SetScale(vec3(scale));
}

Command *RotateGizmo::GetCommand(EditorState *editorState, NodeDescriptor *nodeDescriptor)
{
	return new SetRotationCommand(editorState, nodeDescriptor, startRotation, endRotation);
}