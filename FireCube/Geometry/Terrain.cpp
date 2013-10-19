#include <sstream>

#include "Geometry/Terrain.h"
#include "Core/Engine.h"
#include "Utils/Image.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/IndexBuffer.h"

using namespace FireCube;

Terrain::Terrain(Engine *engine) : Component(engine), patchSize(32), verticesSpacing(1.0f, 64.0f, 1.0f)
{

}

void Terrain::CreateFromHeightMap(Image *image)
{
	numPatchesX = (image->GetWidth() - 1) / patchSize;
	numPatchesY = (image->GetHeight() - 1) / patchSize;
	numVerticesX = numPatchesX * patchSize + 1;
	numVerticesY = numPatchesY * patchSize + 1;
	heightData.resize(numVerticesX * numVerticesY);
	for (int y = 0; y < numVerticesY; ++y)
	{
		for (int x = 0; x < numVerticesX; ++x)
		{
			heightData[y * numVerticesX + x] = image->GetPixel(x, y).y * verticesSpacing.y;
		}
	}
	patchWorldSize = vec2(verticesSpacing.x, verticesSpacing.z) * (float) patchSize;
	GenerateIndexBuffer();	
	vec3 offset(patchWorldSize .x * numPatchesX * -0.5f, 0, patchWorldSize.y * numPatchesY * -0.5f);
	for (int y = 0; y < numPatchesY; ++y)
	{
		for (int x = 0; x < numPatchesX; ++x)
		{
			std::ostringstream oss;
			oss << "patch_" << x << "_" << y;
			std::string nodeName = oss.str();
			NodePtr patchNode = node->GetChild(nodeName, true);
			if (!patchNode)
			{
				patchNode = NodePtr(new Node(engine, nodeName));
				node->AddChild(patchNode);
			}
			TerrainPatch *patch = new TerrainPatch(engine);
			patches.push_back(patch);
			patchNode->AddComponent(patch);			
			patchNode->SetTranslation(offset + vec3(x * patchWorldSize.x, 0, y * patchWorldSize.y));			
			GeneratePatchGeometry(patch, x, y);						
		}
	}
}

void Terrain::GenerateIndexBuffer()
{
	indexBuffer = IndexBufferPtr(new IndexBuffer(engine->GetRenderer()));
	indexBuffer->SetShadowed(true);
	std::vector<unsigned int> indices(patchSize * patchSize * 6);
	for (int y = 0; y < patchSize; ++y)
	{
		for (int x = 0; x < patchSize; ++x)
		{
			int i0 = y * (patchSize + 1) + x;
			int i1 = i0 + 1;
			int i2 = (y + 1) * (patchSize + 1) + x;
			int i3 = i2 + 1;
			indices[y * patchSize * 6 + x * 6 + 0] = i0;
			indices[y * patchSize * 6 + x * 6 + 1] = i2;
			indices[y * patchSize * 6 + x * 6 + 2] = i1;
			indices[y * patchSize * 6 + x * 6 + 3] = i1;
			indices[y * patchSize * 6 + x * 6 + 4] = i2;
			indices[y * patchSize * 6 + x * 6 + 5] = i3;			
		}
	}
	indexBuffer->LoadData(&indices[0], indices.size(), STATIC);
}

void Terrain::GeneratePatchGeometry(TerrainPatch *patch, int patchX, int patchY)
{	
	GeometryPtr geometry = patch->GetGeometry();
	VertexBufferPtr vertexBuffer(new VertexBuffer(engine->GetRenderer()));
	vertexBuffer->SetShadowed(true);
	geometry->SetVertexBuffer(vertexBuffer);
	geometry->SetIndexBuffer(indexBuffer);
	geometry->SetMaterial(material);
	BoundingBox boundingBox;	
	unsigned int vertexSize = 3 + 3 + 2;
	unsigned int vertexCount = (patchSize + 1) * (patchSize + 1);
	std::vector<float> vertexData(vertexCount * vertexSize);	
	for (int z = 0; z < patchSize + 1; ++z)
	{
		for (int x = 0; x < patchSize + 1; ++x)
		{			
			vec3 pos(x * verticesSpacing.x, GetHeightDiscrete(patchX * patchSize + x, patchY * patchSize + z), z * verticesSpacing.z);
			*((vec3 *) &vertexData[(z * (patchSize + 1) + x) * vertexSize + 0]) = pos;
			boundingBox.Expand(pos);
			vec3 normal = GetNormalDiscrete(patchX * patchSize + x, patchY * patchSize + z);
			*((vec3 *) &vertexData[(z * (patchSize + 1) + x) * vertexSize + 3]) = normal;
			vec2 uv((float) (patchX *patchSize + x) / (float) (numVerticesX - 1), (float) (patchY *patchSize + z) / (float) (numVerticesY - 1));
			*((vec2 *) &vertexData[(z * (patchSize + 1) + x) * vertexSize + 6]) = uv;
		}	
	}
	
	vertexBuffer->LoadData(&vertexData[0], vertexCount, VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_NORMAL | VERTEX_ATTRIBUTE_TEXCOORD0, STATIC);	
	patch->SetBoundingBox(boundingBox);		
	geometry->SetPrimitiveCount(patchSize * patchSize * 2);
	geometry->Update();
}

