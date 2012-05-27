#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <queue>
using namespace std;
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Dependencies/glew.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Utils/ResourcePool.h"
#include "Math/MyMath.h"
#include "Rendering/Texture.h"
#include "Rendering/Font.h"
#include "Rendering/Renderer.h"

#include "Rendering/privateFont.h"
using namespace FireCube;

FT_Library  freeTypeLibrary;

FontPool::FontPool()
{

}

std::shared_ptr<FontPage> FontPool::CreateNewPage()
{
	// Create a new font page - a blank texture
	std::shared_ptr<FontPage> p(new FontPage);
	p->tex = TexturePtr(new Texture);
	p->tex->Create();
	glBindTexture(GL_TEXTURE_2D, p->tex->GetId());
	unsigned char empty[512 * 512];
	ZeroMemory(empty, 512 * 512);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, empty);
	p->textureSize = 512;
	p->curPos = vec2(0, 0);
	page.push_back(p);
	return p;
}

FontPtr FontPool::Create(const string &filename, int size)
{	
	ostringstream fullPathName;
	std::string loadfile = Filesystem::SearchForFileName(filename);
	if (loadfile.empty())
		return FontPtr();
		
	// Add the size of the font to the full path name.
	// Used to differentiate between the same font with different sizes when
	// searching it in the font pool to see if it is already loaded.
	fullPathName << Filesystem::GetFullPath(loadfile) << ":" << size;

	map<std::string, std::weak_ptr<Font>>::iterator i = pool.find(fullPathName.str());
	if (i != pool.end())
		if (!i->second.expired())
			return i->second.lock();
	FontPtr ret(new Font);
	if (ret->Load(loadfile, size))
	{
		pool[fullPathName.str()] = ret;
		return ret;
	}
	else
		return FontPtr();
}

Font::Font()
{
	glyph.resize(256);
	fontImpl = new FontImpl;
}

Font::~Font()
{
	Logger::Write(Logger::LOG_INFO, "Destroying font");
	delete fontImpl;
}

bool Font::AddChar(char c)
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
		page->curPos.y += size;
	}
	// If no room left return false
	if (page->textureSize - page->curPos.y < fontImpl->face->glyph->bitmap.rows)
		return false;
	// Update the font texture
	glTexSubImage2D(GL_TEXTURE_2D, 0, (int)page->curPos.x, (int)page->curPos.y, fontImpl->face->glyph->bitmap.width, fontImpl->face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, fontImpl->face->glyph->bitmap.buffer);
	// Store information about this glyph which is used when rendering it
	glyph[c].uv = page->curPos / 512.0f;
	glyph[c].size = vec2((float)fontImpl->face->glyph->bitmap.width, (float)fontImpl->face->glyph->bitmap.rows);
	glyph[c].bitmapOffset = vec2((float)fontImpl->face->glyph->bitmap_left, size - (float)fontImpl->face->glyph->bitmap_top);
	glyph[c].advance = fontImpl->face->glyph->advance.x >> 6;
	
	// Update the current write position
	page->curPos.x += glyph[c].size.x;

	return true;
}

bool Font::Load(const string &name, int size)
{
	Logger::Write(Logger::LOG_INFO, "Loading font with name:" + name);
	int error = 0;
	// This string specifies the list of glyphs that will be rendered
	char text[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()-=_+[]{};:'\"\\|,./<>?/. ";
	this->size = size;
	// Load the font face.
	error = FT_New_Face(freeTypeLibrary, name.c_str(), 0, &(fontImpl->face));
	if (error)
		return false;
	// Set the pixel size
	error = FT_Set_Pixel_Sizes(fontImpl->face, 0, size);
	if (error)
		return false;
	vector<std::weak_ptr<FontPage>>::iterator p = Renderer::GetFontPool().page.begin();
	bool found = false;
	// Iterate over all font pages and find one with enough space to hold this font
	for (; p != Renderer::GetFontPool().page.end(); p++)
	{
		if (!p->expired())
		{
			std::shared_ptr<FontPage> pg = p->lock();
			int availSpace = (pg->textureSize - ((int)pg->curPos.y + size)) * pg->textureSize + (pg->textureSize - ((int)pg->curPos.x + size)) * size;
			if (availSpace >= (int)strlen(text)*size * size)
			{
				found = true;
				break;
			}
		}
	}	
	if (found)
		page = (*p).lock();
	else
	{
		// If none found create a new one
		page = Renderer::GetFontPool().CreateNewPage();
	}

	glBindTexture(GL_TEXTURE_2D, page->tex->GetId());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned int i = 0; i < strlen(text); i++)
	{
		AddChar(text[i]);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	return true;
}

bool Font::Load(const string &name)
{
	return Load(name, 18);
}