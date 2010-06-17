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
/**
* A 2d texture.
*/
class FIRECUBE_API TextureResource
{
public:
	TextureResource();
	~TextureResource();
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

	GLuint id;
	string filename;
};	

typedef ResourceManager<TextureResource> TextureManager;
typedef boost::shared_ptr<TextureResource> Texture;
}
#pragma warning(pop)
#endif