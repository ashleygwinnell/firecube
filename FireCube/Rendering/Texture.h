#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/utils.h"
#include "Rendering/GraphicsResource.h"
#include "Core/Resource.h"

namespace FireCube
{

enum class TextureFormat
{
	RGB,
	RGBA,
	R,
	DEPTH
};

// Forward declarations.
class Renderer;

/**
* Specifies the type of texture filtering.
*/
enum class TextureFilter
{
	NEAREST, LINEAR, MIPMAP
};

enum class TextureWrapMode
{
	REPEAT, CLAMP_TO_EDGE, CLAMP
};

class Texture;

/**
* A base class for textures.
*/
class FIRECUBE_API Texture : public Resource, public GraphicsResource
{
	FIRECUBE_OBJECT(Texture)
	friend class Renderer;
public:
	Texture(Engine *engine);
	~Texture();

	/**
	* Creates a new texture.
	*/
	bool Create();

	/**
	* Generates mipmaps for the texture.
	*/
	void GenerateMipMaps();

	/**
	* Sets the texture filtering.
	* @param filter The filter.	
	*/
	void SetFiltering(TextureFilter filter);

	/**
	* @returns The filtering used for this texture
	*/
	TextureFilter GetFiltering() const;
	
	/**
	* @returns The width of the texture
	*/
	int GetWidth() const;

	/**
	* @returns The height of the texture
	*/
	int GetHeight() const;

	/**
	* Sets the width of the texture
	* @param width The width of the texture
	*/
	void SetWidth(int width);

	/**
	* Sets the height of the texture
	* @param height The height of the texture
	*/
	void SetHeight(int height);

	/**
	* Sets the wrap mode for the texture
	* @param wrapMode The wrap mode to set
	*/
	void SetWrapMode(TextureWrapMode wrapMode);

	/**
	* @returns The texture target to use for this texture (GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP, ...)
	*/
	GLenum GetTarget() const;

	TextureFormat GetFormat() const;
protected:	

	TextureFilter filter;
	TextureWrapMode wrapMode;
	GLenum target;

	int width, height;
	TextureFormat format;
};

}
#pragma warning(pop)
