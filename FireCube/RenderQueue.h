#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

namespace FireCube
{
	class RenderQueue;
	class Geometry;
	class Surface;
	class Material;
	class RenderParameters;
	class Light;
	class Node;
	namespace Renderer
	{
		void FIRECUBE_API Render(RenderQueue &renderQueue);
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