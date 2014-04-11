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
enum TextureFilter
{
	NEAREST, LINEAR, MIPMAP
};

class Texture;

/**
* A 2d texture.
*/
class FIRECUBE_API Texture : public Resource, public GraphicsResource
{
	friend class Renderer;
public:
	Texture(Engine *engine);
	~Texture();

	/**
	* Loads a texture.
	* @param filename The file to load.
	*/
	bool Load(const std::string &filename);	

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
	* @param minFilter The minifying filter.
	* @param magFilter The magnification filter.
	*/
	void SetFiltering(TextureFilter minFilter, TextureFilter magFilter);
	
private:		
	TextureFilter minFilter, magFilter;
};

}
#pragma warning(pop)
