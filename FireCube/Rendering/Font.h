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
class Texture;
class vec2;
class vec3;
class vec4;

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
	Texture *tex;
	vec2 curPos;
	int textureSize;
};
/** @endcond */

/**
* This class holds the data needed to draw text with a specific font and size.
*/
class FIRECUBE_API Font : public Resource
{	
	OBJECT(Font)
public:
	Font(Engine *engine);
	~Font();

	/**
	* Loads a font.
	* @param name The file to load.
	*/
	bool Load(const std::string &filename);

	FontFace *GenerateFontFace(int pointSize);
private:
	std::vector<char> data;
	std::map<int, FontFace *> faces;
};

class FIRECUBE_API FontFace
{
	friend class Font;
	friend class Renderer;
public:
	FontFace();
	~FontFace();
private:
	bool AddChar(char c);
	int pointSize;
	std::vector<Glyph> glyph;
	FontPage *page;
	FontImpl *fontImpl;
};

}
#pragma warning(pop)
