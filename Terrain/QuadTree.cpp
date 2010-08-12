#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "Frustum.h"
#include "QuadTree.h"

QuadTree::QuadTree()
{
}
void QuadTree::Initialize()
{	
	indexBuffer.Create();
	plainColor.Create(Renderer::GetShaderManager().Create("plainColor.vshader"),Renderer::GetShaderManager().Create("plainColor.fshader"));
}
void QuadTree::Init(vec2 size,vec2 verticesSize)
{
	
	root=NodePtr(new Node);
	root->face=Node::FaceListPtr(new Node::FaceList);
	root->min=vec2(0,0);
	root->max=verticesSize;
	aspect=(size-vec2(1.0f,1.0f))/verticesSize;
	for (DWORD y=0;y<size.y-1.0f;y++)
	{
		for (DWORD x=0;x<size.x-1.0f;x++)
		{
			root->face->push_back(vec2((float)x/(size.x-1.0f)*verticesSize.x,(float)y/(size.y-1.0f)*verticesSize.y));
		}
	}
	this->size=size-vec2(1.0f,1.0f);	
}
void QuadTree::Build(float minSize,DWORD maxNumerOfFaces)
{
	Build(root,minSize,maxNumerOfFaces);
	BuildIndices(root);
}
void QuadTree::Build(NodePtr node,float minSize,DWORD maxNumerOfFaces)
{
	if ((node->max.x-node->min.x<=minSize) || (node->face->size()<maxNumerOfFaces))
		return;
	vec2 midPoint=(node->min+node->max)/2.0f;
	NodePtr child[4];
	child[0]=NodePtr(new Node);
	child[0]->face=Node::FaceListPtr(new Node::FaceList);
	child[1]=NodePtr(new Node);
	child[1]->face=Node::FaceListPtr(new Node::FaceList);
	child[2]=NodePtr(new Node);
	child[2]->face=Node::FaceListPtr(new Node::FaceList);
	child[3]=NodePtr(new Node);
	child[3]->face=Node::FaceListPtr(new Node::FaceList);
	child[0]->min=node->min;
	child[0]->max=midPoint;
	child[1]->min=vec2(midPoint.x,node->min.y);
	child[1]->max=vec2(node->max.x,midPoint.y);
	child[2]->min=vec2(node->min.x,midPoint.y);
	child[2]->max=vec2(midPoint.x,node->max.y);
	child[3]->min=midPoint;
	child[3]->max=node->max;
	for (DWORD i=0;i<node->face->size();i++)
	{		
		for (DWORD j=0;j<4;j++)
		{	
			vec2 f=(*node->face)[i];
			vec2 f2=(*node->face)[i]+vec2(1.0f/aspect.x,1.0f/aspect.y);
			if (((f.x>=child[j]->min.x) && (f.y>=child[j]->min.y) && (f.x<=child[j]->max.x) && (f.y<=child[j]->max.y) ) || ((f2.x>=child[j]->min.x) && (f2.y>=child[j]->min.y) && (f2.x<=child[j]->max.x) && (f2.y<=child[j]->max.y)))
			{
				child[j]->face->push_back(f);
			}
		}
	}
	node->face.reset();
	for (DWORD i=0;i<4;i++)
	{
		if (child[i]->face->size()==0)
			child[i].reset();
		node->child[i]=child[i];
	}
	for (DWORD i=0;i<4;i++)
		if (node->child[i])
			Build(node->child[i],minSize,maxNumerOfFaces);
}
DWORD QuadTree::Render(Frustum &frustum)
{
	currentIndex=0;
	Render(root,frustum);
	if (currentIndex>0)
	{
		indexBuffer.LoadIndexData(&indicesToRender[0],currentIndex,DYNAMIC);
		indexBuffer.SetIndexStream();
		Renderer::RenderIndexStream(TRIANGLES,currentIndex);
	}
	return currentIndex;
}
void QuadTree::Render(NodePtr node,Frustum &frustum)
{
	vec3 pos=vec3( (node->min.x+node->max.x)/2.0f,25.0f,(node->min.y+node->max.y)/2.0f);
	vec3 sized=vec3( (node->max.x-node->min.x)/2.0f,25.0f,(node->max.y-node->min.y)/2.0f);
	DWORD sx=(DWORD)this->size.x*6;
	if (frustum.BoxInFrustum(pos,sized))
	{
		if (!node->indices.empty())
		{	
			if (indicesToRender.size()-currentIndex<node->indices.size())
				indicesToRender.resize(indicesToRender.size()+node->indices.size());
			std::copy(node->indices.begin(),node->indices.end(),indicesToRender.begin()+currentIndex);			
			currentIndex+=node->indices.size();
		}
		else
		{
			for (DWORD i=0;i<4;i++)
				if (node->child[i])
					Render(node->child[i],frustum);
		}
	}
}
void QuadTree::BuildIndices(NodePtr node)
{
	DWORD sx=(DWORD)this->size.x*6;
	if ((node->face) && (node->face->size()>0))
	{
		node->indices.resize(node->face->size()*6);
		DWORD cIndex=0;
		for (DWORD i=0;i<node->face->size();i++)	
		{
			vec2 f=(*node->face)[i]*aspect;
			DWORD x=(DWORD)f.x;
			DWORD y=(DWORD)f.y;				
			DWORD index=y*sx+x*6;
			DWORD fi=y*sx/6+x;
			node->indices[cIndex++]=index+0;
			node->indices[cIndex++]=index+1;
			node->indices[cIndex++]=index+2;
			node->indices[cIndex++]=index+3;
			node->indices[cIndex++]=index+4;
			node->indices[cIndex++]=index+5;
		}
		node->face.reset();
	}
	else
	{
		for (DWORD i=0;i<4;i++)
			if (node->child[i])
				BuildIndices(node->child[i]);
	}
}
void QuadTree::RenderLines()
{
	Renderer::UseProgram(plainColor);
	RenderLines(root);
}
void QuadTree::RenderLines(NodePtr node)
{
	Buffer v;
	v.Create();
	vector<vec3> vv;
	vv.push_back(vec3(node->min.x,20.0f,node->min.y));
	vv.push_back(vec3(node->max.x,20.0f,node->min.y));
	vv.push_back(vec3(node->max.x,20.0f,node->max.y));
	vv.push_back(vec3(node->min.x,20.0f,node->max.y));
	v.LoadData(&vv[0],sizeof(vec3)*vv.size(),DYNAMIC);
	v.SetVertexStream(3);
	Renderer::RenderStream(LINE_LOOP,vv.size());
	for (DWORD j=0;j<4;j++)
		if (node->child[j])
			RenderLines(node->child[j]);
}
void QuadTree::Save(const string &filename)
{
	ofstream file(filename.c_str(),ios::binary);
	file.write((const char*)&size,sizeof(vec2));
	file.write((const char*)&aspect,sizeof(vec2));
	Save(root,file);
}
void QuadTree::Save(NodePtr node,ofstream &file)
{
	unsigned char children=0;
	DWORD numIndices=node->indices.size();
	if (node->child[0])
		children|=1;
	if (node->child[1])
		children|=2;
	if (node->child[2])
		children|=4;
	if (node->child[3])
		children|=8;
	file.write((const char*)&node->min,sizeof(vec2));
	file.write((const char*)&node->max,sizeof(vec2));
	file.write((const char*)&children,1);
	file.write((const char*)&numIndices,sizeof(DWORD));	
	if (numIndices>0)
	{
		DWORD lastIndex=node->indices[0];
		for (DWORD i=1;i<node->indices.size();i++)
		{
			if (node->indices[i]!=node->indices[i-1]+1)
			{
				file.write((const char *)&lastIndex,sizeof(DWORD));
				DWORD j=node->indices[i-1];
				file.write((const char *)&j,sizeof(DWORD));
				lastIndex=node->indices[i];
				if (i==node->indices.size()-1)
				{
					file.write((const char *)&lastIndex,sizeof(DWORD));
					file.write((const char *)&lastIndex,sizeof(DWORD));
				}
			}
			else
			{
				 if (i==node->indices.size()-1)
				 {
					 file.write((const char *)&lastIndex,sizeof(DWORD));
					 DWORD j=node->indices[i];
					 file.write((const char *)&j,sizeof(DWORD));					 
				 }
			}
		}		
	}
	for (DWORD i=0;i<4;i++)
		if (node->child[i])
			Save(node->child[i],file);
}
void QuadTree::Load(const string &filename)
{
	ifstream file(filename.c_str(),ios::binary);
	file.seekg(0,ios_base::end);
	DWORD size=file.tellg(),currentIndex=0;
	vector<unsigned char> buffer;
	buffer.resize(size);
	file.seekg(0,ios_base::beg);
	file.read((char*)&buffer[0],size);
	this->size=*(vec2*)&buffer[currentIndex];
	currentIndex+=sizeof(vec2);	
	aspect=*(vec2*)&buffer[currentIndex];
	currentIndex+=sizeof(vec2);	
	root=NodePtr(new Node);
	Load(buffer,currentIndex,root);
}
void QuadTree::Load(const vector<unsigned char> &buffer,DWORD &currentIndex,NodePtr node)
{
	unsigned char children;
	DWORD numIndices;
	node->min=*(vec2*)&buffer[currentIndex];
	currentIndex+=sizeof(vec2);		
	node->max=*(vec2*)&buffer[currentIndex];
	currentIndex+=sizeof(vec2);
	children=*(unsigned char*)&buffer[currentIndex];
	currentIndex+=1;
	numIndices=*(DWORD*)&buffer[currentIndex];
	currentIndex+=sizeof(DWORD);
	if (numIndices>0)
	{
		node->indices.resize(numIndices);
		DWORD startIndex,endIndex,nodeCurrentIndex=0;
		while (nodeCurrentIndex!=numIndices)
		{

			startIndex=*(DWORD*)&buffer[currentIndex];
			currentIndex+=sizeof(DWORD);
			endIndex=*(DWORD*)&buffer[currentIndex];
			currentIndex+=sizeof(DWORD);			
			for (DWORD i=startIndex;i<=endIndex;i++)
				node->indices[nodeCurrentIndex++]=i;
		}
	}
	if (children&1)
	{
		node->child[0]=NodePtr(new Node);
		Load(buffer,currentIndex,node->child[0]);
	}
	if (children&2)
	{
		node->child[1]=NodePtr(new Node);
		Load(buffer,currentIndex,node->child[1]);
	}
	if (children&4)
	{
		node->child[2]=NodePtr(new Node);
		Load(buffer,currentIndex,node->child[2]);
	}
	if (children&8)
	{
		node->child[3]=NodePtr(new Node);
		Load(buffer,currentIndex,node->child[3]);
	}
}