float Terrain::GetHeightDiscrete(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= numVerticesX)
		x = numVerticesX - 1;
	if (y < 0)
		y = 0;
	else if (y >= numVerticesY)
		y = numVerticesY - 1;
	return heightData[y * numVerticesX + x];
}

vec3 Terrain::GetNormalDiscrete(int x, int y)
{	
	float baseHeight = GetHeightDiscrete(x, y);
	float d0 = GetHeightDiscrete(x, y - 1) - baseHeight;
	float d1 = GetHeightDiscrete(x + 1, y - 1) - baseHeight;
	float d2 = GetHeightDiscrete(x + 1, y) - baseHeight;
	float d3 = GetHeightDiscrete(x + 1, y + 1) - baseHeight;
	float d4 = GetHeightDiscrete(x, y + 1) - baseHeight;
	float d5 = GetHeightDiscrete(x - 1, y + 1) - baseHeight;
	float d6 = GetHeightDiscrete(x - 1, y) - baseHeight;
	float d7 = GetHeightDiscrete(x - 1, y - 1) - baseHeight;
	vec3 n = vec3(0, verticesSpacing.z, d0).Normalized() +
			 vec3(-d1, verticesSpacing.x + verticesSpacing.z, d1).Normalized() +
			 vec3(-d2, verticesSpacing.x, 0).Normalized() + 
			 vec3(-d3, verticesSpacing.x + verticesSpacing.z, -d3).Normalized() +
			 vec3(0, verticesSpacing.z, -d4).Normalized() +
			 vec3(d5, verticesSpacing.x + verticesSpacing.z, -d5).Normalized() +
			 vec3(d6, verticesSpacing.x, 0).Normalized() + 
			 vec3(d7, verticesSpacing.x + verticesSpacing.z, d7).Normalized();
	n.Normalize();
	return n;
}

void Terrain::SetVerticesSpacing(vec3 spacing)
{
	verticesSpacing = spacing;
}

void Terrain::SetPatchSize(int patchSize)
{
	this->patchSize = patchSize;
}

void Terrain::SetMaterial(MaterialPtr material)
{
	this->material = material;
	for (auto terrainPatch : patches)
		terrainPatch->SetMaterial(material);
}

TerrainPatch::TerrainPatch(Engine *engine) : Renderable(engine)
{
	renderableParts.resize(1);
	geometry = GeometryPtr(new Geometry(engine->GetRenderer()));
	geometry->SetPrimitiveType(TRIANGLES);
	renderableParts[0].geometry = geometry.get();	
}

GeometryPtr TerrainPatch::GetGeometry()
{
	return geometry;
}

void TerrainPatch::UpdateWorldBoundingBox()
{
	worldBoundingBox = boundingBox;
	worldBoundingBox.Transform(node->GetWorldTransformation());
}

void TerrainPatch::SetBoundingBox(BoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
}

void TerrainPatch::SetMaterial(MaterialPtr material)
{
	geometry->SetMaterial(material);
}

void TerrainPatch::IntersectRay(RayQuery &rayQuery)
{
	float distance;
	if (rayQuery.ray.IntersectBoundingBox(GetWorldBoundingBox(), distance) && distance <= rayQuery.maxDistance)
	{
		mat4 worldTransform = node->GetWorldTransformation();
		worldTransform.Inverse();
		Ray localRay = rayQuery.ray.Transformed(worldTransform);
		vec3 normal;
		if (geometry->IntersectRay(localRay, distance, normal) && distance <= rayQuery.maxDistance)
		{
			RayQueryResult result;
			result.distance = distance;
			result.renderable = this;
			result.normal = normal;
			rayQuery.results.push_back(result);
		}		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*

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

*/