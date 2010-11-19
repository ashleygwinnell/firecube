#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

namespace FireCube
{
	class RenderQueue;
	namespace Renderer
	{
		void FIRECUBE_API Render(const RenderQueue &queue);
	}
	class RenderJob
	{
	public:
		Geometry geometry;
		Surface *surface;
		Material material;
		Program program;
	};
	class RenderQueue
	{
		friend void Renderer::Render(const RenderQueue &queue);
	public:
		void Clear();
		void AddNode(Node node);
	private:
		std::vector<RenderJob> renderJobs;
	};
}
#endif