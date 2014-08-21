#include "Geometry/GeometryGenerator.h"
#include "Geometry/Material.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Core/Engine.h"
#include "Rendering/RenderingTypes.h"

using namespace FireCube;

Geometry *FIRECUBE_API GeometryGenerator::GenerateBox(Engine *engine, const vec3 &size)
{
	Geometry *ret = new Geometry(engine->GetRenderer());
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	IndexBuffer *indexBuffer = new IndexBuffer(engine->GetRenderer());
	ret->SetVertexBuffer(vertexBuffer);
	ret->SetIndexBuffer(indexBuffer);
	vec3 halfSize = size * 0.5f;
	unsigned int currentVertex = 0;
	unsigned int vertexSize = 3 + 3 + 2;
	std::vector<float> vertexData(4 * 6 * vertexSize);
	std::vector<unsigned int> indices(36);
	// Front
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, -halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, 1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, -halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, 1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, 1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 0.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, 1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 0.0f);
	currentVertex++;
	indices[0] = currentVertex - 4;
	indices[1] = currentVertex - 3;
	indices[2] = currentVertex - 2;
	indices[3] = currentVertex - 2;
	indices[4] = currentVertex - 3;
	indices[5] = currentVertex - 1;
	
	// Back
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, -halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, -1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, -halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, -1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, -1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 0.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 0, -1);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 0.0f);
	currentVertex++;
	indices[6] = currentVertex - 4;
	indices[7] = currentVertex - 3;
	indices[8] = currentVertex - 2;
	indices[9] = currentVertex - 2;
	indices[10] = currentVertex - 3;
	indices[11] = currentVertex - 1;

	// Left
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, -halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(-1, 0, 0);
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, -halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(-1, 0, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(-1, 0, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 0.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(-1, 0, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 0.0f);
	currentVertex++;
	indices[12] = currentVertex - 4;
	indices[13] = currentVertex - 3;
	indices[14] = currentVertex - 2;
	indices[15] = currentVertex - 2;
	indices[16] = currentVertex - 3;
	indices[17] = currentVertex - 1;

	// Right
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, -halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(1, 0, 0);
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, -halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(1, 0, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(1, 0, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 0.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(1, 0, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 0.0f);
	currentVertex++;
	indices[18] = currentVertex - 4;
	indices[19] = currentVertex - 3;
	indices[20] = currentVertex - 2;
	indices[21] = currentVertex - 2;
	indices[22] = currentVertex - 3;
	indices[23] = currentVertex - 1;

	// Top
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 0.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 0.0f);
	currentVertex++;
	indices[24] = currentVertex - 4;
	indices[25] = currentVertex - 3;
	indices[26] = currentVertex - 2;
	indices[27] = currentVertex - 2;
	indices[28] = currentVertex - 3;
	indices[29] = currentVertex - 1;

	// Bottom
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, -halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, -1, 0);
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, -halfSize.y, -halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, -1, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 1.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, -halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, -1, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 0.0f);
	currentVertex++;
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, -halfSize.y, halfSize.z);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, -1, 0);	
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 0.0f);
	currentVertex++;
	indices[30] = currentVertex - 4;
	indices[31] = currentVertex - 3;
	indices[32] = currentVertex - 2;
	indices[33] = currentVertex - 2;
	indices[34] = currentVertex - 3;
	indices[35] = currentVertex - 1;
		
	vertexBuffer->LoadData(&vertexData[0], 4 * 6, VertexAttribute::POSITION | VertexAttribute::NORMAL | VertexAttribute::TEXCOORD0, STATIC);	
	indexBuffer->LoadData(&indices[0], indices.size(), STATIC);

	ret->SetPrimitiveType(TRIANGLES);
	ret->SetPrimitiveCount(indices.size() / 3);
	
	ret->Update();
	return ret;
}

