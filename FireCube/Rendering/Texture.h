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
	REPEAT, CLAMP_TO_EDGE
};

class Texture;

/**
* A 2d texture.
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
	* @param magFilter The magnification filter.
	*/
	void SetFiltering(TextureFilter filter);
	
	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int width);
	void SetHeight(int height);

	void SetWrapMode(TextureWrapMode wrapMode);

	GLenum GetTarget() const;
protected:	

	TextureFilter filter;
	TextureWrapMode wrapMode;
	GLenum target;

	int width, height;
};

}
#pragma warning(pop)
