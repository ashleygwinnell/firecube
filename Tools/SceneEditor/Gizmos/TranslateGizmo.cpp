#include "TranslateGizmo.h"
#include "../Types.h"
#include "../MathUtils.h"
#include "../Commands/TransformCommands.h"
#include "../Descriptors/NodeDescriptor.h"

using namespace FireCube;

TranslateGizmo::TranslateGizmo(FireCube::Engine *engine, FireCube::Node *parent) : Object(engine), snapToGrid(false)
{
	const float lineLength = 1.0f;
	const float cylinderRadius = 0.15f;
	const float cylinderHeight = 0.5f;
	const unsigned int cylinderTesselation = 8;
	const float planeSizeRatioFromLine = 0.5f;
	const float intersectionBoxLength = lineLength + cylinderHeight - (lineLength * planeSizeRatioFromLine);

	SharedPtr<Mesh> mesh;
	SharedPtr<Mesh> meshIntersection;
	SharedPtr<Mesh> meshLine;

	StaticModel *staticModel;
	Node *child;
	node = parent->CreateChild("Editor_TranslateGizmo");

	// X Axis
	xAxisMaterial = engine->GetResourceCache()->GetResource<Material>("Materials/Unlit.xml")->Clone();
	xAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 0.0f, 0.0f));
	mesh = new Mesh(engine);
	meshLine = new Mesh(engine);
	meshIntersection = new Mesh(engine);	
	mesh->AddGeometry(GeometryGenerator::GenerateCylinder(engine, cylinderRadius, 0.0f, cylinderHeight, 2, cylinderTesselation), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), xAxisMaterial);
	meshLine->AddGeometry(GeometryGenerator::GeneratePolyline(engine, { vec3(0, 0, 0), vec3(lineLength, 0 ,0) }), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), xAxisMaterial);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(intersectionBoxLength, cylinderRadius * 2.0f, cylinderRadius * 2.0f)), BoundingBox(vec3(-intersectionBoxLength * 0.5f, -cylinderRadius, -cylinderRadius), vec3(intersectionBoxLength * 0.5f, cylinderRadius, cylinderRadius)), nullptr);
	Node *xAxis = node->CreateChild();
	staticModel = xAxis->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshLine);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);
	child = xAxis->CreateChild();
	child->Move(vec3(lineLength, 0.0f, 0.0f));
	child->Rotate(vec3(0.0f, 0.0f, -PI * 0.5f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = xAxis->CreateChild("XAxis");	
	child->Move(vec3(intersectionBoxLength * 0.5f + (lineLength * planeSizeRatioFromLine), 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// Y Axis
	yAxisMaterial = xAxisMaterial->Clone();
	yAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 1.0f, 0.0f));
	mesh = new Mesh(engine);
	meshLine = new Mesh(engine);
	meshIntersection = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateCylinder(engine, cylinderRadius, 0.0f, cylinderHeight, 2, cylinderTesselation), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), yAxisMaterial);
	meshLine->AddGeometry(GeometryGenerator::GeneratePolyline(engine, { vec3(0, 0, 0), vec3(0, lineLength, 0) }), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), yAxisMaterial);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(cylinderRadius * 2.0f, intersectionBoxLength, cylinderRadius * 2.0f)), BoundingBox(vec3(-cylinderRadius, -intersectionBoxLength * 0.5f, -cylinderRadius), vec3(cylinderRadius, intersectionBoxLength * 0.5f, cylinderRadius)), nullptr);
	Node *yAxis = node->CreateChild();
	staticModel = yAxis->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshLine);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);
	child = yAxis->CreateChild();
	child->Move(vec3(0.0f, lineLength, 0.0f));	
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = yAxis->CreateChild("YAxis");
	child->Move(vec3(0.0f, intersectionBoxLength * 0.5f + (lineLength * planeSizeRatioFromLine), 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// Z Axis
	zAxisMaterial = xAxisMaterial->Clone();
	zAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 0.0f, 1.0f));
	mesh = new Mesh(engine);
	meshLine = new Mesh(engine);
	meshIntersection = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateCylinder(engine, cylinderRadius, 0.0f, cylinderHeight, 2, cylinderTesselation), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), zAxisMaterial);
	meshLine->AddGeometry(GeometryGenerator::GeneratePolyline(engine, { vec3(0, 0, 0), vec3(0, 0, lineLength) }), BoundingBox(vec3(-0.05f, -0.5f, -0.05f), vec3(0.05f, 0.5f, 0.05f)), zAxisMaterial);
	meshIntersection->AddGeometry(GeometryGenerator::GenerateBox(engine, vec3(cylinderRadius * 2.0f, cylinderRadius * 2.0f, intersectionBoxLength)), BoundingBox(vec3(-cylinderRadius, -cylinderRadius, -intersectionBoxLength * 0.5f), vec3(cylinderRadius, cylinderRadius, intersectionBoxLength * 0.5f)), nullptr);
	Node *zAxis = node->CreateChild("ZAxisMain");
	staticModel = zAxis->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshLine);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);
	child = zAxis->CreateChild();
	child->Move(vec3(0.0f, 0.0f, lineLength));
	child->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(0);
	staticModel->SetEnabled(false);

	child = zAxis->CreateChild("ZAxis");
	child->Move(vec3(0.0f, 0.0f, intersectionBoxLength * 0.5f + (lineLength * planeSizeRatioFromLine)));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(meshIntersection);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// XY Plane
	xyPlaneMaterial = engine->GetResourceCache()->GetResource<Material>("Materials/UnlitAlpha.xml")->Clone();
	xyPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
	xyPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 0.5f);
	mesh = new Mesh(engine);	
	mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, vec2(lineLength * planeSizeRatioFromLine, lineLength * planeSizeRatioFromLine)), BoundingBox(vec3(-lineLength * planeSizeRatioFromLine * 0.5f), vec3(lineLength * planeSizeRatioFromLine * 0.5f)), xyPlaneMaterial);
	Node *xyPlane = node->CreateChild("XYPlaneMain");	
	xyPlane->Move(vec3(lineLength * planeSizeRatioFromLine * 0.5f, lineLength * planeSizeRatioFromLine * 0.5f, 0.0f));
	child = xyPlane->CreateChild("XYPlane");
	child->Rotate(vec3(PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);	
	child = xyPlane->CreateChild("XYPlane");
	child->Rotate(vec3(-PI * 0.5f, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// XZ Plane
	xzPlaneMaterial = xyPlaneMaterial->Clone();
	xzPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 0.0f, 1.0f));
	xzPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 0.5f);
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, vec2(lineLength * planeSizeRatioFromLine, lineLength * planeSizeRatioFromLine)), BoundingBox(vec3(-lineLength * planeSizeRatioFromLine * 0.5f), vec3(lineLength * planeSizeRatioFromLine * 0.5f)), xzPlaneMaterial);
	Node *xzPlane = node->CreateChild("XZPlaneMain");
	xzPlane->Move(vec3(lineLength * planeSizeRatioFromLine * 0.5f, 0.0f, lineLength * planeSizeRatioFromLine * 0.5f));
	child = xzPlane->CreateChild("XZPlane");	
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);
	child = xzPlane->CreateChild("XZPlane");
	child->Rotate(vec3(PI, 0.0f, 0.0f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);

	// YZ Plane
	yzPlaneMaterial = xyPlaneMaterial->Clone();
	yzPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 1.0f, 1.0f));
	yzPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 0.5f);
	mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, vec2(lineLength * planeSizeRatioFromLine, lineLength * planeSizeRatioFromLine)), BoundingBox(vec3(-lineLength * planeSizeRatioFromLine * 0.5f), vec3(lineLength * planeSizeRatioFromLine * 0.5f)), yzPlaneMaterial);
	Node *yzPlane = node->CreateChild("YZPlaneMain");
	yzPlane->Move(vec3(0.0f, lineLength * planeSizeRatioFromLine * 0.5f, lineLength * planeSizeRatioFromLine * 0.5f));
	child = yzPlane->CreateChild("YZPlane");
	child->Rotate(vec3(0.0f, 0.0f, PI * 0.5f));
	staticModel = child->CreateComponent<StaticModel>();
	staticModel->CreateFromMesh(mesh);
	staticModel->SetCollisionQueryMask(GIZMO_GEOMETRY);
	staticModel->SetEnabled(false);
	child = yzPlane->CreateChild("YZPlane");
	child->Rotate(vec3(0.0f, 0.0f, -PI * 0.5f));
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
			parentRotationQuat = currentNode->GetNode()->GetParent()->GetWorldRotation();
		}
		else
		{
			parentRotationQuat.Identity();
		}
		currentAxis = node->GetName();
		dragStart = query.ray.origin + query.ray.direction * result.distance;
		startPosition = currentNode->GetTranslation();

		if (currentAxis == "XAxis")
		{			
			xAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
		}
		else if (currentAxis == "YAxis")
		{		
			yAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
		}
		else if (currentAxis == "ZAxis")
		{		
			zAxisMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
		}
		else if (currentAxis == "XYPlane")
		{
			xyPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
			xyPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
		}
		else if (currentAxis == "XZPlane")
		{
			xzPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
			xzPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
		}
		else if (currentAxis == "YZPlane")
		{
			yzPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
			yzPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
		}

		return true;
	}

	return false;
}

