#ifndef FONT_H
#define FONT_H

#pragma warning(push)
#pragma warning(disable:4251)

class FontResource;
/* CPPDOC_BEGIN_EXCLUDE */
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
	friend class Renderer;
public:
	FontManager();	
private:
	vector<FontPage> page;
	FontPage *CreateNewPage();	
};
/**
* Holds the data for a single font face.
*/
class FIRECUBE_API FontResource
{
	friend class Renderer;
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
	FontPage *page;
	FT_Face face;
	int size;
};

typedef boost::shared_ptr<FontResource> Font;

#pragma warning(pop)
#endif