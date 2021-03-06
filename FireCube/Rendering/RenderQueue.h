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

	/**
	* The rendering pass used for this render job
	*/
	Pass *pass;

	/**
	* The vertex shader used for this job
	*/
	Shader *vertexShader;

	/**
	* The fragment shader used for this job
	*/
	Shader *fragmentShader;

	/**
	* The sorting key of this job
	*/
	unsigned long long int sortKey;

	/**
	* The distance from the camera
	*/
	float distance;

	/**
	* An array of skin matrices
	*/
	mat4 *skinMatrices;

	/**
	* The number of skin matrices
	*/
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
