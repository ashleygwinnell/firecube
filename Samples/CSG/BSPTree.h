class BSPTree
{
public:
	BSPTree();
	void FromGeometry(GeometryPtr geometry);
	void Union(BSPTree &other, BSPTree &ret1, BSPTree &ret2);
	void Subtract(BSPTree &other, BSPTree &ret1, BSPTree &ret2);
	void Intersect(BSPTree &other, BSPTree &ret1, BSPTree &ret2);
	GeometryPtr GetGeometry();	
private:
	class Node;
	typedef std::shared_ptr<Node> NodePtr;
	class Node
	{
	public:
		std::vector<Face> faces;
		Plane plane;
		NodePtr front, back;
	};

	void BuildTree(NodePtr node, std::vector<Face> &faces);
	std::vector<Face> ClipFaces(NodePtr node, GeometryPtr pGeometry, std::vector<Face> &faces);
	void ClipTo(NodePtr node, BSPTree &bsp);
	void ClipTo(BSPTree &bsp);
	void Invert();
	void Invert(NodePtr node);
	NodePtr Clone(NodePtr node);
	void Clone(BSPTree &clone);
	void CopyFacesToGeometry(NodePtr node);
	void Update();
	unsigned int ChooseSplittingPlane(std::vector<Face> &faces);
	NodePtr root;
	std::vector<unsigned int> vertexUsage;
	GeometryPtr geometry;
	bool updated;	
};