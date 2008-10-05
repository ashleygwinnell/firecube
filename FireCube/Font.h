#ifndef FONT_H
#define FONT_H

#pragma warning(push)
#pragma warning(disable:4251)

class FontResource;
typedef boost::shared_ptr<FontResource> Font;
/* CPPDOC_BEGIN_EXCLUDE */
namespace Renderer
{
	void FIRECUBE_API RenderText(Font font,vec2 pos,vec4 color,const string &str);
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
/* CPPDOC_END_EXCLUDE */
/**
* Manages the various fonts.
*/
class FIRECUBE_API FontManager : public ResourceManager<FontResource>
{
	friend class FontResource;
	friend void Renderer::RenderText(Font font,vec2 pos,vec4 color,const string &str);
public:
	FontManager();
	/**
	* Creates and loads a font from the specified file.
	* @param filename The file to load.
	* @param size The size of the font.
	*/
	boost::shared_ptr<FontResource> Create(const string &filename,int size);
private:
	vector<boost::weak_ptr<FontPage>> page;
	boost::shared_ptr<FontPage> CreateNewPage();	
};
/**
* Holds the data for a single font face.
*/
class FIRECUBE_API FontResource
{
	friend void Renderer::RenderText(Font font,vec2 pos,vec4 color,const string &str);
public:
	FontResource();
	~FontResource();
	/**
	* Loads a font.
	* @param name The file to load separated with : and an integer representing the font size.
	*/
	bool Load(const string &name);	
	/**
	* Loads a font.
	* @param name The file to load.
	* @param size The size of the font.
	*/
	bool Load(const string &name,int size);	
private:
	bool AddChar(char c);
	vector<Glyph> glyph;
	boost::shared_ptr<FontPage> page;
	FontImpl *fontImpl;
	int size;
};

#pragma warning(pop)
#endif