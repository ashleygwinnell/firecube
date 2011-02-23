namespace FireCube
{
class NodeResource;

/**
* A class representing a node in a scene graph.
*/
class FIRECUBE_API Node
{
public:
	Node();

	/**
	* Constructs a node from a node resource.
	* @param nr The resource to assign to this node.
	*/
	Node(NodeResource *nr);	

	/**
	* Constructs a node.
	* @param name The name of this node.
	*/
	Node(const std::string &name);	
	
	/**
	* Sets the name of this node.
	* @param name The name to give to this node.
	*/
	void SetName(const std::string &name);

	/**
	* Gets the name of this node.
	* @return The name of this node.
	*/
	std::string GetName();

	/**
	* Gets the local transformation of this node.
	* @return The local transformation of this node.
	*/
	mat4 GetLocalTransformation();

	/**
	* Gets the world transformation of this node.
	* @return The world transformation of this node.
	*/
	mat4 GetWorldTransformation();
	
	/**
	* Sets the world transformation of this node.
	* @param mat The world transformation to assign to this node.
	*/
	void SetWorldTransformation(mat4 mat);

	/**
	* Sets the translation of this node.
	* @param t The translation to assign to this node.
	*/
	void SetTranslation(vec3 t);

	/**
	* Gets the translation of this node.
	* @return The translation of this node.
	*/
	vec3 GetTranslation();
	
	/**
	* Sets the rotation of this node.
	* @param r The rotation(Euler angles) to assign to this node.
	*/
	void SetRotation(vec3 r);

	/**
	* Gets the rotation of this node.
	* @return The rotation(Euler angles) of this node.
	*/
	vec3 GetRotation();

	/**
	* Sets the scale of this node.
	* @param s The scale to assign to this node.
	*/
	void SetScale(vec3 s);

	/**
	* Gets the scale of this node.
	* @return The scale of this node.
	*/
	vec3 GetScale();

	/**
	* Sets the matrix transformation of this node.
	* @param t The matrix transformation to assign to this node.
	*/
	void SetMatrixTransformation(mat4 t);

	/**
	* Gets the matrix transformation of this node.
	* @return The matrix transformation of this node.
	*/
	mat4 GetMatrixTransformation();

	/**
	* Translates the node.
	* @param t The translation vector.
	*/
	void Move(vec3 t);

	/**
	* Rotates the node.
	* @param r The rotation vector(Euler angles).
	*/
	void Rotate(vec3 r);

	/**
	* Scales the node.
	* @param s The scale vector.
	*/
	void Scale(vec3 s);

	/**
	* Gets the parent of this node.
	* @return The parent of this node.
	*/
	Node &GetParent();

	/**
	* Sets the parent of this node.
	* @param parent The new parent to assign to this node
	*/
	void SetParent(Node parent);

	/**
	* Gets the children of this node.
	* @return A list of children.
	*/
	std::vector<Node> &GetChildren();
	
	/**
	* Adds a geometry to this node.
	* @param geometry The geometry to add.
	*/
	void AddGeometry(Geometry geometry);
	
	/**
	* Gets the geometries of this node.
	* @return A list of geometries.
	*/
	std::vector<Geometry> &GetGeometries();

	/**
	* Adds a light to this node.
	* @param light The light to add.
	*/
	void AddLight(Light light);	

	/**
	* Removes a light from this node.
	* @param light The light to remove.
	*/
	void RemoveLight(Light light);

	/**
	* Gets the lights of this node.
	* @return A list of lights.
	*/
	std::vector<Light> &GetLights();	

	/**
	* Adds a child node.
	* @param node The node to add as a child.
	*/
	Node AddChild(Node node);

	/**
	* Gets a child node by name.
	* @param name The name of the node.
	* @return The child node.
	*/
	Node GetChild(const std::string &name);

	/**
	* Removes a child node.
	* @param name The name of the child node to remove.
	*/
	Node RemoveChild(const std::string &name);

	/**
	* Removes a child node.
	* @param node The child node to remove.
	*/
	Node RemoveChild(Node node);	

	/**
	* Renders the node.
	*/
	void Render();

	/**
	* Creates hard normals for all geometries of the sub tree under this node.
	*/
	void CreateHardNormals();

	/**
	* Renders the bounding boxes of the sub tree under this node.
	*/
	void RenderBoundingBox();

	/**
	* Updates the bounding boxes of the sub tree under this node.
	*/
	void UpdateBoundingBox();

	/**
	* Gets the render parameters of this node.
	* @return The render parameters of this node.
	*/
	RenderParameters &GetRenderParameters();

	/**
	* Sets the program used for rendering of the sub tree under this node.
	* @param program The program to set.
	*/
	void SetProgram(Program program);

	/**
	* Gets the program of this node.
	* @return The program used for rendering of this node.
	*/
	Program GetProgram();

	/**
	* Sets the technique used for rendering of the sub tree under this node.
	* @param name The name of the technique to set.
	*/
	void SetTechnique(const std::string &name);

	/**
	* Gets the technique of this node.
	* @return The technique used for rendering of this node.
	*/
	Technique GetTechnique() const;

	/**
	* Sets whether dynamic lighting is enabled for the sub tree under this node.
	* @param enabled True to enable, false to disable.
	*/
	void SetLighting(bool enabled=true);

	/**
	* Checks whether dynamic lighting is enabled for this node.
	* @return True if enabled, false otherwise.
	*/
	bool GetLighting();
	
	/**
	* Sets whether fog is enabled for the sub tree under this node.
	* @param enabled True to enable, false to disable.
	*/
	void SetFog(bool enabled=true);

	/**
	* Checks whether fog is enabled for this node.
	* @return True if enabled, false otherwise.
	*/
	bool GetFog();

	/**
	* Sets the fog color for the sub tree under this node.
	* @param color The fog color to set.
	*/
	void SetFogColor(vec4 color);

	/**
	* Gets the fog color of this node.
	* @return The fog color.
	*/
	vec4 GetFogColor();

	/**
	* Sets the fog density for the sub tree under this node.
	* @param density The fog density to set.
	*/
	void SetFogDensity(float density);

	/**
	* Gets the fog density of this node.
	* @return The fog density.
	*/
	float GetFogDensity();
	
	/**
	* @return True if points to an initialized resource, false otherwise.
	*/
	operator bool () const;

	/**
	* @return True of both objects points to the same resource, false otherwise.
	*/
	bool operator== (const Node &node) const;
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

/**
* Loads a node from a file.
* Supported file formats are 3ds and Collada.
* @param filename The file to load.
* @return The loaded node.
*/
Node FIRECUBE_API LoadMesh(const std::string &filename);
}