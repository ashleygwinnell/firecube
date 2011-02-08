#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

namespace FireCube
{
	class RenderQueue;
	namespace Renderer
	{
		void FIRECUBE_API Render(const RenderQueue &queue);
	}
	/**
	* A class storing information of a rendering job.
	*/
	class RenderJob
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
		* The program of this job.
		*/
		Program program;
	};
	/**
	* A class storing a list of rendering jobs.
	*/
	class RenderQueue
	{
		friend void Renderer::Render(const RenderQueue &queue);
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
	};
}
#endif