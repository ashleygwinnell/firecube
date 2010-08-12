#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace FireCube
{
class FrameBuffer;

/* CPPDOC_BEGIN_EXCLUDE */
namespace Renderer
{
	void FIRECUBE_API UseFrameBuffer(FrameBuffer frameBuffer);
}

/* CPPDOC_END_EXCLUDE */

/**
* A class representing a frame buffer.
*/
class FIRECUBE_API FrameBufferResource
{
public:	
	FrameBufferResource();
	~FrameBufferResource();

	int width,height;
	GLuint id;
	GLuint depthBuffer;
	Texture texture[MAX_TEXTURES];
	Texture depthTexture;
};
class FIRECUBE_API FrameBuffer
{
public:
	friend void Renderer::UseFrameBuffer(FrameBuffer frameBuffer);
	/**
	* Creates a frame buffer.
	* @param width The width of the frame buffer and it's sub-images(depth buffer, render targets).
	* @param height The height of the frame buffer and it's sub-images(depth buffer, render targets).
	*/
	void Create(int width,int height);
	/**
	* Adds a depth buffer as a renderbuffer.
	*/
	void AddDepthBuffer();
	/**
	* Adds a depth buffer as a texture.
	*/
	void AddDepthBufferTexture();
	/**
	* Adds a new render target.
	* @param attachmentPoint The attachment point to add to.
	*/
	void AddRenderTarget(int attachmnetPoint);
	/**
	* Sets a render target from an already existing texture.
	* @param texture The texture.
	* @param attachmentPoint The attachment point to add to.
	*/
	void SetRenderTarget(Texture texture,int attachmnetPoint);
	/**
	* Returns a texture from an attachment point.
	*/
	Texture GetRenderTarget(int attachmnetPoint);	
	/**
	* Returns a texture from the depth buffer.
	*/
	Texture GetDepthBuffer();	
	/**
	* Checks whether the frame buffer is valid and can be rendered to. 
	*/
	bool IsValid();
	/**
	* Returns the width of the frame buffer.
	*/
	int GetWidth();
	/**
	* Returns the height of the frame buffer.
	*/
	int GetHeight();
	/**
	* Returns the resource id of the frame buffer.
	*/
	unsigned int GetId() const;

	operator bool () const;
	bool operator== (const FrameBuffer &frameBuffer) const;
private:
	boost::shared_ptr<FrameBufferResource> resource;
};
}

#endif