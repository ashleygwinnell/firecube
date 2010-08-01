#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <queue>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <gl/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "utils.h"	
#include "Logger.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"	
#include "BoundingBox.h"
#include "Texture.h"		
#include "Buffer.h"
#include "Shaders.h"
#include "Geometry.h"	
#include "FrameBuffer.h"
#include "Font.h"
#include "ShaderGenerator.h"
#include "Renderer.h"				
#include "Application.h"

#include "privateFont.h"
using namespace FireCube;

FT_Library  freeTypeLibrary;

FontManager::FontManager()
{
	
}
boost::shared_ptr<FontPage> FontManager::CreateNewPage()
{
	boost::shared_ptr<FontPage> p(new FontPage);
	p->tex=Texture(new TextureResource);	
	p->tex->Create();
	glBindTexture(GL_TEXTURE_2D,p->tex->id);
	unsigned char empty[512*512];
	ZeroMemory(empty,512*512);
	glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,512,512,0,GL_ALPHA,GL_UNSIGNED_BYTE,empty);	
	p->textureSize=512;
	p->curPos=vec2(0,0);
	page.push_back(p);
	return p;
}
boost::shared_ptr<FontResource> FontManager::Create(const string &filename,int size)
{
	ostringstream oss;
	oss << filename << ":" << size;
	return ResourceManager<FontResource>::Create(oss.str());
}
FontResource::FontResource()
{	
	glyph.resize(256);
	fontImpl=new FontImpl;
}
FontResource::~FontResource()
{
	Logger::Write("Destroying font.\n");
	delete fontImpl;
}
bool FontResource::AddChar(char c)
{
	int error;
	FT_UInt glyph_index;
	glyph_index=FT_Get_Char_Index(fontImpl->face,c);
	error=FT_Load_Glyph(fontImpl->face,glyph_index,FT_LOAD_DEFAULT);
	if (error)
		return false;
	if (c==32)
	{
		glyph[c].advance=fontImpl->face->glyph->advance.x>>6;
		return true;
	}	
	error=FT_Render_Glyph(fontImpl->face->glyph,FT_RENDER_MODE_NORMAL);
	if (error)
		return false;
	if (page->textureSize-page->curPos.x<fontImpl->face->glyph->bitmap.width)
	{
		page->curPos.x=0;
		page->curPos.y+=size;
	}
	if (page->textureSize-page->curPos.y<fontImpl->face->glyph->bitmap.rows)
		return false;
	glTexSubImage2D(GL_TEXTURE_2D,0,(int)page->curPos.x,(int)page->curPos.y,fontImpl->face->glyph->bitmap.width,fontImpl->face->glyph->bitmap.rows,GL_ALPHA,GL_UNSIGNED_BYTE,fontImpl->face->glyph->bitmap.buffer);
	glyph[c].uv=page->curPos/512.0f;
	glyph[c].size=vec2((float)fontImpl->face->glyph->bitmap.width,(float)fontImpl->face->glyph->bitmap.rows);
	glyph[c].bitmapOffset=vec2((float)fontImpl->face->glyph->bitmap_left,size-(float)fontImpl->face->glyph->bitmap_top);	
	glyph[c].advance=fontImpl->face->glyph->advance.x>>6;
	page->curPos.x+=glyph[c].size.x;
	
	return true;
}
bool FontResource::Load(const string &name,int size)
{	
	Logger::Write("Loading font with name:");
	Logger::Write(name);
	Logger::Write("\n");
	int error=0;
	char text[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()-=_+[]{};:'\"\\|,./<>?/*. ";
	this->size=size;		
	error=FT_New_Face(freeTypeLibrary,name.c_str(),0,&(fontImpl->face));
	if (error)
		return false;
	error=FT_Set_Pixel_Sizes(fontImpl->face,0,size);
	if (error)
		return false;
	vector<boost::weak_ptr<FontPage>>::iterator p=Renderer::GetFontManager().page.begin();
	bool found=false;
	for (;p!=Renderer::GetFontManager().page.end();p++)
	{
		if (!p->expired())
		{
			boost::shared_ptr<FontPage> pg=p->lock();
			int availSpace=(pg->textureSize-((int)pg->curPos.y+size))*pg->textureSize+(pg->textureSize-((int)pg->curPos.x+size))*size;
			if (availSpace>=(int)strlen(text)*size*size)
			{
				found=true;
				break;
			}
		}
	}
	if (found)
		page=(*p).lock();
	else
	{
		page=Renderer::GetFontManager().CreateNewPage();
	}
	glBindTexture(GL_TEXTURE_2D,page->tex->id);	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	for (unsigned int i=0;i<strlen(text);i++)
	{		
		AddChar(text[i]);
	}	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	return true;
}
bool FontResource::Load(const string &name)
{
	int size=0;
	string::size_type d;
	d=name.find_last_of(":");
	if (d==string::npos)
		return false;
	string ssize=name.substr(d+1);
	string fontName=name.substr(0,d);	
	size=atoi(ssize.c_str());
	return Load(fontName,size);
}