Geometry *FIRECUBE_API GeometryGenerator::GenerateSphere(Engine *engine, float radius, unsigned int rings, unsigned int columns)
{
	Geometry *ret = new Geometry(engine->GetRenderer());
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	IndexBuffer *indexBuffer = new IndexBuffer(engine->GetRenderer());
	ret->SetVertexBuffer(vertexBuffer);
	ret->SetIndexBuffer(indexBuffer);
	std::vector<unsigned int> indices;
	unsigned int vertexSize = 3 + 3 + 2;
	unsigned int vertexCount = (rings + 1) * (columns + 1);
	std::vector<float> vertexData(vertexCount * vertexSize);
	unsigned int currentVertex = 0;	
	indices.reserve(2 * rings * (columns + 1));

	const float fDeltaRingAngle = (float)PI / rings;
	const float fDeltacolumnAngle = 2 * (float)PI / columns;

	unsigned int vertexIndex = 0;

	for(unsigned int ring = 0; ring < rings + 1 ; ring++)
	{
		const float r0 = std::sin(ring * fDeltaRingAngle);
		const float y0 = std::cos(ring * fDeltaRingAngle);

		for(unsigned column = 0; column < columns + 1 ; column++)
		{
			vec3 pos(-r0 * std::sin(column * fDeltacolumnAngle), y0, -r0 * std::cos(column * fDeltacolumnAngle));
			vec2 uv(column / (float)columns, ring / (float) rings);

			*((vec3 *) &vertexData[currentVertex * vertexSize + 0]) = pos * radius;
			*((vec3 *) &vertexData[currentVertex * vertexSize + 3]) = pos.Normalized();
			*((vec2 *) &vertexData[currentVertex * vertexSize + 6]) = uv;			
			currentVertex++;
			if(ring < rings)
			{
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + columns + 1);
				++vertexIndex;
			}
		}
	}
	std::vector<unsigned int> indexData(3 + (indices.size() - 3) * 3);
	indexData[0] = indices[0];
	indexData[1] = indices[1];
	indexData[2] = indices[2];	
	for (unsigned int i = 3; i < indices.size(); i++)
	{
		if (i % 2 == 1)
		{			
			indexData[(i - 2) * 3 + 0] = indices[i - 1];
			indexData[(i - 2) * 3 + 1] = indices[i - 2];
			indexData[(i - 2) * 3 + 2] = indices[i];			
		}
		else
		{
			indexData[(i - 2) * 3 + 0] = indices[i - 2];
			indexData[(i - 2) * 3 + 1] = indices[i - 1];
			indexData[(i - 2) * 3 + 2] = indices[i];								
		}
	}
	
	vertexBuffer->LoadData(&vertexData[0], vertexCount, VertexAttribute::POSITION | VertexAttribute::NORMAL | VertexAttribute::TEXCOORD0, STATIC);	
	indexBuffer->LoadData(&indexData[0], indexData.size(), STATIC);

	ret->SetPrimitiveType(TRIANGLES);
	ret->SetPrimitiveCount(indexData.size() / 3);

	ret->Update();
	return ret;
}

Geometry *FIRECUBE_API GeometryGenerator::GeneratePlane(Engine *engine, const vec2 &size)
{
	Geometry *ret = new Geometry(engine->GetRenderer());
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	IndexBuffer *indexBuffer = new IndexBuffer(engine->GetRenderer());
	ret->SetVertexBuffer(vertexBuffer);
	ret->SetIndexBuffer(indexBuffer);
	vec2 halfSize = size * 0.5f;
	unsigned int vertexSize = 3 + 3 + 2;
	unsigned int vertexCount = 4;
	std::vector<float> vertexData(vertexCount * vertexSize);
	std::vector<unsigned int> indexData(6);
	unsigned int currentVertex = 0;
	*((vec3 *) &vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, 0, halfSize.y);
	*((vec3 *) &vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);
	*((vec2 *) &vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 0.0f);
	currentVertex++;
	*((vec3 *) &vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, 0, halfSize.y);
	*((vec3 *) &vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);
	*((vec2 *) &vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 0.0f);
	currentVertex++;
	*((vec3 *) &vertexData[currentVertex * vertexSize + 0]) = vec3(-halfSize.x, 0, -halfSize.y);
	*((vec3 *) &vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);
	*((vec2 *) &vertexData[currentVertex * vertexSize + 6]) = vec2(0.0f, 1.0f);
	currentVertex++;
	*((vec3 *) &vertexData[currentVertex * vertexSize + 0]) = vec3(halfSize.x, 0, -halfSize.y);
	*((vec3 *) &vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);
	*((vec2 *) &vertexData[currentVertex * vertexSize + 6]) = vec2(1.0f, 1.0f);
	currentVertex++;

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 2;
	indexData[4] = 1;
	indexData[5] = 3;
	
	vertexBuffer->LoadData(&vertexData[0], vertexCount, VertexAttribute::POSITION | VertexAttribute::NORMAL | VertexAttribute::TEXCOORD0, STATIC);
	indexBuffer->LoadData(&indexData[0], indexData.size(), STATIC);
	
	ret->SetPrimitiveType(TRIANGLES);
	ret->SetPrimitiveCount(indexData.size() / 3);
	
	ret->Update();
	return ret;
}
