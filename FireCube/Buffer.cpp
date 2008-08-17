#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

Buffer::Buffer() : buffer(0)
{

}
Buffer::~Buffer()
{
	Destroy();
}

void Buffer::Create()
{	
	glGenBuffers(1,&buffer);	
}
void Buffer::LoadIndexData(void *data,DWORD count,BufferType bt)
{
	GLenum e;
	if (bt==STREAM)
		e=GL_STREAM_DRAW;
	else if (bt==DYNAMIC)
		e=GL_DYNAMIC_DRAW;
	else if (bt==STATIC)
		e=GL_STATIC_DRAW;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(DWORD),data,e);
}
void Buffer::LoadData(void *data,DWORD size,BufferType bt)
{
	GLenum e;
	if (bt==STREAM)
		e=GL_STREAM_DRAW;
	else if (bt==DYNAMIC)
		e=GL_DYNAMIC_DRAW;
	else if (bt==STATIC)
		e=GL_STATIC_DRAW;

	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glBufferData(GL_ARRAY_BUFFER,size,data,e);
}
void Buffer::SetVertexStream(int numCoords)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glVertexPointer(numCoords,GL_FLOAT,0,0);
}
void Buffer::SetNormalStream()
{
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glNormalPointer(GL_FLOAT,0,0);	
}
void Buffer::SetTexCoordStream(unsigned int unit)
{
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB+unit);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glTexCoordPointer(2,GL_FLOAT,0,0);
}

void Buffer::SetColorStream()
{
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glColorPointer(3,GL_FLOAT,0,0);
}
void Buffer::SetIndexStream()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer);	
}
void Buffer::Destroy()
{
	glDeleteBuffers(1,&buffer);
	buffer=NULL;
}