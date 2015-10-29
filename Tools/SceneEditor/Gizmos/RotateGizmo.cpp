#include "RotateGizmo.h"
#include "../Types.h"
#include "../MathUtils.h"
#include "../Commands/TransformCommands.h"
#include "../Descriptors/NodeDescriptor.h"

using namespace FireCube;

RotateGizmo::RotateGizmo(FireCube::Engine *engine, FireCube::Node *parent) : Object(engine), snapToGrid(false)
{
	const float arcRadius = 2;
	const unsigned int arcTesselation = 50;
	const float tubeRadius = 0.2f;
	
	SharedPtr<Mesh> mesh;
	SharedPtr<Mesh> meshIntersection;
	
	StaticModel *staticModel;
	Node *child;
	node = parent->CreateChild("Editor_RotateGizmo");
	
	// X Axis
	xAxisMaterial = engine->GetResourceCache()->GetResource<Material>("Materials/Unlit.xml")->Clone();
	xAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 0.0f, 0.0f));
	mesh = new Mesh(engine);
	meshIntersection = new Mesh(engine);
	mesh->AddGeometry(CreateArc(arcRadius, PI * 0.5f, PI * 1.5f, arcTesselation), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), xAxisMaterial);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateTorus(engine, arcRadius, tubeRadius, PI, 16, 16), BoundingBox(vec3(-(arcRadius + tubeRadius), 0, -tubeRadius), vec3(arcRadius + tubeRadius, arcRadius + tubeRadius, tubeRadius)), nullptr);
	xAxis = node->CreateChild();	 
	child = xAxis->CreateChild();
	child->Rotate(vec3(0.0f, -PI * 0.5f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);
	
	child = xAxis->CreateChild("XAxis");
	child->Rotate(vec3(0.0f, PI * 0.5f, PI * 0.5f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// Y Axis
	yAxisMaterial = xAxisMaterial->Clone();
	yAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 1.0f, 0.0f));
	mesh = new Mesh(engine);
	meshIntersection = new Mesh(engine);
	mesh->AddGeometry(CreateArc(arcRadius, 0, PI, arcTesselation), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), yAxisMaterial);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateTorus(engine, arcRadius, tubeRadius, PI, 16, 16), BoundingBox(vec3(-(arcRadius + tubeRadius), 0, -tubeRadius), vec3(arcRadius + tubeRadius, arcRadius + tubeRadius, tubeRadius)), nullptr);
	yAxis = node->CreateChild();	 
	child = yAxis->CreateChild();
	child->Rotate(vec3(-PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);
	
	child = yAxis->CreateChild("YAxis");
	child->Rotate(vec3(-PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// Z Axis
	zAxisMaterial = xAxisMaterial->Clone();
	zAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 0.0f, 1.0f));
	mesh = new Mesh(engine);
	meshIntersection = new Mesh(engine);
	mesh->AddGeometry(CreateArc(arcRadius, PI * 0.5f, PI * 1.5f, arcTesselation), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), zAxisMaterial);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateTorus(engine, arcRadius, tubeRadius, PI, 16, 16), BoundingBox(vec3(-(arcRadius + tubeRadius), 0, -tubeRadius), vec3(arcRadius + tubeRadius, arcRadius + tubeRadius, tubeRadius)), nullptr);	
	zAxis = node->CreateChild();
	child = zAxis->CreateChild();
	child->Rotate(vec3(0, 0, PI));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);
	
	child = zAxis->CreateChild("ZAxis");
	child->Rotate(vec3(0, 0, PI * 0.5f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);
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
	float det = v1.x * v2.y * n.z + v2.x * n.y * v1.z + n.x * v1.y * v2.z - v1.z * v2.y * n.x - v2.z * n.y * v1.x - n.z * v1.y * v2.x;
	float dot = Dot(v1, v2);
	return std::atan2(det, dot);
}

