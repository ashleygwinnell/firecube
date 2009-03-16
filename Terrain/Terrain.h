#ifndef TERRAIN_H
#define TERRAIN_H
class Terrain
{
public:
	Terrain();			
	bool GenerateTerrain(const string &heightmap,const string &diffuse,vec3 sizeVertices,vec2 sizeUv);
	DWORD Render(Frustum &frustum);
	float GetHeight(vec2 pos);
	int GetWidth();
	int GetHeight();
	vec3 GetNormal(vec2 pos);
private:
	QuadTree quadtree;
	Material material;
	Texture diffuseTexture;
	vec3 terrainScale;
	Image heightmapImage;
	Buffer vertexBuffer;	
	Buffer uvBuffer;
	Buffer normalBuffer;	
	DWORD width,height;
};
#endif