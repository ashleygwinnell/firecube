#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
enum TextureFilter
{
	NEAREST,LINEAR,MIPMAP
};

class FIRECUBE_API TextureResource
{
public:
	TextureResource();
	~TextureResource();	
	GLuint id;
	string filename;
};	
class Texture;
namespace Renderer
{
	void FIRECUBE_API UseTexture(Texture tex,unsigned int unit);
}

/**
* A 2d texture.
*/
class FIRECUBE_API Texture
{
	friend class ResourceManager<Texture,TextureResource>;
	friend void Renderer::UseTexture(Texture tex,unsigned int unit);
public:
	Texture();
	Texture(boost::shared_ptr<TextureResource> resource);
	/**
	* Loads a texture.
	* @param filename The file to load.
	*/
	bool Load(const string &filename);
	/**
	* Returns whether the texture is valid.
	*/
	bool IsValid();	
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
	void SetFiltering(TextureFilter minFilter,TextureFilter magFilter);
	/**
	* Returns the file name of the texture.
	*/
	string GetFileName() const;
	/**
	* Returns the resource id of the texture.
	*/
	unsigned int GetId() const;

	operator bool () const;
	bool operator== (const Texture &texture) const;
private:
	boost::shared_ptr<TextureResource> resource;
};	

typedef ResourceManager<Texture,TextureResource> TextureManager;
}
#pragma warning(pop)
#endif