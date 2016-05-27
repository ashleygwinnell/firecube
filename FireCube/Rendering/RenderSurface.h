#pragma once

#include "ThirdParty/GLEW/glew.h"
#include "Rendering/GraphicsResource.h"
#include "Core/Memory.h"
#include "Rendering/Texture.h"
#include "Rendering/RenderingTypes.h"
#include "Utils/Timer.h"

namespace FireCube
{

class Renderer;

class FIRECUBE_API RenderSurface : public RefCounted, public GraphicsResource
{
public:
	RenderSurface(Renderer *renderer, RenderSurfaceType type);
	~RenderSurface();

	/**
	* Creates a depth render surface
	* @param width The width of the depth buffer
	* @param height The height of the depth buffer
	*/
	void CreateDepth(int width, int height);

	/**
	* Destroys the render surface 
	*/
	void Destroy();

	/**
	* @returns The width of the render surface
	*/
	int GetWidth() const;

	/**
	* @returns The height of the render surface
	*/
	int GetHeight() const;

	/**
	* Sets the linked texture of this render surface (the texture is used as the render target of this surface)
	*/
	void SetLinkedTexture(SharedPtr<Texture> linkedTexture);

	/**
	* @returns The linked texture of this render surface
	*/
	SharedPtr<Texture> GetLinkedTexture();

	/**
	* @returns The render surface type
	*/
	RenderSurfaceType GetRenderSurfaceType() const;

	float GetLastUsed() const;

	void ResetUseTimer();
private:
	int width;
	int height;
	SharedPtr<Texture> linkedTexture;
	RenderSurfaceType type;
	Timer useTimer;
};

}