#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Dependencies/glew.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderQueue.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Math/Plane.h"
#include "Math/Frustum.h"
#include "Scene/Camera.h"
#include "Rendering/Shaders.h"
#include "Rendering/Texture.h"
using namespace FireCube;

void RenderQueue::Clear()
{
	renderJobs.clear();
	activeLights.clear();
}

void RenderQueue::AddNode(NodePtr node, CameraPtr camera)
{    
	node->PopulateRenderQueue(*this, camera);	
}

bool RenderJobCompare(const RenderJob &job1, const RenderJob &job2)
{
	// Sort by program then by texture and lastly by geometry	
	unsigned int id1, id2;
	id1 = job1.program->GetId() * 256;
	id2 = job2.program->GetId() * 256;
	if (job1.geometry->GetMaterial()->GetDiffuseTexture() && job1.geometry->GetMaterial()->GetDiffuseTexture()->IsValid())
		id1 += job1.geometry->GetMaterial()->GetDiffuseTexture()->GetId() % 255;
	if (job1.geometry->GetMaterial()->GetNormalTexture() && job1.geometry->GetMaterial()->GetNormalTexture()->IsValid())
		id1 += job1.geometry->GetMaterial()->GetNormalTexture()->GetId() % 255;
	
	if (job2.geometry->GetMaterial()->GetDiffuseTexture() && job2.geometry->GetMaterial()->GetDiffuseTexture()->IsValid())
		id2 += job2.geometry->GetMaterial()->GetDiffuseTexture()->GetId() % 255;
	if (job2.geometry->GetMaterial()->GetNormalTexture() && job2.geometry->GetMaterial()->GetNormalTexture()->IsValid())
		id2 += job2.geometry->GetMaterial()->GetNormalTexture()->GetId() % 255;

	if (id1 != id2)
		return id1 < id2;
	else
		return job1.geometry < job2.geometry;
}

void RenderQueue::Sort(QueueType type)
{	
	sort(renderJobs[type].begin(), renderJobs[type].end(), RenderJobCompare);
}

void RenderQueue::AddLight(const mat4 &transformation, const Light &light)
{
	activeLights.push_back(make_pair(transformation, light));
}

RenderJob &RenderQueue::AddRenderJob(QueueType queueType)
{
	if (queueType == NORMAL)
	{
		renderJobs[NORMAL].push_back(RenderJob());
		return renderJobs[NORMAL].back();
	}
	else
	{
		renderJobs[NON_LIGHTED].push_back(RenderJob());
		return renderJobs[NON_LIGHTED].back();
	}
}