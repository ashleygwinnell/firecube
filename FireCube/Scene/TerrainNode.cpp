#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <memory>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "Dependencies/glew.h"

#include "Utils/utils.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/Renderer.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
#include "Rendering/Buffer.h"
#include "Math/Plane.h"
#include "Math/Frustum.h"
#include "Scene/Camera.h"
#include "GeometryNode.h"
#include "Utils/Image.h"
#include "Geometry/Terrain.h"
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