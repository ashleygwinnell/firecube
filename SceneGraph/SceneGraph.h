class NodeResource;
class SceneGraph;
typedef boost::shared_ptr<NodeResource> Node;
class NodeResource
{
	friend class SceneGraph;
public:
	NodeResource(const string &name,SceneGraph *sceneGraph,const string &parent);
	mat4 GetTransformation();
	void SetTranslation(vec3 t);
	vec3 GetTranslation();
	void SetRotation(vec3 r);
	vec3 GetRotation();
	void SetScale(vec3 s);
	vec3 GetScale();
	void Move(vec3 t);
	void Rotate(vec3 r);
	void Scale(vec3 s);
	void AttachModel(Model model);
	void AttachModel(const string &name);	
	void DetachModel(int i);
	void DetachModel(const string &name);
	
	void AttachLight(const string &name);		
	void DetachLight(const string &name);

	void Render();
	Node AddChild(const string &name);
	Node GetChild(const string &name);
	Node RemoveChild(const string &name);
	const string &GetName() const;
private:
	vector<Node> children;
	vector<Model> attachedModels;
	vector<Light> attachedLights;
	vec3 translation;
	vec3 rotation;
	vec3 scale;
	mat4 transformation;
	mat4 worldTransformation;
	bool transformationChanged;
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
	Node GetNode(const string &name);
	Model AddModel(const string &name,const string &filename);
	Light AddLight(const string &name);
	Model GetModel(const string &name);
	Light GetLight(const string &name);
private:
	ShaderGenerator shaderGenerator;
	vector<pair<mat4,Model>> renderingQueue;
	vector<pair<mat4,Light>> activeLights;
	map<string,Node> nodes;
	map<string,Model> models;
	map<string,Light> lights;
	ModelManager modelManager;
	Node root;
	bool lighting,fog;
};