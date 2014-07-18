#pragma once

#include "ThirdParty/GLEW/glew.h"
#include "Rendering/GraphicsResource.h"


namespace FireCube
{

class Renderer;

class FIRECUBE_API RenderSurface : public GraphicsResource
{
public:
	RenderSurface(Renderer *renderer);
	~RenderSurface();
	void CreateDepth(int width, int height);
	void Destroy();
	int GetWidth() const;
	int GetHeight() const;
private:
	int width;
	int height;
};

}