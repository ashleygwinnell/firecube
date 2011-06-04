#ifndef FONT_H
#define FONT_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
class Font;
class Texture;
typedef boost::shared_ptr<Texture> TexturePtr;
class vec2;
class vec3;
class vec4;

/**
* A shared pointer to a FontResource.
*/
typedef boost::shared_ptr<Font> FontPtr;

namespace Renderer
{
void FIRECUBE_API RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const std::string &str);
}

class FontImpl;

/**
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

/**
* Stores information about a font page.
*/
class FIRECUBE_API FontPage
{
public:
    TexturePtr tex;
    vec2 curPos;
    int textureSize;
};

/**
* Holds the data for a single font face.
*/
class FIRECUBE_API Font
{
    friend void Renderer::RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const std::string &str);
    friend class FontManager;
    friend class ResourceManager<Font>;
public:
    Font();
    ~Font();

    /**
    * Loads a font.
    * @param name The file to load separated with : and an integer representing the font size.
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
    boost::shared_ptr<FontPage> page;
    FontImpl *fontImpl;
    int size;
};
/**
* Manages the various fonts.
*/
class FIRECUBE_API FontManager : public ResourceManager<Font>
{
    friend class Font;
    friend void Renderer::RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const std::string &str);
public:
    FontManager();
    /**
    * Creates and loads a font from the specified file.
    * @param filename The file to load.
    * @param size The size of the font.
    */
    FontPtr Create(const std::string &filename, int size);
private:
    std::vector<boost::weak_ptr<FontPage>> page;
    boost::shared_ptr<FontPage> CreateNewPage();
};
}
#pragma warning(pop)
#endif