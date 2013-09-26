#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "Utils/utils.h"
#include "Rendering/GraphicsResource.h"

namespace FireCube
{

// Forward declarations.
class FrameBuffer;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
class Renderer;

/**
* A shared pointer to a FrameBuffer.
*/
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

/**
* A class representing a frame buffer.
*/
class FIRECUBE_API FrameBuffer : public Object, public GraphicsResource
{
    friend class Renderer;
public:
    FrameBuffer(Engine *engine);
    ~FrameBuffer();
    /**
    * Creates a frame buffer.
    * @param width The width of the frame buffer and it's sub-images(depth buffer, render targets).
    * @param height The height of the frame buffer and it's sub-images(depth buffer, render targets).
    */
    void Create(int width, int height);
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
    void AddRenderTarget(int attachmentPoint);
    /**
    * Sets a render target from an already existing texture.
    * @param texture The texture.
    * @param attachmentPoint The attachment point to add to.
    */
    void SetRenderTarget(TexturePtr texture, int attachmentPoint);
    /**
    * Returns a texture from an attachment point.
    */
    TexturePtr GetRenderTarget(int attachmentPoint);
    /**
    * Returns a texture from the depth buffer.
    */
    TexturePtr GetDepthBuffer();
    /**
    * Checks whether the frame buffer is valid and can be rendered to.
    */
    bool IsValid() const;
    /**
    * Returns the width of the frame buffer.
    */
    int GetWidth() const;
    /**
    * Returns the height of the frame buffer.
    */
    int GetHeight() const;  

private:
    int width, height;
    
    GLuint depthBuffer;
    TexturePtr texture[MAX_TEXTURES];
    TexturePtr depthTexture;
};
}

#pragma warning(pop)
