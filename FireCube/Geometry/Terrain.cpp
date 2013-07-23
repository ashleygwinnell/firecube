#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <memory>
using namespace std;
#include "glew.h"

#include "Utils/utils.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/Renderer.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
#include "Rendering/Buffer.h"
#include "Math/Plane.h"
#include "Math/Frustum.h"
#include "Scene/Camera.h"
#include "Scene/GeometryNode.h"
#include "Utils/Image.h"
#include "Geometry/Terrain.h"
#include "Scene/TerrainNode.h"

using namespace FireCube;

Terrain::Terrain()
{
	geometry = GeometryPtr(new Geometry);
	geometry->SetMaterial(MaterialPtr(new Material));
	geometry->GetMaterial()->SetAmbientColor(vec3(0.2f, 0.2f, 0.2f));
	geometry->GetMaterial()->SetDiffuseColor(vec3(0.5f, 0.5f, 0.5f));
	geometry->SetPrimitiveType(TRIANGLES);
}
bool Terrain::GenerateTerrain(const string &heightmap, vec3 sizeVertices, vec2 sizeUv, bool smoothNormals)
{
	boundingBox.SetMin(vec3(0, 0, 0));
	boundingBox.SetMax(sizeVertices);
	terrainScale = sizeVertices;
	if (!heightmapImage.Load(heightmap))
		return false;
	width = heightmapImage.GetWidth();
	length = heightmapImage.GetHeight();
	
	vector<vec3> &vertex = geometry->GetVertices();
	vector<vec3> &normal = geometry->GetNormals();
	vector<vec2> &uv = geometry->GetDiffuseUV();
	vertex.resize((width - 1) * (length - 1) * 2 * 3);
	normal.resize((width - 1) * (length - 1) * 2 * 3);
	uv.resize((width - 1) * (length - 1) * 2 * 3);
	unsigned int currentIndex = 0;
	unsigned int normalCurrentIndex = 0;
	unsigned int uvCurrentIndex = 0;
	vec2 verticesDiff(1.0f / (float)(width - 1)*sizeVertices.x, 1.0f / (float)(length - 1)*sizeVertices.z);
	vec2 uvDiff(1.0f / (float)(width - 1)*sizeUv.x, 1.0f / (float)(length - 1)*sizeUv.y);
	for (unsigned int y = 0; y < length - 1; y++)
	{
		for (unsigned int x = 0; x < width - 1; x++)
		{
			vertex[currentIndex].x = (float)x * verticesDiff.x;
			vertex[currentIndex].y = heightmapImage.GetPixel(x, y).x * sizeVertices.y;
			vertex[currentIndex].z = (float)y * verticesDiff.y;
			vec3 v0(vertex[currentIndex]);
			++currentIndex;

			vertex[currentIndex].x = (float)x * verticesDiff.x;
			vertex[currentIndex].y = heightmapImage.GetPixel(x, y + 1).x * sizeVertices.y;
			vertex[currentIndex].z = (float)(y + 1) * verticesDiff.y;
			vec3 v1(vertex[currentIndex]);
			++currentIndex;

			vertex[currentIndex].x = (float)(x + 1) * verticesDiff.x;
			vertex[currentIndex].y = heightmapImage.GetPixel(x + 1, y).x * sizeVertices.y;
			vertex[currentIndex].z = (float)y * verticesDiff.y;
			vec3 v2(vertex[currentIndex]);
			++currentIndex;

			vertex[currentIndex] = vertex[currentIndex - 1];
			++currentIndex;

			vertex[currentIndex] = vertex[currentIndex - 3];			
			++currentIndex;

			vertex[currentIndex].x = (float)(x + 1) * verticesDiff.x;
			vertex[currentIndex].y = heightmapImage.GetPixel(x + 1, y + 1).x * sizeVertices.y;
			vertex[currentIndex].z = (float)(y + 1) * verticesDiff.y;
			vec3 v3(vertex[currentIndex]);
			++currentIndex;

			if (!smoothNormals)
			{			
				vec3 n1 = -Cross(v2 - v0, v1 - v0).Normalize();
				normal[normalCurrentIndex] = n1;
				++normalCurrentIndex;
				normal[normalCurrentIndex] = n1;
				++normalCurrentIndex;
				normal[normalCurrentIndex] = n1;
				++normalCurrentIndex;


				vec3 n2 = -Cross(v3 - v2, v1 - v2).Normalize();
				normal[normalCurrentIndex] = n2;
				++normalCurrentIndex;
				normal[normalCurrentIndex] = n2;
				++normalCurrentIndex;
				normal[normalCurrentIndex] = n2;
				++normalCurrentIndex;			
			}

			vec2 uvPos((float)x * uvDiff.x + uvDiff.x / 2.0f, (float)y * uvDiff.y + uvDiff.y / 2.0f);
			uv[uvCurrentIndex] = uvPos;
			++uvCurrentIndex;

			uv[uvCurrentIndex] = uvPos;
			++uvCurrentIndex;

			uv[uvCurrentIndex] = uvPos;
			++uvCurrentIndex;

			uv[uvCurrentIndex] = uvPos;
			++uvCurrentIndex;

			uv[uvCurrentIndex] = uvPos;
			++uvCurrentIndex;

			uv[uvCurrentIndex] = uvPos;
			++uvCurrentIndex;
		}
	}
	geometry->UpdateBuffers();
		
	string::size_type d;
	d = heightmap.find_last_of("/");
	if (d == string::npos)
		d = heightmap.find_last_of("\\");
	if (d == string::npos)
		d = 0;
	if (d != string::npos)
	{
		string quadfile = heightmap.substr(d + 1);
		d = quadfile.find_last_of(".");
		if (d != string::npos)
			quadfile = quadfile.substr(0, d + 1);
		quadfile.append("bin");
		ifstream f(quadfile.c_str(), ios::binary);
		if (f.is_open())
		{
			f.close();
			quadtree.Load(quadfile);
		}
		else
		{
			quadtree.Init(vec2((float)GetWidth(), (float)GetLength()), vec2(sizeVertices.x, sizeVertices.z));
			quadtree.Build(sizeVertices.x / 32.0f, 1);
			quadtree.Save(quadfile);
		}
	}
	else
		return false;

	return true;
}
void Terrain::Render(CameraPtr camera)
{
	unsigned int count = quadtree.Render(camera, geometry->GetIndices());
	geometry->UpdateIndexBuffer();
	geometry->SetVertexCount(count);
	geometry->SetPrimitiveCount(count / 3);	
}
float Terrain::GetHeight(vec2 pos)
{
	if ((pos.x < 0) || (pos.x >= terrainScale.x) || (pos.y < 0) || (pos.y >= terrainScale.z))
		return 0;
	pos.x = pos.x / terrainScale.x * (float)(width - 1);
	pos.y = pos.y / terrainScale.z * (float)(length - 1);
	int ix = (int)pos.x, iy = (int)pos.y;
	float fx = pos.x - (float)ix, fy = pos.y - (float)iy;
	float i0, i1, i2;
	float z0, z1;
	z0 = fx * fx + fy * fy;
	z1 = (1.0f - fx) * (1.0f - fx) + (1.0f - fy) * (1.0f - fy);
	if (z0 < z1)
	{
		i0 = heightmapImage.GetPixel(ix, iy).x;
		i1 = heightmapImage.GetPixel(ix + 1, iy).x;
		i2 = heightmapImage.GetPixel(ix, iy + 1).x;
		float d0, d1;
		d0 = i1 - i0;
		d1 = i2 - i0;
		return (i0 + d0 * fx + d1 * fy) * terrainScale.y;
	}
	i0 = heightmapImage.GetPixel(ix + 1, iy).x;
	i2 = heightmapImage.GetPixel(ix, iy + 1).x;
	i1 = heightmapImage.GetPixel(ix + 1, iy + 1).x;
	float d0, d1;
	d0 = i2 - i1;
	d1 = i0 - i1;
	return (i1 + d0 * (1.0f - fx) + d1 * (1.0f - fy)) * terrainScale.y;
}
int Terrain::GetWidth()
{
	return width;
}
int Terrain::GetLength()
{
	return length;
}
vec3 Terrain::GetNormal(vec2 pos)
{
	if ((pos.x < 0) || (pos.x >= terrainScale.x) || (pos.y < 0) || (pos.y >= terrainScale.z))
		return vec3(0, 1, 0);

	pos.x = pos.x / terrainScale.x * (float)(width - 1);
	pos.y = pos.y / terrainScale.z * (float)(length - 1);
	int ix = (int)pos.x, iy = (int)pos.y;
	float fx = pos.x - (float)ix, fy = pos.y - (float)iy;
	float i0, i1, i2;
	float z0, z1;
	z0 = fx * fx + fy * fy;
	z1 = (1.0f - fx) * (1.0f - fx) + (1.0f - fy) * (1.0f - fy);
	if (z0 < z1)
	{
		i0 = heightmapImage.GetPixel(ix, iy).x;
		i1 = heightmapImage.GetPixel(ix + 1, iy).x;
		i2 = heightmapImage.GetPixel(ix, iy + 1).x;
		vec3 v0(0, i0 * terrainScale.y, 0);
		vec3 v1(1.0f / (float)(width - 1)*terrainScale.x, i1 * terrainScale.y, 0);
		vec3 v2(1.0f / (float)(width - 1)*terrainScale.x, i2 * terrainScale.y, 1.0f / (float)(length - 1)*terrainScale.z);
		return Cross(v2 - v0, v1 - v0).Normalize();
	}
	i0 = heightmapImage.GetPixel(ix + 1, iy).x;
	i2 = heightmapImage.GetPixel(ix, iy + 1).x;
	i1 = heightmapImage.GetPixel(ix + 1, iy + 1).x;
	vec3 v0(1.0f / (float)(width - 1)*terrainScale.x, i0 * terrainScale.y, 0);
	vec3 v1(0, i2 * terrainScale.y, 1.0f / (float)(length - 1)*terrainScale.z);
	vec3 v2(1.0f / (float)(width - 1)*terrainScale.x, i1 * terrainScale.y, 1.0f / (float)(length - 1)*terrainScale.z);
	return Cross(v1 - v0, v2 - v0).Normalize();
}

