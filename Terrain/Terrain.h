#ifndef TERRAIN_H
#define TERRAIN_H
class Terrain
{
public:
    Terrain();
    bool GenerateTerrain(const string &heightmap, const string &diffuse, vec3 sizeVertices, vec2 sizeUv);
    unsigned int Render(Frustum &frustum);
    float GetHeight(vec2 pos);
    int GetWidth();
    int GetHeight();
    vec3 GetNormal(vec2 pos);
private:
    QuadTree quadtree;
    MaterialPtr material;
    ProgramPtr program;
    TexturePtr diffuseTexture;
    vec3 terrainScale;
    Image heightmapImage;
    BufferPtr vertexBuffer;
    BufferPtr uvBuffer;
    BufferPtr normalBuffer;
    unsigned int width, height;
};
#endif