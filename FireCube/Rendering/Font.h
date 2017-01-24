#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <vector>
#include <map>
#include "Math/Math.h"
#include "Core/Resource.h"

namespace FireCube
{

//Forward declarations.
class Font;
class Texture2D;
class vec2;
class vec3;
class vec4;
class UIText;
class FontImpl;
class FontFace;

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
	Texture2D *tex;
	vec2 curPos;
	int textureSize;
};
/** @endcond */

/**
* This class holds the data needed to draw text with a specific font.
*/
class FIRECUBE_API Font : public Resource
{	
	FIRECUBE_OBJECT(Font)
public:
	Font(Engine *engine);
	~Font();

	/**
	* Loads a font.
	* @param name The file to load.
	*/
	bool Load(const std::string &filename);

	/**
	* Generates a font face using an actual size
	* @param pointSize The size of the font
	*/
	FontFace *GenerateFontFace(int pointSize);

	static void Init();
private:
	std::vector<char> data;
	std::map<int, FontFace *> faces;
};

/**
* This class holds the data needed to draw text with a specific font and size.
*/
class FIRECUBE_API FontFace
{
	friend class Font;
	friend class Renderer;
	friend class UIText;
public:
	FontFace();
	~FontFace();

	/**
	* @returns the kerning between two characters in pixels
	*/
	short GetKerning(unsigned int a, unsigned int b) const;

private:
	bool AddChar(char c);
	int pointSize;
	std::vector<Glyph> glyph;
	FontPage *page;
	FontImpl *fontImpl;
	std::map<unsigned int, short> kerning;
};

}
#pragma warning(pop)
