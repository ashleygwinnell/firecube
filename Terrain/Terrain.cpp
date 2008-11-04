#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "Frustum.h"
#include "QuadTree.h"
#include "Terrain.h"

Terrain::Terrain() : vertexBuffer(new BufferResource), uvBuffer(new BufferResource),normalBuffer(new BufferResource),material(new MaterialResource)
{
	material->ambient=vec4(0.2f,0.2f,0.2f,1.0f);
	material->diffuse=vec4(0.5f,0.5f,0.5f,1.0f);	
}
bool Terrain::GenerateTerrain(const string &heightmap,const string &diffuse,vec3 sizeVertices,vec2 sizeUv)
{	
	Timer timer;
	timer.Init();
	float t;

	timer.Update();
	diffuseTexture=Renderer::GetTextureManager()->Create(diffuse);
	diffuseTexture->SetFiltering(NEAREST,NEAREST);
	terrainScale=sizeVertices;
	material->program=Program(new ProgramResource);
	material->program->Create(Renderer::GetShaderManager()->Create("terrain.vshader"),Renderer::GetShaderManager()->Create("terrain.fshader"));	
	normalBuffer=Buffer(new BufferResource);
	normalBuffer->Create();
	vertexBuffer->Create();	
	uvBuffer->Create();	
	if (!heightmapImage.Load(heightmap))
		return false;
	width=heightmapImage.GetWidth();
	height=heightmapImage.GetHeight();
	vector<float> vertex((width-1)*(height-1)*2*3*3);
	vector<float> normal((width-1)*(height-1)*2*3*3);		
	vector<float> uv((width-1)*(height-1)*2*3*2);
	DWORD currentIndex=0;
	DWORD normalCurrentIndex=0;
	DWORD uvCurrentIndex=0;	
	vec2 verticesDiff(1.0f/(float)(width-1)*sizeVertices.x,1.0f/(float)(height-1)*sizeVertices.z);
	vec2 uvDiff(1.0f/(float)(width-1)*sizeUv.x,1.0f/(float)(height-1)*sizeUv.y);	
	for (DWORD y=0;y<height-1;y++)
	{
		for (DWORD x=0;x<width-1;x++)
		{			
			vertex[currentIndex++]=(float)x*verticesDiff.x;
			vertex[currentIndex++]=heightmapImage.GetPixel(x,y).x*sizeVertices.y;
			vertex[currentIndex++]=(float)y*verticesDiff.y;
			vec3 v0(vertex[currentIndex-3],vertex[currentIndex-2],vertex[currentIndex-1]);

			vertex[currentIndex++]=(float)x*verticesDiff.x;
			vertex[currentIndex++]=heightmapImage.GetPixel(x,y+1).x*sizeVertices.y;
			vertex[currentIndex++]=(float)(y+1)*verticesDiff.y;
			vec3 v1(vertex[currentIndex-3],vertex[currentIndex-2],vertex[currentIndex-1]);

			vertex[currentIndex++]=(float)(x+1)*verticesDiff.x;
			vertex[currentIndex++]=heightmapImage.GetPixel(x+1,y).x*sizeVertices.y;
			vertex[currentIndex++]=(float)y*verticesDiff.y;
			vec3 v2(vertex[currentIndex-3],vertex[currentIndex-2],vertex[currentIndex-1]);
			
			vertex[currentIndex++]=vertex[currentIndex-4];
			vertex[currentIndex++]=vertex[currentIndex-4];
			vertex[currentIndex++]=vertex[currentIndex-4];			
		
			vertex[currentIndex++]=vertex[currentIndex-10];
			vertex[currentIndex++]=vertex[currentIndex-10];
			vertex[currentIndex++]=vertex[currentIndex-10];			
			
			vertex[currentIndex++]=(float)(x+1)*verticesDiff.x;
			vertex[currentIndex++]=heightmapImage.GetPixel(x+1,y+1).x*sizeVertices.y;
			vertex[currentIndex++]=(float)(y+1)*verticesDiff.y;
			vec3 v3(vertex[currentIndex-3],vertex[currentIndex-2],vertex[currentIndex-1]);

			vec3 n1=-Cross(v2-v0,v1-v0).Normalize();
			normal[normalCurrentIndex++]=n1.x;
			normal[normalCurrentIndex++]=n1.y;
			normal[normalCurrentIndex++]=n1.z;

			normal[normalCurrentIndex++]=n1.x;
			normal[normalCurrentIndex++]=n1.y;
			normal[normalCurrentIndex++]=n1.z;

			normal[normalCurrentIndex++]=n1.x;
			normal[normalCurrentIndex++]=n1.y;
			normal[normalCurrentIndex++]=n1.z;

			vec3 n2=-Cross(v3-v2,v1-v2).Normalize();
			normal[normalCurrentIndex++]=n2.x;
			normal[normalCurrentIndex++]=n2.y;
			normal[normalCurrentIndex++]=n2.z;

			normal[normalCurrentIndex++]=n2.x;
			normal[normalCurrentIndex++]=n2.y;
			normal[normalCurrentIndex++]=n2.z;

			normal[normalCurrentIndex++]=n2.x;
			normal[normalCurrentIndex++]=n2.y;
			normal[normalCurrentIndex++]=n2.z;

			vec2 uvPos((float)x*uvDiff.x+uvDiff.x/2.0f,(float)y*uvDiff.y+uvDiff.y/2.0f);
			uv[uvCurrentIndex++]=uvPos.x;
			uv[uvCurrentIndex++]=uvPos.y;
			
			uv[uvCurrentIndex++]=uvPos.x;
			uv[uvCurrentIndex++]=uvPos.y;

			uv[uvCurrentIndex++]=uvPos.x;
			uv[uvCurrentIndex++]=uvPos.y;

			uv[uvCurrentIndex++]=uvPos.x;
			uv[uvCurrentIndex++]=uvPos.y;

			uv[uvCurrentIndex++]=uvPos.x;
			uv[uvCurrentIndex++]=uvPos.y;

			uv[uvCurrentIndex++]=uvPos.x;
			uv[uvCurrentIndex++]=uvPos.y;
		}
	}
	vertexBuffer->LoadData(&vertex[0],vertex.size()*sizeof(float),STATIC);		
	normalBuffer->LoadData(&normal[0],normal.size()*sizeof(float),STATIC);		
	uvBuffer->LoadData(&uv[0],uv.size()*sizeof(float),STATIC);
	t=(float)timer.Passed();
	cout << "Generating terrain took " << t << " seconds" << endl;
	timer.Update();

	quadtree.Initialize();
	string::size_type d;
	d=heightmap.find_last_of(".");	
	if (d!=string::npos)
	{
		string quadfile=heightmap.substr(0,d+1);
		quadfile.append("bin");
		ifstream f(quadfile.c_str(),ios::binary);
		if (f.is_open())
		{
			f.close();
			quadtree.Load(quadfile);
		}
		else
		{
			quadtree.Init(vec2((float)GetWidth(),(float)GetHeight()),vec2(sizeVertices.x,sizeVertices.z));
			quadtree.Build(sizeVertices.x/32.0f,1);
			quadtree.Save(quadfile);
		}
	}
	else
		return false;
	
	t=(float)timer.Passed();
	cout << "Generating/Loading quadtree took " << t << " seconds" << endl;

	return true;
}
DWORD Terrain::Render(Frustum &frustum)
{
	Renderer::UseMaterial(material);
	vertexBuffer->SetVertexStream(3);
	uvBuffer->SetTexCoordStream(0);	
	normalBuffer->SetNormalStream();	
	material->program->SetUniform("fogDensity",0.01f);
	material->program->SetUniform("fogColor",vec4(0.30f,0.42f,0.95f,1.0f));
	mat4 m=Renderer::GetModelViewMatrix();
	material->program->SetUniform("lightDir",m*vec3(1,-1,1));
	material->program->SetUniform("tex",0);
	Renderer::UseTexture(diffuseTexture,0);	

	return quadtree.Render(frustum);
}
float Terrain::GetHeight(vec2 pos)
{
	if ((pos.x<0) || (pos.x>=terrainScale.x) || (pos.y<0) || (pos.y>=terrainScale.z))
		return 0;
	pos.x=pos.x/terrainScale.x*(float)(width-1);
	pos.y=pos.y/terrainScale.z*(float)(height-1);
	int ix=(int)pos.x,iy=(int)pos.y;
	float fx=pos.x-(float)ix,fy=pos.y-(float)iy;	
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
vec3 Terrain::GetNormal(vec2 pos)
{
	if ((pos.x<0) || (pos.x>=terrainScale.x) || (pos.y<0) || (pos.y>=terrainScale.z))
		return vec3(0,1,0);
	
	pos.x=pos.x/terrainScale.x*(float)(width-1);
	pos.y=pos.y/terrainScale.z*(float)(height-1);
	int ix=(int)pos.x,iy=(int)pos.y;
	float fx=pos.x-(float)ix,fy=pos.y-(float)iy;	
	float i0,i1,i2;
	float z0,z1;
	z0=fx*fx+fy*fy;
	z1=(1.0f-fx)*(1.0f-fx)+(1.0f-fy)*(1.0f-fy);
	if (z0<z1)
	{
		i0=heightmapImage.GetPixel(ix,iy).x;
		i1=heightmapImage.GetPixel(ix+1,iy).x;
		i2=heightmapImage.GetPixel(ix,iy+1).x;
		vec3 v0(0,i0*terrainScale.y,0);
		vec3 v1(1.0f/(float)(width-1)*terrainScale.x,i1*terrainScale.y,0);
		vec3 v2(1.0f/(float)(width-1)*terrainScale.x,i2*terrainScale.y,1.0f/(float)(height-1)*terrainScale.z);
		return Cross(v2-v0,v1-v0).Normalize();
	}
	i0=heightmapImage.GetPixel(ix+1,iy).x;
	i2=heightmapImage.GetPixel(ix,iy+1).x;
	i1=heightmapImage.GetPixel(ix+1,iy+1).x;
	vec3 v0(1.0f/(float)(width-1)*terrainScale.x,i0*terrainScale.y,0);
	vec3 v1(0,i2*terrainScale.y,1.0f/(float)(height-1)*terrainScale.z);
	vec3 v2(1.0f/(float)(width-1)*terrainScale.x,i1*terrainScale.y,1.0f/(float)(height-1)*terrainScale.z);
	return Cross(v1-v0,v2-v0).Normalize();
}