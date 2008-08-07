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
class FIRECUBE_API FontManager : public ResourceManager<FontResource>
{
	friend class FontResource;
	friend class Renderer;
public:
	FontManager();	
private:
	vector<FontPage> page;
	FontPage *CreateNewPage();
	FT_Library  library;
};
class FIRECUBE_API FontResource
{
	friend class Renderer;
public:
	FontResource();
	bool Load(const string &name);	
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