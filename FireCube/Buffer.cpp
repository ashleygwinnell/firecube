#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

BufferResource::BufferResource() : id(0)
{

}
BufferResource::~BufferResource()
{
	Destroy();
}

void BufferResource::Create()
{		
	glGenBuffers(1,&id);
	ostringstream ss;
	ss<< "Created buffer with id="<<id<<endl;
	Logger::Write(ss.str());
}
bool BufferResource::LoadIndexData(void *data,DWORD count,BufferType bt)
{
	GLenum e;
	if (bt==STREAM)
		e=GL_STREAM_DRAW;
	else if (bt==DYNAMIC)
		e=GL_DYNAMIC_DRAW;
	else if (bt==STATIC)
		e=GL_STATIC_DRAW;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(DWORD),data,e);
	if (glGetError()==GL_OUT_OF_MEMORY)
		return false;
	return true;
}
bool BufferResource::LoadData(void *data,DWORD size,BufferType bt)
{
	GLenum e;
	if (bt==STREAM)
		e=GL_STREAM_DRAW;
	else if (bt==DYNAMIC)
		e=GL_DYNAMIC_DRAW;
	else if (bt==STATIC)
		e=GL_STATIC_DRAW;

	glBindBuffer(GL_ARRAY_BUFFER,id);
	glBufferData(GL_ARRAY_BUFFER,size,data,e);
	if (glGetError()==GL_OUT_OF_MEMORY)
		return false;
	return true;
}
void BufferResource::SetVertexStream(int numCoords)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glVertexPointer(numCoords,GL_FLOAT,0,0);
}
void BufferResource::SetNormalStream()
{
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glNormalPointer(GL_FLOAT,0,0);	
}
void BufferResource::SetTexCoordStream(unsigned int unit)
{
	glClientActiveTexture(GL_TEXTURE0+unit);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glTexCoordPointer(2,GL_FLOAT,0,0);
}

void BufferResource::SetColorStream()
{
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glColorPointer(3,GL_FLOAT,0,0);
}
void BufferResource::SetIndexStream()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);	
}
bool BufferResource::IsValid()
{
	return id!=0;
}
void BufferResource::Destroy()
{
	ostringstream ss;
	ss<< "Destroyed buffer with id="<<id<<endl;
	Logger::Write(ss.str());
	glDeleteBuffers(1,&id);
	id=0;
}