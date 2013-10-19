#include <algorithm>

#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/RenderQueue.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"

using namespace FireCube;

bool RenderJobCompare(const RenderJob &job1, const RenderJob &job2)
{
	if (job1.sortKey != job2.sortKey)
		return job1.sortKey < job2.sortKey;
	else
		return job1.distance < job2.distance;	
}


void RenderQueue::Sort()
{
	std::sort(renderJobs.begin(), renderJobs.end(), &RenderJobCompare);
}

std::vector<RenderJob> &RenderQueue::GetRenderJobs()
{
	return renderJobs;
}

void RenderQueue::Clear()
{
	renderJobs.clear();
}

void RenderJob::CalculateSortKey()
{	
	unsigned int shaderKey = ((unsigned int) vertexShader + (unsigned int) fragmentShader) & 0xFFFF;
	unsigned int materialKey = ((unsigned int) geometry->GetMaterial().get()) & 0xFFFF;
	unsigned int geometryKey = ((unsigned int) geometry) & 0xFFFF;
	sortKey = (((unsigned long long int) shaderKey) << 32) | (((unsigned long long int) materialKey) << 16) 
		| ((unsigned long long int) geometryKey);
}