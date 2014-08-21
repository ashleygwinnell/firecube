#include <sstream>

#include "Geometry/Terrain.h"
#include "Core/Engine.h"
#include "Utils/Image.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/IndexBuffer.h"

using namespace FireCube;

Terrain::Terrain(Engine *engine) : Component(engine), patchSize(32), verticesSpacing(1.0f, 64.0f, 1.0f), smoothHeightMap(false), generateHardNormals(false),
								   indexBuffer(nullptr)
{

}

Terrain::~Terrain()
{
	delete indexBuffer;	
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
	if (smoothHeightMap)
		SmoothHeightMap();
	patchWorldSize = vec2(verticesSpacing.x, verticesSpacing.z) * (float) patchSize;
	if (!generateHardNormals)
		GenerateIndexBuffer();	
	vec3 offset(patchWorldSize .x * numPatchesX * -0.5f, 0, patchWorldSize.y * numPatchesY * -0.5f);
	for (int y = 0; y < numPatchesY; ++y)
	{
		for (int x = 0; x < numPatchesX; ++x)
		{
			std::ostringstream oss;
			oss << "patch_" << x << "_" << y;
			std::string nodeName = oss.str();
			Node *patchNode = node->GetChild(nodeName, true);
			if (!patchNode)
			{
				patchNode = new Node(engine, nodeName);
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
	indexBuffer = new IndexBuffer(engine->GetRenderer());
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
	Geometry *geometry = patch->GetGeometry();
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	vertexBuffer->SetShadowed(true);
	geometry->SetVertexBuffer(vertexBuffer);
	if (!generateHardNormals)
		geometry->SetIndexBuffer(indexBuffer);
	patch->SetMaterial(material);
	BoundingBox boundingBox;	
	unsigned int vertexSize = 3 + 3 + 2;
	unsigned int vertexCount;
	std::vector<float> vertexData;
	if (generateHardNormals)
	{
		vertexCount = patchSize * patchSize * 6;
		vertexData.resize(vertexCount * vertexSize);
		unsigned int currentPos = 0;
		for (int z = 0; z < patchSize; ++z)
		{
			for (int x = 0; x < patchSize; ++x)
			{
				vec3 pos0(x * verticesSpacing.x, GetHeightDiscrete(patchX * patchSize + x, patchY * patchSize + z), z * verticesSpacing.z);
				vec3 pos1((x + 1) * verticesSpacing.x, GetHeightDiscrete(patchX * patchSize + x + 1, patchY * patchSize + z), z * verticesSpacing.z);
				vec3 pos2((x + 1) * verticesSpacing.x, GetHeightDiscrete(patchX * patchSize + x + 1, patchY * patchSize + z + 1), (z + 1) * verticesSpacing.z);
				vec3 pos3(x * verticesSpacing.x, GetHeightDiscrete(patchX * patchSize + x, patchY * patchSize + z + 1), (z + 1) * verticesSpacing.z);
				vec2 uv0((float)(patchX *patchSize + x) / (float)(numVerticesX - 1), (float)(patchY *patchSize + z) / (float)(numVerticesY - 1));
				vec2 uv1((float)(patchX *patchSize + x + 1) / (float)(numVerticesX - 1), (float)(patchY *patchSize + z) / (float)(numVerticesY - 1));
				vec2 uv2((float)(patchX *patchSize + x + 1) / (float)(numVerticesX - 1), (float)(patchY *patchSize + z + 1) / (float)(numVerticesY - 1));
				vec2 uv3((float)(patchX *patchSize + x) / (float)(numVerticesX - 1), (float)(patchY *patchSize + z + 1) / (float)(numVerticesY - 1));
				vec3 normal0 = Cross(pos2 - pos0, pos1 - pos0).Normalized();
				vec3 normal1 = Cross(pos3 - pos0, pos2 - pos0).Normalized();

				*((vec3 *)&vertexData[currentPos]) = pos0;
				currentPos += 3;
				boundingBox.Expand(pos0);
				*((vec3 *)&vertexData[currentPos]) = normal0;
				currentPos += 3;
				*((vec2 *)&vertexData[currentPos]) = uv0;
				currentPos += 2;
				*((vec3 *)&vertexData[currentPos]) = pos2;
				currentPos += 3;
				boundingBox.Expand(pos2);
				*((vec3 *)&vertexData[currentPos]) = normal0;
				currentPos += 3;
				*((vec2 *)&vertexData[currentPos]) = uv2;
				currentPos += 2;
				*((vec3 *)&vertexData[currentPos]) = pos1;
				currentPos += 3;
				boundingBox.Expand(pos1);
				*((vec3 *)&vertexData[currentPos]) = normal0;
				currentPos += 3;
				*((vec2 *)&vertexData[currentPos]) = uv1;
				currentPos += 2;

				*((vec3 *)&vertexData[currentPos]) = pos0;
				currentPos += 3;
				boundingBox.Expand(pos0);
				*((vec3 *)&vertexData[currentPos]) = normal1;
				currentPos += 3;
				*((vec2 *)&vertexData[currentPos]) = uv0;
				currentPos += 2;				
				*((vec3 *)&vertexData[currentPos]) = pos3;
				currentPos += 3;
				boundingBox.Expand(pos3);
				*((vec3 *)&vertexData[currentPos]) = normal1;
				currentPos += 3;
				*((vec2 *)&vertexData[currentPos]) = uv3;
				currentPos += 2;
				*((vec3 *)&vertexData[currentPos]) = pos2;
				currentPos += 3;
				boundingBox.Expand(pos2);
				*((vec3 *)&vertexData[currentPos]) = normal1;
				currentPos += 3;
				*((vec2 *)&vertexData[currentPos]) = uv2;
				currentPos += 2;
			}
		}
	}	
	else
	{
		vertexCount = (patchSize + 1) * (patchSize + 1);
		vertexData.resize(vertexCount * vertexSize);
		for (int z = 0; z < patchSize + 1; ++z)
		{
			for (int x = 0; x < patchSize + 1; ++x)
			{
				vec3 pos(x * verticesSpacing.x, GetHeightDiscrete(patchX * patchSize + x, patchY * patchSize + z), z * verticesSpacing.z);
				*((vec3 *)&vertexData[(z * (patchSize + 1) + x) * vertexSize + 0]) = pos;
				boundingBox.Expand(pos);
				vec3 normal = GetNormalDiscrete(patchX * patchSize + x, patchY * patchSize + z);
				*((vec3 *)&vertexData[(z * (patchSize + 1) + x) * vertexSize + 3]) = normal;
				vec2 uv((float)(patchX *patchSize + x) / (float)(numVerticesX - 1), (float)(patchY *patchSize + z) / (float)(numVerticesY - 1));
				*((vec2 *)&vertexData[(z * (patchSize + 1) + x) * vertexSize + 6]) = uv;
			}
		}		
	}
	
	vertexBuffer->LoadData(&vertexData[0], vertexCount, VertexAttribute::POSITION | VertexAttribute::NORMAL | VertexAttribute::TEXCOORD0, STATIC);	
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

float Terrain::GetHeight(vec2 pos)
{
	vec2 offset(patchWorldSize.x * numPatchesX * 0.5f, patchWorldSize.y * numPatchesY * 0.5f);
	pos += offset;
	if ((pos.x < 0) || (pos.x >= patchWorldSize.x * (float)numPatchesX) || (pos.y < 0) || (pos.y >= patchWorldSize.y * (float)numPatchesY))
		return 0;
	pos.x = pos.x / (patchWorldSize.x * (float)numPatchesX) * (float)(numVerticesX - 1);
	pos.y = pos.y / (patchWorldSize.y * (float)numPatchesY) * (float)(numVerticesY - 1);
	int ix = (int)pos.x, iy = (int)pos.y;
	float fx = pos.x - (float)ix, fy = pos.y - (float)iy;
	float i0, i1, i2;
	float z0, z1;
	z0 = fx * fx + fy * fy;
	z1 = (1.0f - fx) * (1.0f - fx) + (1.0f - fy) * (1.0f - fy);
	if (z0 < z1)
	{
		i0 = GetHeightDiscrete(ix, iy);
		i1 = GetHeightDiscrete(ix + 1, iy);
		i2 = GetHeightDiscrete(ix, iy + 1);
		float d0, d1;
		d0 = i1 - i0;
		d1 = i2 - i0;
		return (i0 + d0 * fx + d1 * fy);
	}
	else
	{
		i0 = GetHeightDiscrete(ix + 1, iy);
		i2 = GetHeightDiscrete(ix, iy + 1);
		i1 = GetHeightDiscrete(ix + 1, iy + 1);
		float d0, d1;
		d0 = i2 - i1;
		d1 = i0 - i1;
		return (i1 + d0 * (1.0f - fx) + d1 * (1.0f - fy));
	}
}

/*
vec3 Terrain::GetNormalDiscrete(int x, int y)
{	
	float baseHeight = GetHeightDiscrete(x, y);
	float d0 = GetHeightDiscrete(x, y - 1);
	float d1 = GetHeightDiscrete(x + 1, y - 1);
	float d2 = GetHeightDiscrete(x + 1, y);
	float d3 = GetHeightDiscrete(x + 1, y + 1);
	float d4 = GetHeightDiscrete(x, y + 1);
	float d5 = GetHeightDiscrete(x - 1, y + 1);
	float d6 = GetHeightDiscrete(x - 1, y);
	float d7 = GetHeightDiscrete(x - 1, y - 1);
	float xx = (float) x;
	float yy = (float) y;
	vec3 b((xx + 0) * verticesSpacing.x, baseHeight, (yy + 0) * verticesSpacing.z);
	vec3 v0((xx + 0) * verticesSpacing.x, d0, (yy - 1) * verticesSpacing.z);
	vec3 v1((xx + 1) * verticesSpacing.x, d1, (yy - 1) * verticesSpacing.z);
	vec3 v2((xx + 1) * verticesSpacing.x, d2, (yy + 0) * verticesSpacing.z);
	vec3 v3((xx + 1) * verticesSpacing.x, d3, (yy + 1) * verticesSpacing.z);
	vec3 v4((xx + 0) * verticesSpacing.x, d4, (yy + 1) * verticesSpacing.z);
	vec3 v5((xx - 1) * verticesSpacing.x, d5, (yy + 1) * verticesSpacing.z);
	vec3 v6((xx - 1) * verticesSpacing.x, d6, (yy + 0) * verticesSpacing.z);
	vec3 v7((xx - 1) * verticesSpacing.x, d7, (yy - 1) * verticesSpacing.z);
	vec3 n0 = Cross(v1 - b, v0 - b).Normalized();
	vec3 n1 = Cross(v2 - b, v1 - b).Normalized();
	vec3 n2 = Cross(v3 - b, v2 - b).Normalized();
	vec3 n3 = Cross(v4 - b, v3 - b).Normalized();
	vec3 n4 = Cross(v5 - b, v4 - b).Normalized();
	vec3 n5 = Cross(v6 - b, v5 - b).Normalized();
	vec3 n6 = Cross(v7 - b, v6 - b).Normalized();
	vec3 n7 = Cross(v0 - b, v7 - b).Normalized();
	vec3 n = (n0 + n1 + n2 + n3 + n4 + n5 + n6 + n7).Normalized();
	return n;
}*/


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

vec3 Terrain::GetNormal(vec2 pos)
{
	vec2 offset(patchWorldSize.x * numPatchesX * 0.5f, patchWorldSize.y * numPatchesY * 0.5f);
	pos += offset;
	if ((pos.x < 0) || (pos.x >= patchWorldSize.x * (float)numPatchesX) || (pos.y < 0) || (pos.y >= patchWorldSize.y * (float)numPatchesY))
		return vec3(0, 1, 0);

	pos.x = pos.x / (patchWorldSize.x * (float)numPatchesX) * (float)(numVerticesX - 1);
	pos.y = pos.y / (patchWorldSize.y * (float)numPatchesY) * (float)(numVerticesY - 1);
	int ix = (int)pos.x, iy = (int)pos.y;
	float fx = pos.x - (float)ix, fy = pos.y - (float)iy;
	float i0, i1, i2;
	float z0, z1;
	z0 = fx * fx + fy * fy;
	z1 = (1.0f - fx) * (1.0f - fx) + (1.0f - fy) * (1.0f - fy);
	if (z0 < z1)
	{
		i0 = GetHeightDiscrete(ix, iy);
		i1 = GetHeightDiscrete(ix + 1, iy);
		i2 = GetHeightDiscrete(ix, iy + 1);
		vec3 v0(0, i0, 0);
		vec3 v1(verticesSpacing.x, i1, 0);
		vec3 v2(verticesSpacing.x, i2, verticesSpacing.z);
		return Cross(v2 - v0, v1 - v0).Normalized();
	}
	else
	{
		i0 = GetHeightDiscrete(ix + 1, iy);
		i2 = GetHeightDiscrete(ix, iy + 1);
		i1 = GetHeightDiscrete(ix + 1, iy + 1);
		vec3 v0(verticesSpacing.x, i0, 0);
		vec3 v1(0, i2, verticesSpacing.z);
		vec3 v2(verticesSpacing.x, i1, verticesSpacing.z);
		return Cross(v1 - v0, v2 - v0).Normalized();
	}
}

void Terrain::SetVerticesSpacing(vec3 spacing)
{
	verticesSpacing = spacing;
}

void Terrain::SetPatchSize(int patchSize)
{
	this->patchSize = patchSize;
}

void Terrain::SetMaterial(Material *material)
{
	this->material = material;
	for (auto terrainPatch : patches)
		terrainPatch->SetMaterial(material);
}

void Terrain::SetSmoothHeightMap(bool smoothHeightMap)
{
	this->smoothHeightMap = smoothHeightMap;
}

void Terrain::SmoothHeightMap()
{
	std::vector<float> smoothHeightData(numVerticesX * numVerticesY);
	for (int y = 0; y < numVerticesY; ++y)
	{
		for (int x = 0; x < numVerticesX; ++x)
		{
			smoothHeightData[y * numVerticesX + x] = 
				(GetHeightDiscrete(x - 1, y - 1) + GetHeightDiscrete(x, y - 1) * 2.0f + GetHeightDiscrete(x + 1, y - 1) +
				 GetHeightDiscrete(x - 1, y) * 2.0f + GetHeightDiscrete(x, y) * 4.0f + GetHeightDiscrete(x + 1, y) * 2.0f +
				 GetHeightDiscrete(x - 1, y + 1) + GetHeightDiscrete(x, y + 1) * 2.0f + GetHeightDiscrete(x + 1, y + 1)) / 16.0f;
		}
	}
	heightData = smoothHeightData;
}

void Terrain::SetGenerateHardNormals(bool generateHardNormals)
{
	this->generateHardNormals = generateHardNormals;
}

vec2 Terrain::GetWorldSize() const
{
	return vec2(patchWorldSize.x * numPatchesX, patchWorldSize.y * numPatchesY);
}

TerrainPatch::TerrainPatch(Engine *engine) : Renderable(engine)
{
	renderableParts.resize(1);
	geometry = new Geometry(engine->GetRenderer());
	geometry->SetPrimitiveType(TRIANGLES);
	renderableParts[0].geometry = geometry;
}

TerrainPatch::~TerrainPatch()
{
	geometry->SetIndexBuffer(nullptr); // The index buffer is shared between the terrain patches and has been deleted in the destruction of Terrain
	delete geometry;
}

Geometry *TerrainPatch::GetGeometry()
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

void TerrainPatch::SetMaterial(Material *material)
{
	this->material = material;
	renderableParts[0].material = material;
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