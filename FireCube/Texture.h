#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
template<class T>
class ResourceManager;

/**
* Specifies the type of texture filtering.
*/
enum TextureFilter
{
    NEAREST, LINEAR, MIPMAP
};

class TextureResource;

/**
* A shared pointer to a TextureResource.
*/
typedef boost::shared_ptr<TextureResource> Texture;

namespace Renderer
{
void FIRECUBE_API UseTexture(Texture tex, unsigned int unit);
}

/**
* A 2d texture.
*/
class FIRECUBE_API TextureResource
{
    friend class ResourceManager<TextureResource>;
    friend void Renderer::UseTexture(Texture tex, unsigned int unit);
public:
    TextureResource();
    ~TextureResource();

    /**
    * Loads a texture.
    * @param filename The file to load.
    */
    bool Load(const std::string &filename);

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
    void SetFiltering(TextureFilter minFilter, TextureFilter magFilter);

    /**
    * Returns the file name of the texture.
    */
    std::string GetFileName() const;

    /**
    * Returns the resource id of the texture.
    */
    unsigned int GetId() const;

private:
    GLuint id;
    std::string filename;
};

/**
* A texture resource manager.
*/
typedef ResourceManager<TextureResource> TextureManager;
}
#pragma warning(pop)
#endif