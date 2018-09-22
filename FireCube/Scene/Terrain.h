#pragma once

#include "Core/Component.h"
#include "Scene/Renderable.h"
#include "Geometry/CollisionQuery.h"

namespace FireCube
{

class Engine;
class Image;
class Terrain;
class IndexBuffer;

class TerrainPatch : public Renderable
{
	FIRECUBE_OBJECT(TerrainPatch)
	friend class Terrain;
public:
	TerrainPatch(Engine *engine);
	~TerrainPatch();
	Geometry *GetGeometry();
	void SetMaterial(Material *material);
	virtual void IntersectRay(RayQuery &rayQuery);
	virtual Component *Clone() const;
private:
	void SetBoundingBox(BoundingBox boundingBox);
	virtual void UpdateWorldBoundingBox();

	Geometry *geometry;
	Material *material;
	BoundingBox boundingBox;
};

/**
* A class representing a terrain. The terrain is loaded from an heightmap
*/
class Terrain : public Component
{
	FIRECUBE_OBJECT(Terrain)
public:
	Terrain(Engine *engine);
	~Terrain();
	
	/**
	* Creates the terrain from a height map
	* @param image An image containing a gray scale heightmap. The dimensions of the image minus 1 must be divisible by the patch size
	*/
	void CreateFromHeightMap(Image *image);	
	
	/**
	* Controls the sizing of the terrain (how pixels in the height map are mapped to positions)
	* @param spacing The spacing of the pixels. x, z determine the spacing between each pixel in the height map. y controls the height when the height map pixels are white
	*/
	void SetVerticesSpacing(vec3 spacing);
	
	/**
	* Sets the patch size to use. The terrain is broken into rectangular patches of this size.
	* @param patchSize The patch size to use
	*/
	void SetPatchSize(int patchSize);
	
	/**
	* Sets the material to use for rendering the terrain
	* @param material The material to use
	*/	
	void SetMaterial(Material *material);
	
	/**
	* Controls whether to apply a smoothing filter on the height map
	* @param smoothHeightMap Whether to smooth the height map
	*/
	void SetSmoothHeightMap(bool smoothHeightMap);
	
	/**
	* Controls whether to generate hard normals for the height map (smooth normals are generated otherwise)
	* @param generateHardNormals Whether to generate hard normals
	*/
	void SetGenerateHardNormals(bool generateHardNormals);
	
	/**
	* Returns the height of a given point on the height map. The point is given in world space with the origin being the center of the height map
	* @param pos The point to query its height
	*/
	float GetHeight(vec2 pos);
	
	/**
	* Returns the normal of the terrain of a given point. The point is given in world space with the origin being the center of the height map
	* @param pos The point to query its normal
	*/
	vec3 GetNormal(vec2 pos);
	
	/**
	* @returns the world size of the terrain along its width and depth	
	*/
	vec2 GetWorldSize() const;

	void SetCollisionQueryMask(unsigned int collisionQueryMask);
	unsigned int GetCollisionQueryMask() const;
	void SetCastShadow(bool castShadow);
	bool GetCastShadow() const;
	void SetLightMask(unsigned int lightMask);
	unsigned int GetLightMask() const;
	
	/**
	* Clones the terrain
	*/
	virtual Component *Clone() const;
private:

	Terrain(const Terrain &other);

	void CreatePatches();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene) {}
	void GeneratePatchGeometry(TerrainPatch *patch, int patchX, int patchY);
	float GetHeightDiscrete(int x, int y);
	vec3 GetNormalDiscrete(int x, int y);
	void GenerateIndexBuffer();
	void SmoothHeightMap();
	virtual void MarkedDirty() {}
	void HeightmapReloaded(Resource *image);
	
	std::vector<float> heightData;
	int patchSize;
	int numVerticesX, numVerticesY;
	int numPatchesX, numPatchesY;
	vec2 patchWorldSize;
	vec3 verticesSpacing;
	IndexBuffer *indexBuffer;
	Material *material;
	std::vector<TerrainPatch *> patches;
	bool smoothHeightMap;
	bool generateHardNormals;
	bool patchesCreated;
	unsigned int collisionQueryMask;
	unsigned int lightMask;
	bool castShadow;
};

}
