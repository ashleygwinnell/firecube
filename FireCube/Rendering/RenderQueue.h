#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#pragma warning(push)
#pragma warning(disable:4251)

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
class Node;
typedef std::shared_ptr<Node> NodePtr;
class Program;
typedef std::shared_ptr<Program> ProgramPtr;

namespace Renderer
{
void FIRECUBE_API Render(RenderQueue &renderQueue);
void FIRECUBE_API Render(RenderQueue &renderQueue, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);
}
/**
* A class storing information of a rendering job.
*/
class FIRECUBE_API RenderJob
{
public:
	/**
	* The geometry of this job.
	*/
	GeometryPtr geometry;
	
	/**
	* The rendering parameters of this job.
	*/
	RenderParameters renderParameters;

	/**
	* The transformation to apply to this job.
	*/
	mat4 transformation;

	/**
	* The program used to render this job.
	*/
	ProgramPtr program;
};
/**
* A class storing a list of rendering jobs.
*/
class FIRECUBE_API RenderQueue
{
	friend void Renderer::Render(RenderQueue &renderQueue);
	friend void Renderer::Render(RenderQueue &renderQueue, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);
public:
	/**
	* An enumeration of possible queue types.
	*/
	enum QueueType
	{
		NORMAL, NON_LIGHTED
	};


	/**
	* Clears the rendering queue.
	*/
	void Clear();

	/**
	* Adds a node to the rendering queue.
	* @param node The node to enqueue.
	* @param camera The camera used to cull this node.
	*/
	void AddNode(NodePtr node, CameraPtr camera);

	/**
	* Sorts the render queue to reduce state changes.
	* @param type The queue type to sort.
	*/
	void Sort(QueueType type);

	void AddLight(const mat4 &transformation, const Light &light);

	RenderJob &AddRenderJob(QueueType queueType);
private:	
	std::map<QueueType, std::vector<RenderJob>> renderJobs;
	std::vector<std::pair<mat4, Light>> activeLights;
};
}

#pragma warning(pop)

#endif