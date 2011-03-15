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
using namespace FireCube;

void RenderQueue::Clear()
{
    renderJobs.clear();
    activeLights.clear();
}

void RenderQueue::AddNode(Node node)
{
    if (node->GetParent())
        node->SetWorldTransformation(node->GetParent()->GetWorldTransformation()*node->GetLocalTransformation());
    else
        node->SetWorldTransformation(node->GetLocalTransformation());
    for (vector<Geometry>::iterator i = node->GetGeometries().begin(); i != node->GetGeometries().end(); i++)
    {
        Geometry geometry = *i;
        for (vector<Surface>::iterator j = geometry->GetSurfaces().begin(); j != geometry->GetSurfaces().end(); j++)
        {
            RenderJob job;
            job.surface = &(*j);
            job.geometry = geometry;
            job.material = job.surface->material;
            job.renderParameters = node->GetRenderParameters();
            job.transformation = node->GetWorldTransformation();
            renderJobs.push_back(job);
        }
    }
    for (vector<Light>::iterator i = node->GetLights().begin(); i != node->GetLights().end(); i++)
    {
        activeLights.push_back(make_pair(node->GetWorldTransformation(), *i));
    }
    for (vector<Node>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
        AddNode(*i);
}