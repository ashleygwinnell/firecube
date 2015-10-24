#include "Geometry/GeometryGenerator.h"
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
	vertexBuffer->SetShadowed(true);
	indexBuffer->SetShadowed(true);
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
		
	vertexBuffer->LoadData(&vertexData[0], 4 * 6, VertexAttributeType::POSITION | VertexAttributeType::NORMAL | VertexAttributeType::TEXCOORD0, BufferType::STATIC);	
	indexBuffer->LoadData(&indices[0], indices.size(), BufferType::STATIC);

	ret->SetPrimitiveType(PrimitiveType::TRIANGLES);
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
	vertexBuffer->SetShadowed(true);
	indexBuffer->SetShadowed(true);
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
	
	vertexBuffer->LoadData(&vertexData[0], vertexCount, VertexAttributeType::POSITION | VertexAttributeType::NORMAL | VertexAttributeType::TEXCOORD0, BufferType::STATIC);
	indexBuffer->LoadData(&indexData[0], indexData.size(), BufferType::STATIC);

	ret->SetPrimitiveType(PrimitiveType::TRIANGLES);
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
	vertexBuffer->SetShadowed(true);
	indexBuffer->SetShadowed(true);
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
	
	vertexBuffer->LoadData(&vertexData[0], vertexCount, VertexAttributeType::POSITION | VertexAttributeType::NORMAL | VertexAttributeType::TEXCOORD0, BufferType::STATIC);
	indexBuffer->LoadData(&indexData[0], indexData.size(), BufferType::STATIC);
	
	ret->SetPrimitiveType(PrimitiveType::TRIANGLES);
	ret->SetPrimitiveCount(indexData.size() / 3);
	
	ret->Update();
	return ret;
}

Geometry *GeometryGenerator::GenerateTorus(Engine *engine, float outerRadius, float innerRadius, float endAngle, unsigned int segments, unsigned int tubeSegments)
{
	Geometry *ret = new Geometry(engine->GetRenderer());
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	IndexBuffer *indexBuffer = new IndexBuffer(engine->GetRenderer());
	ret->SetVertexBuffer(vertexBuffer);
	ret->SetIndexBuffer(indexBuffer);
	vertexBuffer->SetShadowed(true);
	indexBuffer->SetShadowed(true);
	
	unsigned int vertexSize = 3 + 3 + 2;
	unsigned int vertexCount = (segments + 1) * (tubeSegments + 1);
	std::vector<float> vertexData(vertexCount * vertexSize);
	std::vector<unsigned int> indexData(segments * tubeSegments * 6);
	unsigned int currentVertex = 0;

	for (unsigned int j = 0; j <= segments; j++) 
	{
		for (unsigned int i = 0; i <= tubeSegments; i++) 
		{
			float u = (float) i / (float) tubeSegments * endAngle;
			float v = (float) j / (float) segments * PI * 2.0f;

			vec3 center(cos(u) * outerRadius, sin(u) * outerRadius, 0);
						
			vec3 point((outerRadius + innerRadius * cos(v)) * cos(u),
			(outerRadius + innerRadius * cos(v)) * sin(u),
				innerRadius * sin(v));

			*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = point;
			*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = (point - center).Normalized();
			*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2((float)i / (float)tubeSegments, (float)j / (float)segments);
			currentVertex++;
		}
	}

	unsigned int currentIndex = 0;

	for (unsigned int j = 1; j <= segments; j++) 
	{
		for (unsigned int i = 1; i <= tubeSegments; i++) 
		{
			unsigned int a = (tubeSegments + 1) * j + i - 1;
			unsigned int b = (tubeSegments + 1) * (j - 1) + i - 1;
			unsigned int c = (tubeSegments + 1) * (j - 1) + i;
			unsigned int d = (tubeSegments + 1) * j + i;

			indexData[currentIndex++] = a;
			indexData[currentIndex++] = b;
			indexData[currentIndex++] = d;
			indexData[currentIndex++] = b;
			indexData[currentIndex++] = c;
			indexData[currentIndex++] = d;			
		}
	}

	vertexBuffer->LoadData(&vertexData[0], vertexCount, VertexAttributeType::POSITION | VertexAttributeType::NORMAL | VertexAttributeType::TEXCOORD0, BufferType::STATIC);
	indexBuffer->LoadData(&indexData[0], indexData.size(), BufferType::STATIC);

	ret->SetPrimitiveType(PrimitiveType::TRIANGLES);
	ret->SetPrimitiveCount(indexData.size() / 3);

	ret->Update();
	return ret;
}

