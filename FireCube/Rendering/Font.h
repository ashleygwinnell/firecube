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
typedef std::shared_ptr<Texture> TexturePtr;
class vec2;
class vec3;
class vec4;

/**
* A shared pointer to a Font.
*/
typedef std::shared_ptr<Font> FontPtr;

class FontImpl;

class FontFace;
typedef std::shared_ptr<FontFace> FontFacePtr;

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
class FIRECUBE_API Font : public Resource
{	
public:
	Font(Engine *engine);
	~Font();

	/**
	* Loads a font.
	* @param name The file to load.
	*/
	bool Load(const std::string &filename);

	FontFacePtr GenerateFontFace(int pointSize);
private:
	std::vector<char> data;
	std::map<int, FontFacePtr> faces;
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
	std::shared_ptr<FontPage> page;
	FontImpl *fontImpl;
};

}
#pragma warning(pop)
