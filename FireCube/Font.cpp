#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gl/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <FireCube.h>
using namespace FireCube;

FT_Library  freeTypeLibrary;
FontManager::FontManager()
{
	
}
FontPage *FontManager::CreateNewPage()
{
	FontPage p;
	p.tex=Texture(new TextureResource);	
	glGenTextures(1,&p.tex->id);
	glBindTexture(GL_TEXTURE_2D,p.tex->id);
	unsigned char empty[512*512];
	ZeroMemory(empty,512*512);
	glTexImage2D(GL_TEXTURE_2D,0,1,512,512,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,empty);	
	p.textureSize=512;
	p.curPos=vec2(0,0);
	page.push_back(p);
	return &page[page.size()-1];
}
FontResource::FontResource()
{	
	glyph.resize(256);
}
FontResource::~FontResource()
{

}
bool FontResource::AddChar(char c)
{
	int error;
	FT_UInt glyph_index;
	glyph_index=FT_Get_Char_Index(face,c);
	error=FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT);
	if (error)
		return false;
	if (c==32)
	{
		glyph[c].advance=face->glyph->advance.x>>6;
		return true;
	}	
	error=FT_Render_Glyph(face->glyph,FT_RENDER_MODE_NORMAL);
	if (error)
		return false;
	if (page->textureSize-page->curPos.x<face->glyph->bitmap.width)
	{
		page->curPos.x=0;
		page->curPos.y+=size;
	}
	if (page->textureSize-page->curPos.y<face->glyph->bitmap.rows)
		return false;
	glTexSubImage2D(GL_TEXTURE_2D,0,(int)page->curPos.x,(int)page->curPos.y,face->glyph->bitmap.width,face->glyph->bitmap.rows,GL_LUMINANCE,GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);
	glyph[c].uv=page->curPos/512.0f;
	glyph[c].size=vec2((float)face->glyph->bitmap.width,(float)face->glyph->bitmap.rows);
	glyph[c].bitmapOffset=vec2((float)face->glyph->bitmap_left,size-(float)face->glyph->bitmap_top);
	glyph[c].advance=face->glyph->advance.x>>6;
	page->curPos.x+=face->glyph->advance.x>>6;		
	
	return true;
}
bool FontResource::Load(const string &name,int size)
{	
	int error=0;
	char text[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()-=_+[]{};:'\"\\|,./<>?/*. ";
	this->size=size;		
	error=FT_New_Face(freeTypeLibrary,name.c_str(),0,&face);	
	if (error)
		return false;
	error=FT_Set_Pixel_Sizes(face,0,size);
	if (error)
		return false;
	vector<FontPage>::iterator p=Application::GetContext().fontManager->page.begin();
	bool found=false;
	for (;p!=Application::GetContext().fontManager->page.end();p++)
	{
		int availSpace=(p->textureSize-((int)p->curPos.y+size))*p->textureSize+(p->textureSize-((int)p->curPos.x+size))*size;
		if (availSpace>=(int)strlen(text)*size*size)
		{
			found=true;
			break;
		}
	}
	if (found)
		page=&(*p);
	else
	{
		page=Application::GetContext().fontManager->CreateNewPage();
	}
	glBindTexture(GL_TEXTURE_2D,page->tex->id);	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	for (unsigned int i=0;i<strlen(text);i++)
	{		
		AddChar(text[i]);
	}	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
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
