#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"

#include "utils.h"
#include "Logger.h"
#include "MyMath.h"
#include "BoundingBox.h"
#include "Geometry.h"
#include "Renderer.h"
#include "RenderQueue.h"
#include "Light.h"
#include "Node.h"
#include "Plane.h"
#include "Frustum.h"
#include "Camera.h"
#include "Shaders.h"
#include "Texture.h"
using namespace FireCube;

void RenderQueue::Clear()
{
    renderJobs.clear();
    activeLights.clear();
}

void RenderQueue::AddNode(NodePtr node)
{
	if (node->GetGeometry())
	{
		renderJobs.push_back(RenderJob());
		RenderJob &job = renderJobs.back();		
		job.geometry = node->GetGeometry();		
		job.renderParameters = node->GetRenderParameters();
		job.transformation = node->GetWorldTransformation();		
	}	    
    for (vector<Light>::iterator i = node->GetLights().begin(); i != node->GetLights().end(); i++)
    {
        activeLights.push_back(make_pair(node->GetWorldTransformation(), *i));
    }
    for (vector<NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
        AddNode(*i);
}

void RenderQueue::AddNode(NodePtr node, CameraPtr camera)
{    
	BoundingBox boundingBox = node->GetWorldBoundingBox();
	if (camera->GetFrustum().Contains(boundingBox))
	{
		if (node->GetGeometry())
		{
			renderJobs.push_back(RenderJob());
			RenderJob &job = renderJobs.back();		
			job.geometry = node->GetGeometry();		
			job.renderParameters = node->GetRenderParameters();
			job.transformation = node->GetWorldTransformation();			
		}
		for (vector<Light>::iterator i = node->GetLights().begin(); i != node->GetLights().end(); i++)
		{
			activeLights.push_back(make_pair(node->GetWorldTransformation(), *i));
		}
		for (vector<NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
			AddNode(*i, camera);
	}	
}
bool RenderJobCompare(const RenderJob &job1, const RenderJob &job2)
{
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
void RenderQueue::Sort()
{
	sort(renderJobs.begin(), renderJobs.end(), RenderJobCompare);
}