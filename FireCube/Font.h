#ifndef FONT_H
#define FONT_H

#pragma warning(push)
#pragma warning(disable:4251)

class FontResource;

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
class FIRECUBE_API FontManager : public ResourceManager<FontResource>
{
public:
	FontManager();	
	vector<FontPage> page;
	FontPage *CreateNewPage();
	FT_Library  library;
};
class FIRECUBE_API FontResource
{
public:
	FontResource();
	bool Load(const string &name);	
	bool AddChar(char c);
	vector<Glyph> glyph;
	FontPage *page;
	FT_Face face;
	int size;
};

typedef boost::shared_ptr<FontResource> Font;

#pragma warning(pop)
#endif