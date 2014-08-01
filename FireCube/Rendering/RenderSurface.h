#pragma once

#include "ThirdParty/GLEW/glew.h"
#include "Rendering/GraphicsResource.h"
#include "Core/Memory.h"
#include "Rendering/Texture.h"

namespace FireCube
{

class Renderer;

class FIRECUBE_API RenderSurface : public RefCounted, public GraphicsResource
{
public:
	RenderSurface(Renderer *renderer);
	~RenderSurface();
	void CreateDepth(int width, int height);
	void Destroy();
	int GetWidth() const;
	int GetHeight() const;
	void SetLinkedTexture(SharedPtr<Texture> linkedTexture);
	SharedPtr<Texture> GetLinkedTexture();
private:
	int width;
	int height;
	SharedPtr<Texture> linkedTexture;
};

}