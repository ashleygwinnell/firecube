#include <string>
#include <vector>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

Renderer::Renderer()
{
}
Renderer::~Renderer()
{
}
void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::SetModelViewMatrix(mat4 &m)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m.m);
}
void Renderer::SetProjectionMatrix(mat4 &m)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m.m);
}
void Renderer::Render(Model &model)
{
	vector<Mesh>::iterator i=model.mesh.begin();
	
	model.vertexBuffer.SetVertexStream();
	model.uvBuffer.SetTexCoordStream(0);
	for (;i!=model.mesh.end();i++)
	{
		if (i->material->tex)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,i->material->tex);
		}
		else
			glDisable(GL_TEXTURE_2D);

		i->indexBuffer.RenderIndexStream(TRIANGLES,i->face.size()*3);
	}
}
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
void Buffer::SetVertexStream()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glVertexPointer(3,GL_FLOAT,0,0);
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
void Buffer::RenderIndexStream(RenderMode mode,DWORD count)
{
	GLenum glmode;
	switch (mode)
	{
	case POINTS:
		glmode=GL_POINTS;
		break;
	case LINES:
		glmode=GL_LINES;
		break;
	case TRIANGLES:
		glmode=GL_TRIANGLES;
		break;
	case TRIANGLE_STRIP:
		glmode=GL_TRIANGLE_STRIP;
		break;
	case QUADS:
		glmode=GL_QUADS;
		break;
	case LINE_LOOP:
		glmode=GL_LINE_LOOP;
		break;
	case TRIANGLE_FAN:
		glmode=GL_TRIANGLE_FAN;
		break;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer);
	glDrawElements(glmode,count,GL_UNSIGNED_INT,0);
}
void Buffer::Destroy()
{
	glDeleteBuffers(1,&buffer);
	buffer=NULL;
}