Geometry *GeometryGenerator::GenerateCylinder(Engine *engine, float bottomRadius, float topRadius, float height, unsigned int heightSegments, unsigned int radialSegments)
{
	Geometry *ret = new Geometry(engine->GetRenderer());
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	IndexBuffer *indexBuffer = new IndexBuffer(engine->GetRenderer());
	ret->SetVertexBuffer(vertexBuffer);
	ret->SetIndexBuffer(indexBuffer);
	vertexBuffer->SetShadowed(true);
	indexBuffer->SetShadowed(true);

	unsigned int vertexSize = 3 + 3 + 2;
	unsigned int vertexCount = (radialSegments * heightSegments) + 2;
	std::vector<float> vertexData(vertexCount * vertexSize);
	std::vector<unsigned int> indexData(radialSegments * (heightSegments - 1) * 6 + 2 * radialSegments * 3);
	unsigned int currentVertex = 0;
	float deltaAngle = PI * 2.0f / radialSegments;
	for (unsigned int y = 0; y < heightSegments; ++y)
	{
		float v = (float) y / (float) (heightSegments - 1);
		for (unsigned int x = 0; x < radialSegments; ++x)
		{
			float u = (float)x / (float) radialSegments;
			float ang = u * PI * 2.0f;
			float radius = (1 - v) * bottomRadius + v * topRadius;
			vec3 pos(cos(ang) * radius, v * height, sin(ang) * radius);
			vec3 normal = (pos - vec3(0, v * height, 0)).Normalized();
						
			*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = pos;
			*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = normal;
			*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(u, v);
			currentVertex++;
		}
	}

	// Bottom vertex
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(0, 0, 0);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, -1, 0);
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.5f, 0.5f);
	currentVertex++;

	// Top vertex
	*((vec3 *)&vertexData[currentVertex * vertexSize + 0]) = vec3(0, height, 0);
	*((vec3 *)&vertexData[currentVertex * vertexSize + 3]) = vec3(0, 1, 0);
	*((vec2 *)&vertexData[currentVertex * vertexSize + 6]) = vec2(0.5f, 0.5f);
	currentVertex++;

	unsigned int currentIndex = 0;

	for (unsigned int y = 0; y < heightSegments - 1; ++y)
	{
		for (unsigned int x = 0; x < radialSegments; ++x)
		{
			unsigned int v00 = y * radialSegments + x;
			unsigned int v10 = y * radialSegments + ((x + 1) % radialSegments);
			unsigned int v01 = (y + 1) * radialSegments + x;
			unsigned int v11 = (y + 1) * radialSegments + ((x + 1) % radialSegments);

			indexData[currentIndex++] = v00;
			indexData[currentIndex++] = v11;
			indexData[currentIndex++] = v10;
			indexData[currentIndex++] = v00;
			indexData[currentIndex++] = v01;
			indexData[currentIndex++] = v11;
		}
	}

	unsigned int bottomCapPoint = vertexCount - 2;	

	for (unsigned int x = 0; x < radialSegments; ++x)
	{
		unsigned int v00 = x;
		unsigned int v10 = ((x + 1) % radialSegments);
		
		indexData[currentIndex++] = v00;
		indexData[currentIndex++] = v10;
		indexData[currentIndex++] = bottomCapPoint;		
	}

	unsigned int topCapPoint = vertexCount - 1;

	for (unsigned int x = 0; x < radialSegments; ++x)
	{
		unsigned int v00 = (heightSegments - 1) * radialSegments + x;
		unsigned int v10 = (heightSegments - 1) * radialSegments + ((x + 1) % radialSegments);

		indexData[currentIndex++] = v00;
		indexData[currentIndex++] = topCapPoint;
		indexData[currentIndex++] = v10;
	}

	vertexBuffer->LoadData(&vertexData[0], vertexCount, VertexAttributeType::POSITION | VertexAttributeType::NORMAL | VertexAttributeType::TEXCOORD0, BufferType::STATIC);
	indexBuffer->LoadData(&indexData[0], indexData.size(), BufferType::STATIC);

	ret->SetPrimitiveType(PrimitiveType::TRIANGLES);
	ret->SetPrimitiveCount(indexData.size() / 3);

	ret->Update();
	return ret;
}