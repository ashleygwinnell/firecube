#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "GLee.h"

#include "utils.h"	
#include "Logger.h"
#include "Buffer.h"

using namespace FireCube;

BufferResource::BufferResource() : id(0)
{

}
BufferResource::~BufferResource()
{
	ostringstream ss;
	ss<< "Destroyed buffer with id="<<id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	glDeleteBuffers(1,&id);
	id=0;
}

void Buffer::Create()
{		
	resource=boost::shared_ptr<BufferResource>(new BufferResource);
	glGenBuffers(1,&resource->id);
	ostringstream ss;
	ss<< "Created buffer with id="<<resource->id;
	Logger::Write(Logger::LOG_INFO, ss.str());
}
bool Buffer::LoadIndexData(void *data,unsigned int count,BufferType bt)
{
	GLenum e;
	if (bt==STREAM)
		e=GL_STREAM_DRAW;
	else if (bt==DYNAMIC)
		e=GL_DYNAMIC_DRAW;
	else if (bt==STATIC)
		e=GL_STATIC_DRAW;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,resource->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(unsigned int),data,e);
	if (glGetError()==GL_OUT_OF_MEMORY)
		return false;
	return true;
}
bool Buffer::LoadData(void *data,unsigned int size,BufferType bt)
{
	GLenum e;
	if (bt==STREAM)
		e=GL_STREAM_DRAW;
	else if (bt==DYNAMIC)
		e=GL_DYNAMIC_DRAW;
	else if (bt==STATIC)
		e=GL_STATIC_DRAW;

	glBindBuffer(GL_ARRAY_BUFFER,resource->id);
	glBufferData(GL_ARRAY_BUFFER,size,data,e);
	if (glGetError()==GL_OUT_OF_MEMORY)
		return false;
	return true;
}
void Buffer::SetVertexStream(int numCoords)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,resource->id);
	glVertexPointer(numCoords,GL_FLOAT,0,0);
}
void Buffer::SetNormalStream()
{
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,resource->id);
	glNormalPointer(GL_FLOAT,0,0);	
}
void Buffer::SetTexCoordStream(unsigned int unit)
{
	glClientActiveTexture(GL_TEXTURE0+unit);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	glBindBuffer(GL_ARRAY_BUFFER,resource->id);
	glTexCoordPointer(2,GL_FLOAT,0,0);
}

void Buffer::SetColorStream()
{
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,resource->id);
	glColorPointer(3,GL_FLOAT,0,0);
}
void Buffer::SetIndexStream()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,resource->id);	
}
void Buffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER,resource->id);
}
bool Buffer::IsValid()
{
	return resource->id!=0;
}
void Buffer::Destroy()
{
	resource.reset();
}
unsigned int Buffer::GetId() const
{
	return resource->id;
}
Buffer::operator bool () const
{
	return resource;
}
bool Buffer::operator== (const Buffer &buffer) const
{
	return resource==buffer.resource;
}
