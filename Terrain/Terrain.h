#ifndef TERRAIN_H
#define TERRAIN_H
class Terrain
{
public:
	Terrain();
	bool GenerateTerrain(const string &heightmap,const string &texture,const string &detailMap,vec3 sizeVertices,vec2 sizeUv);
	bool GenerateTerrain(const string &heightmap,vec3 sizeVertices,vec2 sizeUv);
	void Render();
	float GetHeight(float x,float y);
private:
	vec3 terrainScale;
	Image heightmapImage;
	Texture heightmap;
	Texture terrainTexture;
	Texture detailMap;
	Buffer vertexBuffer;
	Buffer indexBuffer;
	Buffer uvBuffer;
	Buffer normalBuffer;
	Program program;
	DWORD width,height;
};
#endif