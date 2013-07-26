#include "Geometry/Geometry.h"
#include "Rendering/RenderQueue.h"
#include "Scene/Camera.h"
#include "GeometryNode.h"

using namespace FireCube;

GeometryNode::GeometryNode() : Node()
{
	type = Node::GEOMETRY;
}

GeometryNode::GeometryNode(const std::string &name) : Node(name)
{
	type = Node::GEOMETRY;
}

void GeometryNode::PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera)
{
	BoundingBox boundingBox = GetWorldBoundingBox();
	if (GetGeometry() && camera->GetFrustum().Contains(boundingBox))
	{
		RenderJob *job;
		if (renderParameters.lighting)
		{			
			job = &renderQueue.AddRenderJob(RenderQueue::NORMAL);
		}
		else if (!renderParameters.lighting)
		{
			job = &renderQueue.AddRenderJob(RenderQueue::NON_LIGHTED);
		}

		job->geometry = geometry;
		job->renderParameters = renderParameters;
		job->transformation = GetWorldTransformation();					
	}
	Node::PopulateRenderQueue(renderQueue, camera);
}

void GeometryNode::SetGeometry(GeometryPtr geometry)
{
	this->geometry = geometry;
	SetBoundingBoxChanged();
}

GeometryPtr GeometryNode::GetGeometry()
{
	return geometry;
}

NodePtr GeometryNode::Clone() const
{
	GeometryNodePtr ret(new GeometryNode);	
	ret->name = name;
	ret->renderParameters = renderParameters;
	ret->translation = translation;
	ret->rotation = rotation;
	ret->scale = scale;
	ret->localTransformation = localTransformation;
	ret->localTransformationChanged = localTransformationChanged;
	ret->worldTransformation = worldTransformation;
	ret->worldTransformationChanged = worldTransformationChanged;
	ret->worldBoundingBox = worldBoundingBox;
	ret->worldBoundingBoxChanged = worldBoundingBoxChanged;
	ret->geometry = geometry;
	for (std::vector<NodePtr>::const_iterator i = children.begin(); i != children.end(); i++)
	{
		NodePtr c = (*i)->Clone();
		c->SetParent(ret);
	}
	return ret;
}

BoundingBox GeometryNode::GetLocalBoundingBox() const
{
	if (geometry)
		return geometry->GetBoundingBox();
	return BoundingBox();
}