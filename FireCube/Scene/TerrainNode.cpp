#include "Geometry/Geometry.h"
#include "Rendering/RenderQueue.h"
#include "Scene/Camera.h"
#include "Scene/TerrainNode.h"

using namespace FireCube;

TerrainNode::TerrainNode() : Node()
{
	type = Node::TERRAIN;
}

void TerrainNode::PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera)
{
	BoundingBox boundingBox = GetWorldBoundingBox();
	if (camera->GetFrustum().Contains(boundingBox))
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
		terrain.Render(camera);
		job->geometry = terrain.GetGeometry();
		job->renderParameters = renderParameters;
		job->transformation = GetWorldTransformation();					
	}
	Node::PopulateRenderQueue(renderQueue, camera);	
}

Terrain &TerrainNode::GetTerrain()
{
	return terrain;
}

MaterialPtr TerrainNode::GetMaterial()
{
	return terrain.GetGeometry()->GetMaterial();
}

BoundingBox TerrainNode::GetLocalBoundingBox() const
{
	return terrain.GetBoundingBox();
}