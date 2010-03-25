class NodeResource;
class SceneGraph;
typedef boost::shared_ptr<NodeResource> Node;
class NodeResource
{
	friend class SceneGraph;
public:
	NodeResource(const string &name,SceneGraph *sceneGraph,NodeResource *parent);
	mat4 &GetTransformation();
	void SetTransformation(const mat4 &t);
	void AttachModel(Model model);
	void DetachModel(int i);
	void Render();
	Node AddChild(const string &name,const mat4 &t);
	Node GetChild(const string &name);
	const string &GetName() const;
private:
	vector<Node> children;
	vector<Model> attachedModels;
	mat4 transformation;
	Node parent;
	SceneGraph *sceneGraph;
	string name;
};
class SceneGraph
{
	friend class NodeResource;
public:
	SceneGraph();
	void Render();
	Node Root();
	Node GetNode(NodeResource *node);
	Node GetNode(const string &name);
	Model AddModel(const string &name,const string &filename);
	Model GetModel(const string &name);
private:	
	map<string,Node> nodes;
	map<string,Model> models;
	ModelManager modelManager;
	Node root;
};