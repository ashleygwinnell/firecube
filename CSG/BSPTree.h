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
	typedef boost::shared_ptr<Node> NodePtr;
	class Node
	{
	public:
		vector<Face> faces;
		Plane plane;
		NodePtr front, back;
	};

	void BuildTree(NodePtr node, vector<Face> &faces);
	vector<Face> ClipFaces(NodePtr node, GeometryPtr pGeometry, vector<Face> &faces);
	void ClipTo(NodePtr node, BSPTree &bsp);
	void ClipTo(BSPTree &bsp);
	void Invert();
	void Invert(NodePtr node);
	NodePtr Clone(NodePtr node);
	void Clone(BSPTree &clone);
	void CopyFacesToGeometry(NodePtr node);
	void Update();
	unsigned int ChooseSplittingPlane(vector<Face> &faces);
	NodePtr root;
	vector<unsigned int> vertexUsage;
	GeometryPtr geometry;
	bool updated;	
};