#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"

#include "utils.h"	
#include "Logger.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"	
#include "BoundingBox.h"
#include "Texture.h"		
#include "Buffer.h"
#include "Shaders.h"
#include "Geometry.h"	
#include "FrameBuffer.h"
#include "Image.h"
#include "Font.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "Application.h"
#include "privateFont.h"
#include "Light.h"
#include "Node.h"
#include "GeometryGenerator.h"

using namespace FireCube;

Material CreateDefaultMaterial()
{
	Material material;
	material.Create();
	material.SetName("Default");
	material.SetAmbientColor(vec4(0.3f,0.3f,0.3f,1.0f));
	material.SetDiffuseColor(vec4(0.7f,0.7f,0.7f,1.0f));
	material.SetSpecularColor(vec4(0.2f,0.2f,0.2f,1.0f));
	material.SetShininess(50.0f);
	return material;
}

Geometry FIRECUBE_API GeometryGenerator::GenerateBox(vec3 size,Material material)
{
	Geometry ret;
	ret.Create();
	size=size*0.5f;

	// Front
	ret.GetVertices().push_back(vec3(-size.x,-size.y,size.z));
	ret.GetVertices().push_back(vec3(size.x,-size.y,size.z));
	ret.GetVertices().push_back(vec3(-size.x,size.y,size.z));
	ret.GetVertices().push_back(vec3(size.x,size.y,size.z));
	ret.GetNormals().push_back(vec3(0,0,1));
	ret.GetNormals().push_back(vec3(0,0,1));
	ret.GetNormals().push_back(vec3(0,0,1));
	ret.GetNormals().push_back(vec3(0,0,1));
	ret.GetDiffuseUV().push_back(vec2(0.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(0.0f,0.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,0.0f));
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-4,ret.GetVertices().size()-3,ret.GetVertices().size()-2));
	ret.GetFaces().back().normal=vec3(0,0,1);
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-2,ret.GetVertices().size()-3,ret.GetVertices().size()-1));
	ret.GetFaces().back().normal=vec3(0,0,1);	
	// Back
	ret.GetVertices().push_back(vec3(size.x,-size.y,-size.z));
	ret.GetVertices().push_back(vec3(-size.x,-size.y,-size.z));
	ret.GetVertices().push_back(vec3(size.x,size.y,-size.z));
	ret.GetVertices().push_back(vec3(-size.x,size.y,-size.z));
	ret.GetNormals().push_back(vec3(0,0,-1));
	ret.GetNormals().push_back(vec3(0,0,-1));
	ret.GetNormals().push_back(vec3(0,0,-1));
	ret.GetNormals().push_back(vec3(0,0,-1));
	ret.GetDiffuseUV().push_back(vec2(0.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(0.0f,0.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,0.0f));
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-4,ret.GetVertices().size()-3,ret.GetVertices().size()-2));
	ret.GetFaces().back().normal=vec3(0,0,-1);
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-2,ret.GetVertices().size()-3,ret.GetVertices().size()-1));
	ret.GetFaces().back().normal=vec3(0,0,-1);
	// Left
	ret.GetVertices().push_back(vec3(-size.x,-size.y,-size.z));
	ret.GetVertices().push_back(vec3(-size.x,-size.y,size.z));
	ret.GetVertices().push_back(vec3(-size.x,size.y,-size.z));
	ret.GetVertices().push_back(vec3(-size.x,size.y,size.z));
	ret.GetNormals().push_back(vec3(-1,0,0));
	ret.GetNormals().push_back(vec3(-1,0,0));
	ret.GetNormals().push_back(vec3(-1,0,0));
	ret.GetNormals().push_back(vec3(-1,0,0));
	ret.GetDiffuseUV().push_back(vec2(0.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(0.0f,0.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,0.0f));
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-4,ret.GetVertices().size()-3,ret.GetVertices().size()-2));
	ret.GetFaces().back().normal=vec3(-1,0,0);
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-2,ret.GetVertices().size()-3,ret.GetVertices().size()-1));
	ret.GetFaces().back().normal=vec3(-1,0,0);
	// Right
	ret.GetVertices().push_back(vec3(size.x,-size.y,size.z));
	ret.GetVertices().push_back(vec3(size.x,-size.y,-size.z));
	ret.GetVertices().push_back(vec3(size.x,size.y,size.z));
	ret.GetVertices().push_back(vec3(size.x,size.y,-size.z));
	ret.GetNormals().push_back(vec3(1,0,0));
	ret.GetNormals().push_back(vec3(1,0,0));
	ret.GetNormals().push_back(vec3(1,0,0));
	ret.GetNormals().push_back(vec3(1,0,0));
	ret.GetDiffuseUV().push_back(vec2(0.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(0.0f,0.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,0.0f));
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-4,ret.GetVertices().size()-3,ret.GetVertices().size()-2));
	ret.GetFaces().back().normal=vec3(1,0,0);
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-2,ret.GetVertices().size()-3,ret.GetVertices().size()-1));
	ret.GetFaces().back().normal=vec3(1,0,0);
	// Top
	ret.GetVertices().push_back(vec3(-size.x,size.y,size.z));
	ret.GetVertices().push_back(vec3(size.x,size.y,size.z));
	ret.GetVertices().push_back(vec3(-size.x,size.y,-size.z));
	ret.GetVertices().push_back(vec3(size.x,size.y,-size.z));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetDiffuseUV().push_back(vec2(0.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(0.0f,0.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,0.0f));
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-4,ret.GetVertices().size()-3,ret.GetVertices().size()-2));
	ret.GetFaces().back().normal=vec3(0,1,0);
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-2,ret.GetVertices().size()-3,ret.GetVertices().size()-1));
	ret.GetFaces().back().normal=vec3(0,1,0);	
	// Bottom
	ret.GetVertices().push_back(vec3(-size.x,-size.y,-size.z));
	ret.GetVertices().push_back(vec3(size.x,-size.y,-size.z));
	ret.GetVertices().push_back(vec3(-size.x,-size.y,size.z));
	ret.GetVertices().push_back(vec3(size.x,-size.y,size.z));
	ret.GetNormals().push_back(vec3(0,-1,0));
	ret.GetNormals().push_back(vec3(0,-1,0));
	ret.GetNormals().push_back(vec3(0,-1,0));
	ret.GetNormals().push_back(vec3(0,-1,0));
	ret.GetDiffuseUV().push_back(vec2(0.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(0.0f,0.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,0.0f));
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-4,ret.GetVertices().size()-3,ret.GetVertices().size()-2));
	ret.GetFaces().back().normal=vec3(0,-1,0);
	ret.GetFaces().push_back(Face(ret.GetVertices().size()-2,ret.GetVertices().size()-3,ret.GetVertices().size()-1));
	ret.GetFaces().back().normal=vec3(0,-1,0);
	
	ret.GetSurfaces().push_back(Surface());
	ret.GetSurfaces().back().face=ret.GetFaces();
	ret.GetSurfaces().back().material=material;
	ret.GetMaterials().push_back(material);
	ret.UpdateBuffers();
	return ret;
}
Geometry FIRECUBE_API GeometryGenerator::GenerateBox(vec3 size)
{
	return GenerateBox(size,CreateDefaultMaterial());
}

