#include <sstream>
#include <fstream>
#include "ThirdParty/GLEW/glew.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "Rendering/Texture.h"
#include "Rendering/Font.h"
#include "Rendering/Renderer.h"
#include "Rendering/privateFont.h"
#include "Utils/Logger.h"

using namespace FireCube;

FT_Library  freeTypeLibrary;

Font::Font(Engine *engine) : Resource(engine)
{
	
}

Font::~Font()
{
	LOGINFO("Destroying font");	
	for (auto i : faces)
		delete i.second;
}

void Font::Init()
{
	FT_Init_FreeType(&freeTypeLibrary);
}

bool Font::Load(const std::string &filename)
{
	LOGINFO("Loading font with name:" + filename);
	std::ifstream file(filename, std::ios::binary);
	if (!file)
		return false;
	this->filename = filename;
	// read the data
	data = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return true;
	
}

FontFace *Font::GenerateFontFace(int pointSize)
{
	auto i = faces.find(pointSize);
	if (i != faces.end())
		return i->second;

	FontFace *fontFace = new FontFace();
	int error = 0;
	// This string specifies the list of glyphs that will be rendered
	char text[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()-=_+[]{};:'\"\\|,./<>?/. ";
	fontFace->pointSize = pointSize;
	// Load the font face.		
	error = FT_New_Memory_Face(freeTypeLibrary, (const FT_Byte *) &data[0], data.size(), 0, &(fontFace->fontImpl->face));
	if (error)
		return false;
	// Set the pixel size
	error = FT_Set_Pixel_Sizes(fontFace->fontImpl->face, 0, pointSize);
	if (error)
		return false;
	
	fontFace->page = new FontPage();		
	fontFace->page->tex = new Texture(engine);	
	fontFace->page->textureSize = 512;
	fontFace->page->curPos = vec2::ZERO;
	glBindTexture(GL_TEXTURE_2D, fontFace->page->tex->GetObjectId());
	std::vector<unsigned char> empty(fontFace->page->textureSize * fontFace->page->textureSize, 0);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontFace->page->textureSize, fontFace->page->textureSize, 0, GL_RED, GL_UNSIGNED_BYTE, &empty[0]);		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned int i = 0; i < strlen(text); i++)
	{
		fontFace->AddChar(text[i]);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);	

	FT_Long useKerning = FT_HAS_KERNING(fontFace->fontImpl->face);
	if (useKerning)
	{
		for (unsigned int i = 0; i < strlen(text); i++)
		{
			for (unsigned int j = 0; j < strlen(text); j++)
			{
				unsigned int a = text[i];
				unsigned int b = text[j];
				unsigned int key = a << 16 | b;
				FT_UInt aIndex = FT_Get_Char_Index(fontFace->fontImpl->face, a);
				FT_UInt bIndex = FT_Get_Char_Index(fontFace->fontImpl->face, b);
				// Retrieve kerning distance and move pen position
				if (aIndex && bIndex)
				{
					FT_Vector delta;
					FT_Get_Kerning(fontFace->fontImpl->face, aIndex, bIndex, FT_KERNING_DEFAULT, &delta);
					fontFace->kerning[key] = (short) (delta.x >> 6);
				}
			}
		}
	}

	faces[pointSize] = fontFace;
	return fontFace;
}

FontFace::FontFace()
{
	glyph.resize(256);
	fontImpl = new FontImpl;
}

FontFace::~FontFace()
{
	LOGINFO("Destroying font face");
	delete page->tex;
	delete page;
	delete fontImpl;
}

bool FontFace::AddChar(char c)
{
	int error;
	FT_UInt glyph_index;
	// Get the glyph index
	glyph_index = FT_Get_Char_Index(fontImpl->face, c);
	error = FT_Load_Glyph(fontImpl->face, glyph_index, FT_LOAD_DEFAULT);
	if (error)
		return false;
	// If it is "space" then no need to render the glyph
	if (c == 32)
	{
		glyph[c].advance = fontImpl->face->glyph->advance.x >> 6;
		return true;
	}
	// Render the glyph
	error = FT_Render_Glyph(fontImpl->face->glyph, FT_RENDER_MODE_NORMAL);
	if (error)
		return false;
	// If no room for the glyph in the current line, then start a new one
	if (page->textureSize - page->curPos.x < fontImpl->face->glyph->bitmap.width)
	{
		page->curPos.x = 0;
		page->curPos.y += pointSize;
	}
	// If no room left return false
	if (page->textureSize - page->curPos.y < fontImpl->face->glyph->bitmap.rows)
		return false;
	// Update the font texture
	glTexSubImage2D(GL_TEXTURE_2D, 0, (int)page->curPos.x, (int)page->curPos.y, fontImpl->face->glyph->bitmap.width, fontImpl->face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, fontImpl->face->glyph->bitmap.buffer);
	// Store information about this glyph which is used when rendering it
	glyph[c].uv = page->curPos / (float) page->textureSize;
	glyph[c].size = vec2((float)fontImpl->face->glyph->bitmap.width, (float)fontImpl->face->glyph->bitmap.rows);
	glyph[c].bitmapOffset = vec2((float)fontImpl->face->glyph->bitmap_left, pointSize - (float)fontImpl->face->glyph->bitmap_top);
	glyph[c].advance = fontImpl->face->glyph->advance.x >> 6;

	// Update the current write position
	page->curPos.x += glyph[c].size.x;

	return true;
}

short FontFace::GetKerning(unsigned int a, unsigned int b) const
{
	if (kerning.empty())
		return 0;

	unsigned int key = a << 16 | b;
	auto i = kerning.find(key);
	if (i == kerning.end())
		return 0;

	return i->second;
}