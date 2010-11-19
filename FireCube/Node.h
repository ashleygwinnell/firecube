namespace FireCube
{
class NodeResource;
class FIRECUBE_API RenderParameters
{
public:
	Program program;
	bool lighting;
	bool fog;
	vec4 fogColor;
	float fogDensity;	
};
class FIRECUBE_API Node
{
public:
	Node();
	Node(NodeResource *nr);	
	Node(const std::string &name);	
	operator bool () const;
	bool operator== (const Node &node) const;
	void SetName(const std::string &name);
	std::string GetName();	
	mat4 GetLocalTransformation();	
	mat4 GetWorldTransformation();
	void SetWorldTransformation(mat4 mat);
	void SetTranslation(vec3 t);
	vec3 GetTranslation();
	void SetRotation(vec3 r);
	vec3 GetRotation();
	void SetScale(vec3 s);
	vec3 GetScale();
	void SetMatrixTransformation(mat4 t);
	mat4 GetMatrixTransformation();
	void Move(vec3 t);
	void Rotate(vec3 r);
	void Scale(vec3 s);
	Node &Parent();
	void SetParent(Node parent);
	std::vector<Node> &GetChildren();
	void AddGeometry(Geometry geometry);
	std::vector<Geometry> &GetGeometries();

	void AddLight(Light l);		
	void RemoveLight(Light l);	
	std::vector<Light> &GetLights();	

	Node AddChild(Node node);
	Node GetChild(const std::string &name);
	Node RemoveChild(const std::string &name);
	Node RemoveChild(Node node);	

	void Render();
	void CreateHardNormals();
	void RenderBoundingBox();
	void UpdateBoundingBox();

	RenderParameters &GetRenderParameters();
	void SetProgram(Program program);
	Program GetProgram();
	void SetLighting(bool enabled=true);
	bool GetLighting();
	void SetFog(bool enabled=true);
	bool GetFog();
	void SetFogColor(vec4 color);
	vec4 GetFogColor();
	void SetFogDensity(float density);
	float GetFogDensity();
private:
	boost::shared_ptr<NodeResource> resource;
};
class FIRECUBE_API NodeResource
{
	friend class Node;
public:
	NodeResource();
private:
	std::vector<Node> children;
	std::vector<Geometry> geometries;
	std::vector<Light> lights;
	vec3 translation;
	vec3 rotation;
	vec3 scale;
	mat4 matTransform;
	mat4 transformation;
	mat4 worldTransformation;
	bool transformationChanged;
	Node parent;	
	std::string name;
	RenderParameters renderParameters;
	BoundingBox localBoundingBox;
	BoundingBox worldBoundingBox;
};
Node FIRECUBE_API LoadMesh(const std::string &filename);
}