void TranslateGizmo::PerformOperation(FireCube::Ray ray, FireCube::vec2 mousePos, NodeDescriptor *currentNode)
{	
	vec3 translation;
	bool moved = false;
	if (currentAxis == "XAxis")
	{		
		vec3 intersectionPoint;
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
	else if (currentAxis == "XYPlane")
	{
		Plane p(vec3(0, 0, 1), dragStart);
		float distance;

		if (ray.IntersectPlane(p, distance))
		{
			vec3 intersectionPoint = ray.origin + ray.direction * distance;
			translation = vec3((intersectionPoint.x - dragStart.x), (intersectionPoint.y - dragStart.y), 0.0f);
			moved = true;
		}
	}
	else if (currentAxis == "XZPlane")
	{
		Plane p(vec3(0, 1, 0), dragStart);
		float distance;

		if (ray.IntersectPlane(p, distance))
		{
			vec3 intersectionPoint = ray.origin + ray.direction * distance;
			translation = vec3((intersectionPoint.x - dragStart.x), 0.0f, (intersectionPoint.z - dragStart.z));
			moved = true;
		}
	}
	else if (currentAxis == "YZPlane")
	{
		Plane p(vec3(1, 0, 0), dragStart);
		float distance;

		if (ray.IntersectPlane(p, distance))
		{
			vec3 intersectionPoint = ray.origin + ray.direction * distance;
			translation = vec3(0.0f, (intersectionPoint.y - dragStart.y), (intersectionPoint.z - dragStart.z));
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

		quat temp = parentRotationQuat;
		temp.Conjugate();
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

void TranslateGizmo::OperationEnd()
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
	else if (currentAxis == "XYPlane")
	{
		xyPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 1.0f, 0.0f));
		xyPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 0.5f);
	}
	else if (currentAxis == "XZPlane")
	{
		xzPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f, 0.0f, 1.0f));
		xzPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 0.5f);
	}
	else if (currentAxis == "YZPlane")
	{
		yzPlaneMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.0f, 1.0f, 1.0f));
		yzPlaneMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 0.5f);
	}
}
