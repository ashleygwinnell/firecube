#ifndef FONT_H
#define FONT_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
class FontResource;
class Font;

namespace Renderer
{
	void FIRECUBE_API RenderText(Font font,vec3 pos,vec4 color,const std::string &str);
}

class FontImpl;

class FIRECUBE_API Glyph
{
public:
	vec2 uv;
	vec2 size;
	vec2 bitmapOffset;
	int advance;
};
class FIRECUBE_API FontPage
{
public:
	Texture tex;
	vec2 curPos;
	int textureSize;
};

class FIRECUBE_API FontResource
{
	friend void Renderer::RenderText(Font font,vec3 pos,vec4 color,const std::string &str);
public:
	FontResource();
	~FontResource();	
	
	std::vector<Glyph> glyph;
	boost::shared_ptr<FontPage> page;
	FontImpl *fontImpl;
	int size;
};

/**
* Holds the data for a single font face.
*/
class FIRECUBE_API Font
{
	friend void Renderer::RenderText(Font font,vec3 pos,vec4 color,const std::string &str);
	friend class FontManager;
	friend class ResourceManager<Font,FontResource>;
public:
	Font();
	
	/**
	* Constructs a font from a font resource.
	* @param resource The resource to assign to this font.
	*/
	Font(boost::shared_ptr<FontResource> resource);
	
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
	bool Load(const std::string &name,int size);	

	/**
	* @return True if points to an initialized resource, false otherwise.
	*/
	operator bool () const;
	
	/**
	* @return True of both objects points to the same resource, false otherwise.
	*/
	bool operator== (const Font &font) const;
private:
	bool AddChar(char c);
	boost::shared_ptr<FontResource> resource;
};
/**
* Manages the various fonts.
*/
class FIRECUBE_API FontManager : public ResourceManager<Font,FontResource>
{
	friend class FontResource;
	friend void Renderer::RenderText(Font font,vec3 pos,vec4 color,const std::string &str);
	friend class Font;
public:
	FontManager();
	/**
	* Creates and loads a font from the specified file.
	* @param filename The file to load.
	* @param size The size of the font.
	*/
	Font Create(const std::string &filename,int size);
private:
	std::vector<boost::weak_ptr<FontPage>> page;
	boost::shared_ptr<FontPage> CreateNewPage();	
};
}
#pragma warning(pop)
#endif