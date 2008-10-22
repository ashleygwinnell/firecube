#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "Terrain.h"

Terrain::Terrain() : indexBuffer(new BufferResource), vertexBuffer(new BufferResource), uvBuffer(new BufferResource),normalBuffer(new BufferResource),program(new ProgramResource)
{	
}
bool Terrain::GenerateTerrain(const string &heightmap,const string &texture,const string &detailMap,vec3 sizeVertices,vec2 sizeUv)
{	
	terrainScale=sizeVertices;
	program->Create(Renderer::GetShaderManager()->Create("1.vshader"),Renderer::GetShaderManager()->Create("1.fshader"));
	vertexBuffer->Create();
	indexBuffer->Create();
	uvBuffer->Create();
	terrainTexture=Renderer::GetTextureManager()->Create(texture);
	this->detailMap=Renderer::GetTextureManager()->Create(detailMap);
	if (!heightmapImage.Load(heightmap))
		return false;
	width=heightmapImage.GetWidth();
	height=heightmapImage.GetHeight();
	vector<vec3> vertex;
	vector<DWORD> index;
	vector<vec2> uv;
	vertex.resize(width*height);	
	uv.resize(width*height);
	for (DWORD y=0;y<height;y++)
	{
		for (DWORD x=0;x<width;x++)
		{
			vertex[y*width+x]=vec3((float)x/(float)(width-1)*sizeVertices.x,heightmapImage.GetPixel(x,y).x*sizeVertices.y,(float)y/(float)(height-1)*sizeVertices.z);
		}
	}
	vertexBuffer->LoadData(&vertex[0],vertex.size()*sizeof(vec3),STATIC);
	for (DWORD y=0;y<height;y++)
	{
		for (DWORD x=0;x<width;x++)
		{
			uv[y*width+x]=vec2((float)x/(float)(width-1)*sizeUv.x,(float)y/(float)(height-1)*sizeUv.y);
		}
	}
	uvBuffer->LoadData(&uv[0],uv.size()*sizeof(vec2),STATIC);
	index.resize((width-1)*(height-1)*2*3);	
	DWORD currentIndex=0;
	for (DWORD y=0;y<height-1;y++)
	{
		for (DWORD x=0;x<width-1;x++)
		{
			DWORD i0=y*width+x;
			DWORD i1=i0+1;
			DWORD i2=(y+1)*width+x;
			DWORD i3=i2+1;
			index[currentIndex++]=i0;
			index[currentIndex++]=i2;
			index[currentIndex++]=i1;

			index[currentIndex++]=i1;
			index[currentIndex++]=i2;
			index[currentIndex++]=i3;
		}
	}	
	indexBuffer->LoadIndexData(&index[0],index.size(),STATIC);	
	return true;
}
bool Terrain::GenerateTerrain(const string &heightmap,vec3 sizeVertices,vec2 sizeUv)
{
	terrainScale=sizeVertices;
	program->Create(Renderer::GetShaderManager()->Create("2.vshader"),Renderer::GetShaderManager()->Create("2.fshader"));
	normalBuffer=Buffer(new BufferResource);
	normalBuffer->Create();
	vertexBuffer->Create();
	indexBuffer->Create();
	uvBuffer->Create();	
	if (!heightmapImage.Load(heightmap))
		return false;
	width=heightmapImage.GetWidth();
	height=heightmapImage.GetHeight();
	vector<vec3> vertex;
	vector<vec3> normal;
	vector<DWORD> index;
	vector<vec2> uv;
	DWORD currentIndex=0;
	DWORD normalCurrentIndex=0;
	DWORD uvCurrentIndex=0;
	vertex.resize((width-1)*(height-1)*2*3);
	normal.resize((width-1)*(height-1)*2*3);
	uv.resize((width-1)*(height-1)*2*3);
	for (DWORD y=0;y<height-1;y++)
	{
		for (DWORD x=0;x<width-1;x++)
		{
			vertex[currentIndex++]=vec3((float)x/(float)(width-1)*sizeVertices.x,heightmapImage.GetPixel(x,y).x*sizeVertices.y,(float)y/(float)(height-1)*sizeVertices.z);
			vertex[currentIndex++]=vec3((float)x/(float)(width-1)*sizeVertices.x,heightmapImage.GetPixel(x,y+1).x*sizeVertices.y,(float)(y+1)/(float)(height-1)*sizeVertices.z);
			vertex[currentIndex++]=vec3((float)(x+1)/(float)(width-1)*sizeVertices.x,heightmapImage.GetPixel(x+1,y).x*sizeVertices.y,(float)y/(float)(height-1)*sizeVertices.z);

			vertex[currentIndex++]=vec3((float)(x+1)/(float)(width-1)*sizeVertices.x,heightmapImage.GetPixel(x+1,y).x*sizeVertices.y,(float)y/(float)(height-1)*sizeVertices.z);
			vertex[currentIndex++]=vec3((float)x/(float)(width-1)*sizeVertices.x,heightmapImage.GetPixel(x,y+1).x*sizeVertices.y,(float)(y+1)/(float)(height-1)*sizeVertices.z);
			vertex[currentIndex++]=vec3((float)(x+1)/(float)(width-1)*sizeVertices.x,heightmapImage.GetPixel(x+1,y+1).x*sizeVertices.y,(float)(y+1)/(float)(height-1)*sizeVertices.z);

			vec3 n1=-Cross(vertex[normalCurrentIndex+2]-vertex[normalCurrentIndex],vertex[normalCurrentIndex+1]-vertex[normalCurrentIndex]).Normalize();
			normal[normalCurrentIndex++]=n1;
			normal[normalCurrentIndex++]=n1;
			normal[normalCurrentIndex++]=n1;

			vec3 n2=-Cross(vertex[normalCurrentIndex+2]-vertex[normalCurrentIndex],vertex[normalCurrentIndex+1]-vertex[normalCurrentIndex]).Normalize();
			normal[normalCurrentIndex++]=n2;
			normal[normalCurrentIndex++]=n2;
			normal[normalCurrentIndex++]=n2;

			uv[uvCurrentIndex++]=vec2((float)x/(float)(width-1)*sizeUv.x,(float)y/(float)(height-1)*sizeUv.y);
			uv[uvCurrentIndex++]=vec2((float)x/(float)(width-1)*sizeUv.x,(float)(y+1)/(float)(height-1)*sizeUv.y);
			uv[uvCurrentIndex++]=vec2((float)(x+1)/(float)(width-1)*sizeUv.x,(float)y/(float)(height-1)*sizeUv.y);

			uv[uvCurrentIndex++]=vec2((float)(x+1)/(float)(width-1)*sizeUv.x,(float)y/(float)(height-1)*sizeUv.y);
			uv[uvCurrentIndex++]=vec2((float)x/(float)(width-1)*sizeUv.x,(float)(y+1)/(float)(height-1)*sizeUv.y);
			uv[uvCurrentIndex++]=vec2((float)(x+1)/(float)(width-1)*sizeUv.x,(float)(y+1)/(float)(height-1)*sizeUv.y);
		}
	}
	vertexBuffer->LoadData(&vertex[0],vertex.size()*sizeof(vec3),STATIC);		
	normalBuffer->LoadData(&normal[0],normal.size()*sizeof(vec3),STATIC);		
	uvBuffer->LoadData(&uv[0],uv.size()*sizeof(vec2),STATIC);

	index.resize((width-1)*(height-1)*2*3);	
	currentIndex=0;
	for (DWORD i=0;i<(width-1)*(height-1)*2*3;i++)
	{
		index[i]=i;
	}	
	indexBuffer->LoadIndexData(&index[0],index.size(),STATIC);	
	return true;
}
void Terrain::PrepareRender()
{
	vertexBuffer->SetVertexStream(3);
	uvBuffer->SetTexCoordStream(0);	
	if (normalBuffer->IsValid())
		normalBuffer->SetNormalStream();
	Renderer::UseProgram(program);
	program->SetUniform("terrainMap",0);
	program->SetUniform("detailMap",1);	
	program->SetUniform("fogDensity",0.01f);
	program->SetUniform("fogColor",vec4(0.30f,0.42f,0.95f,1.0f));
	program->SetUniform("lightDir",vec3(1,-1,1));
	Renderer::UseTexture(terrainTexture,0);	
	Renderer::UseTexture(detailMap,1);		
}
void Terrain::Render()
{
	vertexBuffer->SetVertexStream(3);
	uvBuffer->SetTexCoordStream(0);
	indexBuffer->SetIndexStream();
	if (normalBuffer->IsValid())
		normalBuffer->SetNormalStream();
	Renderer::UseProgram(program);
	program->SetUniform("terrainMap",0);
	program->SetUniform("detailMap",1);	
	program->SetUniform("fogDensity",0.007f);
	program->SetUniform("fogColor",vec4(0.30f,0.42f,0.95f,1.0f));
	program->SetUniform("lightDir",vec3(1,-1,1));
	Renderer::UseTexture(terrainTexture,0);	
	Renderer::UseTexture(detailMap,1);	
	Renderer::RenderIndexStream(TRIANGLES,(width-1)*(height-1)*2*3);
}
float Terrain::GetHeight(float x,float y)
{
	if ((x<0) || (x>=terrainScale.x) || (y<0) || (y>=terrainScale.z))
		return 0;
	x=x/terrainScale.x*(float)(width-1);
	y=y/terrainScale.z*(float)(height-1);
	int ix=(int)x,iy=(int)y;
	float fx=x-(float)ix,fy=y-(float)iy;	
	float i0,i1,i2;
	float z0,z1;
	z0=fx*fx+fy*fy;
	z1=(1.0f-fx)*(1.0f-fx)+(1.0f-fy)*(1.0f-fy);
	if (z0<z1)
	{
		i0=heightmapImage.GetPixel(ix,iy).x;
		i1=heightmapImage.GetPixel(ix+1,iy).x;
		i2=heightmapImage.GetPixel(ix,iy+1).x;
		float d0,d1;
		d0=i1-i0;
		d1=i2-i0;
		return (i0+d0*fx+d1*fy)*terrainScale.y;
	}
	i0=heightmapImage.GetPixel(ix+1,iy).x;
	i2=heightmapImage.GetPixel(ix,iy+1).x;
	i1=heightmapImage.GetPixel(ix+1,iy+1).x;
	float d0,d1;
	d0=i2-i1;
	d1=i0-i1;
	return (i1+d0*(1.0f-fx)+d1*(1.0f-fy))*terrainScale.y;
}
int Terrain::GetWidth()
{
	return width;
}
int Terrain::GetHeight()
{
	return height;
}