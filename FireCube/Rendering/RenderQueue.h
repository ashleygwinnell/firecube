#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <map>
#include <vector>

#include "Utils/utils.h"
#include "Rendering/Renderer.h"
#include "Scene/Light.h"

namespace FireCube
{

// Forward declarations.
class RenderQueue;
class Geometry;
class Material;
class RenderParameters;
class Light;
class Program;
class Renderer;
class Pass;
class Shader;

class FIRECUBE_API RenderJob
{
public:
	void CalculateSortKey();

	/**
	* The geometry of this job.
	*/
	Geometry *geometry;

	/**
	* The material of this job.
	*/
	Material *material;

	/**
	* The transformation to apply to this job.
	*/
	mat4 transformation;

	Pass *pass;

	Shader *vertexShader;

	Shader *fragmentShader;

	unsigned long long int sortKey;

	float distance;

	mat4 *skinMatrices;

	unsigned int skinMatricesCount;
};

class FIRECUBE_API RenderQueue
{	
public:
	/**
	* Clears the rendering queue.
	*/
	void Clear();

	std::vector<RenderJob> &GetRenderJobs();

	void Sort();
public:	
	std::vector<RenderJob> renderJobs;
};

}

#pragma warning(pop)
