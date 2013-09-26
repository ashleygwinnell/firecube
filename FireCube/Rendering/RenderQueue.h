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
typedef std::shared_ptr<Geometry> GeometryPtr;
class Material;
typedef std::shared_ptr<Material> MaterialPtr;
class RenderParameters;
class Light;
class Program;
typedef std::shared_ptr<Program> ProgramPtr;
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
	* The transformation to apply to this job.
	*/
	mat4 transformation;

	Pass *pass;

	Shader *vertexShader;

	Shader *fragmentShader;

	unsigned long long int sortKey;
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

class LightQueue
{
public:
	Light *light;
	RenderQueue renderQueue;
};

}

#pragma warning(pop)
