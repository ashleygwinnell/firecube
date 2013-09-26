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
	return job1.sortKey < job2.sortKey;
	/*
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
		return job1.geometry < job2.geometry;*/
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