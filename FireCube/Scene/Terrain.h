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
	OBJECT(TerrainPatch)
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

class Terrain : public Component
{
	OBJECT(Terrain)
public:
	Terrain(Engine *engine);
	~Terrain();
	void CreateFromHeightMap(Image *image);	
	void SetVerticesSpacing(vec3 spacing);
	void SetPatchSize(int patchSize);
	virtual void MarkedDirty() {}
	void SetMaterial(Material *material);
	void SetSmoothHeightMap(bool smoothHeightMap);
	void SetGenerateHardNormals(bool generateHardNormals);
	float GetHeight(vec2 pos);
	vec3 GetNormal(vec2 pos);
	vec2 GetWorldSize() const;
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
};

/*class Camera;
typedef std::shared_ptr<Camera> CameraPtr;
class Geometry;
typedef std::shared_ptr<Geometry> GeometryPtr;
class Engine;

class FIRECUBE_API Terrain : public Object
{	
public:
	Terrain(Engine *engine);
	bool GenerateTerrain(const std::string &heightmap, vec3 sizeVertices, vec2 sizeUv, bool smoothNormals);
	void Render(CameraPtr camera);
	float GetHeight(vec2 pos);
	int GetWidth();
	int GetLength();
	vec3 GetNormal(vec2 pos);
	GeometryPtr GetGeometry();
	BoundingBox GetBoundingBox() const;
private:
	class QuadTree
	{
	public:
		class Node;
		typedef std::shared_ptr<Node> NodePtr;
		class Node
		{
		public:
			typedef std::vector<vec2> FaceList;
			typedef std::shared_ptr<FaceList> FaceListPtr;		
			BoundingBox boundingBox;
			FaceListPtr face;
			std::vector<unsigned int> indices;
			NodePtr child[4];
		};
		QuadTree();		
		void Init(vec2 size, vec2 verticesSize);
		void Build(float minSize, unsigned int maxNumberOfFaces);
		unsigned int Render(CameraPtr camera, std::vector<unsigned int> &indicesToRender);
		void RenderLines();
		void Save(const std::string &filename);
		void Load(const std::string &filename);
	private:
		void Load(const std::vector<unsigned char> &buffer, unsigned int &currentIndex, NodePtr node);
		void Save(NodePtr node, std::ofstream &file);
		void Build(NodePtr node, float minSize, unsigned int maxNumberOfFaces);
		void BuildIndices(NodePtr node);
		void Render(NodePtr node, CameraPtr camera, std::vector<unsigned int> &indicesToRender);
		void RenderLines(NodePtr node);
		NodePtr root;
		vec2 size;		
		vec2 aspect;
		unsigned int currentIndex;	
	};

	GeometryPtr geometry;
	QuadTree quadtree;
	vec3 terrainScale;
	Image heightmapImage;
	unsigned int width, length;
	BoundingBox boundingBox;
};*/

}
