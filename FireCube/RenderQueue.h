#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

namespace FireCube
{
class RenderQueue;
class GeometryResource;
typedef boost::shared_ptr<GeometryResource> Geometry;
class Surface;
class MaterialResource;
typedef boost::shared_ptr<MaterialResource> Material;
class RenderParameters;
class Light;
class NodeResource;
typedef boost::shared_ptr<NodeResource> Node;
namespace Renderer
{
void FIRECUBE_API Render(RenderQueue &renderQueue);
void FIRECUBE_API Render(RenderQueue &renderQueue, const std::string &techniqueName, ProgramUniformsList &programUniformsList);
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
    Geometry geometry;

    /**
    * The surface of this job.
    */
    Surface *surface;

    /**
    * The material of this job.
    */
    Material material;

    /**
    * The rendering parameters of this job.
    */
    RenderParameters renderParameters;

    /**
    * The transformation to apply to this job.
    */
    mat4 transformation;
};
/**
* A class storing a list of rendering jobs.
*/
class FIRECUBE_API RenderQueue
{
    friend void Renderer::Render(RenderQueue &renderQueue);
	friend void Renderer::Render(RenderQueue &renderQueue, const std::string &techniqueName, ProgramUniformsList &programUniformsList);
public:

    /**
    * Clears the rendering queue.
    */
    void Clear();

    /**
    * Adds a node to the rendering queue.
    * @param node The node to enqueue.
    */
    void AddNode(Node node);
private:
    std::vector<RenderJob> renderJobs;
    std::vector<std::pair<mat4, Light>> activeLights;
};
}
#endif