bool RotateGizmo::CheckOperationStart(FireCube::Scene *scene, NodeDescriptor *currentNode, FireCube::Ray ray, vec2 mousePos)
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
		startRotation = currentNode->GetRotation();
		startRotationMatrix = currentNode->GetNode()->GetWorldRotation();
		if (currentAxis == "XAxis")
		{
			currentPlane = Plane(vec3(1, 0, 0), node->GetWorldPosition());
			xAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
		}
		else if (currentAxis == "YAxis")
		{
			currentPlane = Plane(vec3(0, 1, 0), node->GetWorldPosition());
			yAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
		}
		else if (currentAxis == "ZAxis")
		{
			currentPlane = Plane(vec3(0, 0, 1), node->GetWorldPosition());
			zAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
		}

		float distance;
		if (ray.IntersectPlane(currentPlane, distance))
		{
			planeStartPoint = ray.origin + ray.direction * distance;
		}

		return true;		
	}

	return false;
}

void RotateGizmo::PerformOperation(FireCube::Ray ray, vec2 mousePos, NodeDescriptor *currentNode)
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

	float distance;
	vec3 planeCurrentPoint;
	if (ray.IntersectPlane(currentPlane, distance))
	{
		planeCurrentPoint = ray.origin + ray.direction * distance;
		
		mat4 rot = mat4::IDENTITY;
		vec3 v0 = (planeCurrentPoint - currentNode->GetNode()->GetWorldPosition()).Normalized();
		vec3 v1 = (planeStartPoint - currentNode->GetNode()->GetWorldPosition()).Normalized();
		rot.Rotate(axis, GetAng(currentPlane.GetNormal(), v0, v1));

		mat4 temp = parentRotationMatrix;
		temp.Inverse();
		temp = temp * rot * startRotationMatrix;

		endRotation = temp.ExtractEulerAngles();
		currentNode->SetRotation(endRotation);		
	}
}

void RotateGizmo::SetSnapToGrid(bool snap)
{
	snapToGrid = snap;
}

Command *RotateGizmo::GetCommand(EditorState *editorState, NodeDescriptor *nodeDesc)
{
	return new SetRotationCommand(editorState, "Rotate", nodeDesc, startRotation, endRotation);
}

Geometry *RotateGizmo::CreateArc(float radius, float startAngle, float endAngle, unsigned int tesselation)
{	
	std::vector<vec3> vertices(tesselation);
	
	for (unsigned int i = 0; i < tesselation; ++i)
	{
		float angle = startAngle + (float)i / (float)(tesselation - 1) * (endAngle - startAngle);
		vertices[i] = vec3(cos(angle) * radius, sin(angle) * radius, 0);
	}

	return GeometryGenerator::GeneratePolyline(engine, vertices);
}

void RotateGizmo::UpdateTransformation(FireCube::Camera *camera, NodeDescriptor *currentNode)
{	
	node->SetTranslation(currentNode->GetNode()->GetWorldPosition());
	node->SetScale((camera->GetNode()->GetWorldPosition() - currentNode->GetNode()->GetWorldPosition()).Length() * 0.1f);

	vec3 eyeVector = (camera->GetNode()->GetWorldPosition() - node->GetWorldPosition()).Normalized();

	mat4 rotation = mat4::IDENTITY;	
	rotation.Rotate(vec3(1, 0, 0), -atan2(-eyeVector.y, eyeVector.z));
	xAxis->SetRotation(rotation);

	rotation = mat4::IDENTITY;
	rotation.Rotate(vec3(0, 1, 0), -atan2(eyeVector.x, eyeVector.z));
	yAxis->SetRotation(rotation);

	rotation = mat4::IDENTITY;
	rotation.Rotate(vec3(0, 0, 1), -atan2(eyeVector.y, eyeVector.x));
	zAxis->SetRotation(rotation);
}

void RotateGizmo::OperationEnd()
{
	if (currentAxis == "XAxis")
	{
		xAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 0.0f, 0.0f));
	}
	else if (currentAxis == "YAxis")
	{
		yAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 1.0f, 0.0f));
	}
	else if (currentAxis == "ZAxis")
	{
		zAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 0.0f, 1.0f));
	}
}
