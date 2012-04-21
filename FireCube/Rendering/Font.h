#ifndef FONT_H
#define FONT_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
class Font;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
class vec2;
class vec3;
class vec4;

/**
* A shared pointer to a Font.
*/
typedef std::shared_ptr<Font> FontPtr;

namespace Renderer
{
void FIRECUBE_API RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const std::string &str);
}

class FontImpl;

/**
* @cond
* Stores information about a specific glyph.
*/
class FIRECUBE_API Glyph
{
public:
    vec2 uv;
    vec2 size;
    vec2 bitmapOffset;
    int advance;
};
/** @endcond */

/**
* @cond
* Stores information about a font page.
*/
class FIRECUBE_API FontPage
{
public:
    TexturePtr tex;
    vec2 curPos;
    int textureSize;
};
/** @endcond */

/**
* This class holds the data needed to draw text with a specific font and size.
*/
class FIRECUBE_API Font
{
    friend void Renderer::RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const std::string &str);
    friend class FontManager;
    friend class ResourcePool<Font>;
public:
    Font();
    ~Font();

    /**
    * Loads a font.
    * @param name The file to load.
    */
    bool Load(const std::string &name);

    /**
    * Loads a font.
    * @param name The file to load.
    * @param size The size of the font.
    */
    bool Load(const std::string &name, int size);

private:
    bool AddChar(char c);
    std::vector<Glyph> glyph;
    std::shared_ptr<FontPage> page;
    FontImpl *fontImpl;
    int size;
};

/**
* A font resource pool.
*/
class FIRECUBE_API FontPool : public ResourcePool<Font>
{
    friend class Font;
    friend void Renderer::RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const std::string &str);
public:
    FontPool();
    /**
    * Creates and loads a font from the specified file.
    * @param filename The file to load.
    * @param size The size of the font.
    */
    FontPtr Create(const std::string &filename, int size);
private:
    std::vector<std::weak_ptr<FontPage>> page;
    std::shared_ptr<FontPage> CreateNewPage();
};
}
#pragma warning(pop)
#endif