Geometry FIRECUBE_API GeometryGenerator::GenerateSphere (float radius,unsigned int rings, unsigned int columns,Material material) 
{
	Geometry ret;
	ret.Create();
	vector<unsigned int> indices;

	ret.GetVertices().reserve((rings + 1) * (columns + 1));
	ret.GetNormals().reserve((rings + 1) * (columns + 1));
	ret.GetDiffuseUV().reserve((rings + 1) * (columns + 1));
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

			ret.GetVertices().push_back(pos * radius);
			ret.GetNormals().push_back(pos.Normalize());
			ret.GetDiffuseUV().push_back(uv);
			if(ring < rings)
			{
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + columns + 1);
				++vertexIndex;
			}
		}
	}
	ret.GetFaces().push_back(Face(indices[0],indices[1],indices[2]));
	for (unsigned int i=3;i<indices.size();i++)
	{
		if (i%2==1)
		{		
			Face f(indices[i-1],indices[i-2],indices[i]);
			ret.GetFaces().push_back(f);
		}
		else
		{
			Face f(indices[i-2],indices[i-1],indices[i]);
			ret.GetFaces().push_back(f);
		}
	}
	ret.GetSurfaces().push_back(Surface());	
	ret.GetSurfaces().back().face=ret.GetFaces();
	ret.GetSurfaces().back().material=material;
	ret.GetMaterials().push_back(material);
	ret.UpdateBuffers();
	return ret;
}
Geometry FIRECUBE_API GeometryGenerator::GenerateSphere (float radius,unsigned int rings, unsigned int columns) 
{
	return GenerateSphere(radius,rings,columns,CreateDefaultMaterial());
}
Geometry FIRECUBE_API GeometryGenerator::GeneratePlane(vec2 size)
{
	return GeneratePlane(size,CreateDefaultMaterial());
}
Geometry FIRECUBE_API GeometryGenerator::GeneratePlane(vec2 size,Material material)
{
	Geometry ret;
	ret.Create();
	size=size*0.5f;

	ret.GetVertices().push_back(vec3(-size.x,0,size.y));
	ret.GetVertices().push_back(vec3(size.x,0,size.y));
	ret.GetVertices().push_back(vec3(-size.x,0,-size.y));
	ret.GetVertices().push_back(vec3(size.x,0,-size.y));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetNormals().push_back(vec3(0,1,0));
	ret.GetDiffuseUV().push_back(vec2(0.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,1.0f));
	ret.GetDiffuseUV().push_back(vec2(0.0f,0.0f));
	ret.GetDiffuseUV().push_back(vec2(1.0f,0.0f));
	ret.GetFaces().push_back(Face(0,1,2));
	ret.GetFaces().back().normal=vec3(0,1,0);
	ret.GetFaces().push_back(Face(2,1,3));
	ret.GetFaces().back().normal=vec3(0,1,0);		

	ret.GetSurfaces().push_back(Surface());
	ret.GetSurfaces().back().face=ret.GetFaces();
	ret.GetSurfaces().back().material=material;
	ret.GetMaterials().push_back(material);
	ret.UpdateBuffers();
	return ret;
}