GeometryPtr Terrain::GetGeometry()
{
	return geometry;
}

BoundingBox Terrain::GetBoundingBox() const
{
	return boundingBox;
}

Terrain::QuadTree::QuadTree()
{
}

void Terrain::QuadTree::Init(vec2 size, vec2 verticesSize)
{

	root = NodePtr(new Node);
	root->face = Node::FaceListPtr(new Node::FaceList);
	root->boundingBox.SetMin(vec3(0.0f, 0.0f, 0.0f));
	root->boundingBox.SetMax(vec3(verticesSize.x, 50.0f, verticesSize.y));
	aspect = (size - vec2(1.0f, 1.0f)) / verticesSize;
	for (unsigned int y = 0; y < size.y - 1.0f; y++)
	{
		for (unsigned int x = 0; x < size.x - 1.0f; x++)
		{
			root->face->push_back(vec2((float)x / (size.x - 1.0f)*verticesSize.x, (float)y / (size.y - 1.0f)*verticesSize.y));
		}
	}
	this->size = size - vec2(1.0f, 1.0f);
}
void Terrain::QuadTree::Build(float minSize, unsigned int maxNumberOfFaces)
{
	Build(root, minSize, maxNumberOfFaces);
	BuildIndices(root);
}
void Terrain::QuadTree::Build(NodePtr node, float minSize, unsigned int maxNumberOfFaces)
{
	if ((node->boundingBox.GetWidth() <= minSize) || (node->face->size() < maxNumberOfFaces))
		return;
	vec3 midPoint = node->boundingBox.GetCenter();
	NodePtr child[4];
	child[0] = NodePtr(new Node);
	child[0]->face = Node::FaceListPtr(new Node::FaceList);
	child[1] = NodePtr(new Node);
	child[1]->face = Node::FaceListPtr(new Node::FaceList);
	child[2] = NodePtr(new Node);
	child[2]->face = Node::FaceListPtr(new Node::FaceList);
	child[3] = NodePtr(new Node);
	child[3]->face = Node::FaceListPtr(new Node::FaceList);
	child[0]->boundingBox.SetMin(node->boundingBox.GetMin());
	child[0]->boundingBox.SetMax(vec3(midPoint.x, 50.0f, midPoint.z));
	child[1]->boundingBox.SetMin(vec3(midPoint.x, 0.0f, node->boundingBox.GetMin().z));
	child[1]->boundingBox.SetMax(vec3(node->boundingBox.GetMax().x, 50.0f, midPoint.z));
	child[2]->boundingBox.SetMin(vec3(node->boundingBox.GetMin().x, 0.0f, midPoint.z));
	child[2]->boundingBox.SetMax(vec3(midPoint.x, 50.0f, node->boundingBox.GetMax().z));
	child[3]->boundingBox.SetMin(vec3(midPoint.x, 0.0f, midPoint.z));
	child[3]->boundingBox.SetMax(node->boundingBox.GetMax());
	for (unsigned int i = 0; i < node->face->size(); i++)
	{
		vec2 f = (*node->face)[i];
		vec2 f2 = (*node->face)[i] + vec2(1.0f / aspect.x, 1.0f / aspect.y);
		for (unsigned int j = 0; j < 4; j++)
		{                       
			if (((f.x >= child[j]->boundingBox.GetMin().x) && (f.y >= child[j]->boundingBox.GetMin().z) && (f.x <= child[j]->boundingBox.GetMax().x) && (f.y <= child[j]->boundingBox.GetMax().z) ) || ((f2.x >= child[j]->boundingBox.GetMin().x) && (f2.y >= child[j]->boundingBox.GetMin().z) && (f2.x <= child[j]->boundingBox.GetMax().x) && (f2.y <= child[j]->boundingBox.GetMax().z)))
			{
				child[j]->face->push_back(f);
			}
		}
	}
	node->face.reset();
	for (unsigned int i = 0; i < 4; i++)
	{
		if (child[i]->face->size() == 0)
			child[i].reset();
		node->child[i] = child[i];
	}
	for (unsigned int i = 0; i < 4; i++)
		if (node->child[i])
			Build(node->child[i], minSize, maxNumberOfFaces);
}
unsigned int Terrain::QuadTree::Render(CameraPtr camera, vector<unsigned int> &indicesToRender)
{
	currentIndex = 0;
	Render(root, camera, indicesToRender);	
	return currentIndex;
}
void Terrain::QuadTree::Render(NodePtr node, CameraPtr camera, vector<unsigned int> &indicesToRender)
{
	if (camera->GetFrustum().Contains(node->boundingBox))
	{
		if (!node->indices.empty())
		{
			if (indicesToRender.size() - currentIndex < node->indices.size())
				indicesToRender.resize(indicesToRender.size() + node->indices.size());
			std::copy(node->indices.begin(), node->indices.end(), indicesToRender.begin() + currentIndex);
			currentIndex += node->indices.size();
		}
		else
		{
			for (unsigned int i = 0; i < 4; i++)
				if (node->child[i])
					Render(node->child[i], camera, indicesToRender);
		}
	}
}
void Terrain::QuadTree::BuildIndices(NodePtr node)
{
	unsigned int sx = (unsigned int)this->size.x * 6;
	if ((node->face) && (node->face->size() > 0))
	{
		node->indices.resize(node->face->size() * 6);
		unsigned int cIndex = 0;
		for (unsigned int i = 0; i < node->face->size(); i++)
		{
			vec2 f = (*node->face)[i] * aspect;
			unsigned int x = (unsigned int)f.x;
			unsigned int y = (unsigned int)f.y;
			unsigned int index = y * sx + x * 6;            
			node->indices[cIndex++] = index + 0;
			node->indices[cIndex++] = index + 1;
			node->indices[cIndex++] = index + 2;
			node->indices[cIndex++] = index + 3;
			node->indices[cIndex++] = index + 4;
			node->indices[cIndex++] = index + 5;
		}
		node->face.reset();
	}
	else
	{
		for (unsigned int i = 0; i < 4; i++)
			if (node->child[i])
				BuildIndices(node->child[i]);
	}
}
void Terrain::QuadTree::RenderLines()
{
	//Renderer::UseProgram(plainColor);
	RenderLines(root);
}
void Terrain::QuadTree::RenderLines(NodePtr node)
{
	BufferPtr v(new Buffer);
	v->Create();
	vector<vec3> vv;
	vv.push_back(vec3(node->boundingBox.GetMin().x, 20.0f, node->boundingBox.GetMin().z));
	vv.push_back(vec3(node->boundingBox.GetMax().x, 20.0f, node->boundingBox.GetMin().z));
	vv.push_back(vec3(node->boundingBox.GetMax().x, 20.0f, node->boundingBox.GetMax().z));
	vv.push_back(vec3(node->boundingBox.GetMin().x, 20.0f, node->boundingBox.GetMax().z));
	v->LoadData(&vv[0], sizeof(vec3)*vv.size(), DYNAMIC);
	//v->SetVertexStream(3);
	Renderer::RenderStream(LINE_LOOP, vv.size());
	for (unsigned int j = 0; j < 4; j++)
		if (node->child[j])
			RenderLines(node->child[j]);
}
void Terrain::QuadTree::Save(const string &filename)
{
	ofstream file(filename.c_str(), ios::binary);
	file.write((const char*)&size, sizeof(vec2));
	file.write((const char*)&aspect, sizeof(vec2));
	Save(root, file);
}
void Terrain::QuadTree::Save(NodePtr node, ofstream &file)
{
	unsigned char children = 0;
	unsigned int numIndices = node->indices.size();
	if (node->child[0])
		children |= 1;
	if (node->child[1])
		children |= 2;
	if (node->child[2])
		children |= 4;
	if (node->child[3])
		children |= 8;
	file.write((const char*)&node->boundingBox.GetMin(), sizeof(vec3));
	file.write((const char*)&node->boundingBox.GetMax(), sizeof(vec3));
	file.write((const char*)&children, 1);
	file.write((const char*)&numIndices, sizeof(unsigned int));
	if (numIndices > 0)
	{
		unsigned int lastIndex = node->indices[0];
		for (unsigned int i = 1; i < node->indices.size(); i++)
		{
			if (node->indices[i] != node->indices[i - 1] + 1)
			{
				file.write((const char *)&lastIndex, sizeof(unsigned int));
				unsigned int j = node->indices[i - 1];
				file.write((const char *)&j, sizeof(unsigned int));
				lastIndex = node->indices[i];
				if (i == node->indices.size() - 1)
				{
					file.write((const char *)&lastIndex, sizeof(unsigned int));
					file.write((const char *)&lastIndex, sizeof(unsigned int));
				}
			}
			else
			{
				if (i == node->indices.size() - 1)
				{
					file.write((const char *)&lastIndex, sizeof(unsigned int));
					unsigned int j = node->indices[i];
					file.write((const char *)&j, sizeof(unsigned int));
				}
			}
		}
	}
	for (unsigned int i = 0; i < 4; i++)
		if (node->child[i])
			Save(node->child[i], file);
}
void Terrain::QuadTree::Load(const string &filename)
{
	ifstream file(filename.c_str(), ios::binary);
	file.seekg(0, ios_base::end);
	unsigned int size = (unsigned int) file.tellg(), currentIndex = 0;
	vector<unsigned char> buffer;
	buffer.resize(size);
	file.seekg(0, ios_base::beg);
	file.read((char*)&buffer[0], size);
	this->size = *(vec2*)&buffer[currentIndex];
	currentIndex += sizeof(vec2);
	aspect = *(vec2*)&buffer[currentIndex];
	currentIndex += sizeof(vec2);
	root = NodePtr(new Node);
	Load(buffer, currentIndex, root);
}
void Terrain::QuadTree::Load(const vector<unsigned char> &buffer, unsigned int &currentIndex, NodePtr node)
{
	unsigned char children;
	unsigned int numIndices;
	node->boundingBox.SetMin(*(vec3*)&buffer[currentIndex]);
	currentIndex += sizeof(vec3);
	node->boundingBox.SetMax(*(vec3*)&buffer[currentIndex]);
	currentIndex += sizeof(vec3);
	children = *(unsigned char*)&buffer[currentIndex];
	currentIndex += 1;
	numIndices = *(unsigned int*)&buffer[currentIndex];
	currentIndex += sizeof(unsigned int);
	if (numIndices > 0)
	{
		node->indices.resize(numIndices);
		unsigned int startIndex, endIndex, nodeCurrentIndex = 0;
		while (nodeCurrentIndex != numIndices)
		{

			startIndex = *(unsigned int*)&buffer[currentIndex];
			currentIndex += sizeof(unsigned int);
			endIndex = *(unsigned int*)&buffer[currentIndex];
			currentIndex += sizeof(unsigned int);
			for (unsigned int i = startIndex; i <= endIndex; i++)
				node->indices[nodeCurrentIndex++] = i;
		}
	}
	if (children & 1)
	{
		node->child[0] = NodePtr(new Node);
		Load(buffer, currentIndex, node->child[0]);
	}
	if (children & 2)
	{
		node->child[1] = NodePtr(new Node);
		Load(buffer, currentIndex, node->child[1]);
	}
	if (children & 4)
	{
		node->child[2] = NodePtr(new Node);
		Load(buffer, currentIndex, node->child[2]);
	}
	if (children & 8)
	{
		node->child[3] = NodePtr(new Node);
		Load(buffer, currentIndex, node->child[3]);
	}
}
