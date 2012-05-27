#ifndef TERRAIN_H
#define TERRAIN_H
namespace FireCube
{

class FIRECUBE_API Terrain
{
public:
	Terrain();
	bool GenerateTerrain(const std::string &heightmap, vec3 sizeVertices, vec2 sizeUv, bool smoothNormals);
	unsigned int Render(CameraPtr camera);
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
		void Initialize();
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
};

}
#endif