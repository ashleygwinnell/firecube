#ifndef NODE_H
#define NODE_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
class Node;
class RenderQueue;

/**
* A shared pointer to a Node.
*/
typedef std::shared_ptr<Node> NodePtr;
class Geometry;
typedef std::shared_ptr<Geometry> GeometryPtr;
class Light;
class Material;
typedef std::shared_ptr<Material> MaterialPtr;
class Program;
typedef std::shared_ptr<Program> ProgramPtr;

/**
* A class describing various parameters to apply to models when loaded from a file
*/
class FIRECUBE_API ModelLoadingOptions
{
public:
	ModelLoadingOptions();
	
	/**
	* Boolean flag specifying whether to flip u texture coordinate.
	*/
	bool flipU;

	/**
	* Boolean flag specifying whether to flip v texture coordinate.
	*/
	bool flipV;
};

/**
* A class representing a node in a scene graph.
*/
class FIRECUBE_API Node : public std::enable_shared_from_this<Node>
{
	friend class RenderQueue;
public:
	enum NodeType
	{
		NODE, GEOMETRY, LIGHT, TERRAIN
	};
	Node();

	/**
	* Constructs a node.
	* @param name The name of this node.
	*/
	Node(const std::string &name);

	/**
	* Gets the type of this node.
	* @returns The type of this node.
	*/
	NodeType GetType() const;

	/**
	* Sets the name of this node.
	* @param name The name to give to this node.
	*/
	void SetName(const std::string &name);

	/**
	* Gets the name of this node.
	* @return The name of this node.
	*/
	std::string GetName() const;

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
	* Sets the translation of this node.
	* @param t The translation to assign to this node.
	*/
	void SetTranslation(const vec3 &t);

	/**
	* Gets the translation of this node.
	* @return The translation of this node.
	*/
	vec3 GetTranslation() const;

	/**
	* Sets the rotation of this node.
	* @param r The rotation matrix to assign to this node.
	*/
	void SetRotation(const mat4 &r);

	/**
	* Gets the rotation of this node.
	* @return The rotation matrix of this node.
	*/
	mat4 GetRotation() const;

	/**
	* Sets the scale of this node.
	* @param s The scale to assign to this node.
	*/
	void SetScale(const vec3 &s);

	/**
	* Gets the scale of this node.
	* @return The scale of this node.
	*/
	vec3 GetScale() const;

	/**
	* Translates the node.
	* @param t The translation vector.
	*/
	void Move(const vec3 &t);

	/**
	* Rotates the node.
	* @param r The rotation vector(Euler angles).
	*/
	void Rotate(const vec3 &r);

	/**
	* Scales the node.
	* @param s The scale vector.
	*/
	void Scale(const vec3 &s);
	/**
	* Make the node look at a specific point.
	* @param position The new position of the node;
	* @param at The look at position;
	* @param up The up vector;
	*/
	void LookAt(vec3 position, vec3 at, vec3 up);

	/**
	* Gets the parent of this node.
	* @return The parent of this node.
	*/
	Node *GetParent();

	/**
	* Sets the parent of this node.
	* @param parent The new parent to assign to this node
	*/
	void SetParent(NodePtr parent);

	/**
	* Gets the children of this node.
	* @return A list of children.
	*/
	std::vector<NodePtr> &GetChildren();
	
	/**
	* Adds a child node.
	* @param node The node to add as a child.
	*/
	NodePtr AddChild(NodePtr node);

	/**
	* Gets a child node by name.
	* @param name The name of the node.
	* @return The child node.
	*/
	NodePtr GetChild(const std::string &name);

	/**
	* Removes a child node.
	* @param name The name of the child node to remove.
	*/
	NodePtr RemoveChild(const std::string &name);

	/**
	* Removes a child node.
	* @param node The child node to remove.
	*/
	NodePtr RemoveChild(NodePtr node);

	/**
	* Renders the node.
	*/
	void Render();
	
	/**
	* Returns the local bounding box of this node;
	*/
	BoundingBox GetWorldBoundingBox();

	/**
	* Renders the bounding boxes of the sub tree under this node.
	* @param color The color to render the boxes with.
	* @param onlyWithGeometry Specifies whether to render bounding boxes only for nodes that has a geometry associated with them, defaults to true. 
	*/
	void RenderBoundingBox(vec3 color, bool onlyWithGeometry = true);

	/**
	* Gets the render parameters of this node.
	* @return The render parameters of this node.
	*/
	RenderParameters &GetRenderParameters();

	/**
	* Sets the program used for rendering of the sub tree under this node.
	* @param program The program to set.
	*/
	void SetProgram(ProgramPtr program);

	/**
	* Gets the program of this node.
	* @return The program used for rendering of this node.
	*/
	ProgramPtr GetProgram() const;

	/**
	* Sets the technique used for rendering of the sub tree under this node.
	* @param name The name of the technique to set.
	*/
	void SetTechnique(const std::string &name);

	/**
	* Gets the technique of this node.
	* @return The technique used for rendering of this node.
	*/
	TechniquePtr GetTechnique() const;

	/**
	* Sets whether dynamic lighting is enabled for the sub tree under this node.
	* @param enabled True to enable, false to disable.
	*/
	void SetLighting(bool enabled = true);

	/**
	* Checks whether dynamic lighting is enabled for this node.
	* @return True if enabled, false otherwise.
	*/
	bool GetLighting() const;

	/**
	* Sets whether fog is enabled for the sub tree under this node.
	* @param enabled True to enable, false to disable.
	*/
	void SetFog(bool enabled = true);

	/**
	* Checks whether fog is enabled for this node.
	* @return True if enabled, false otherwise.
	*/
	bool GetFog() const;

	/**
	* Sets the fog color for the sub tree under this node.
	* @param color The fog color to set.
	*/
	void SetFogColor(const vec4 &color);

	/**
	* Gets the fog color of this node.
	* @return The fog color.
	*/
	vec4 GetFogColor() const;

	/**
	* Sets the fog density for the sub tree under this node.
	* @param density The fog density to set.
	*/
	void SetFogDensity(float density);

	/**
	* Gets the fog density of this node.
	* @return The fog density.
	*/
	float GetFogDensity() const;
	
	/**
	* Clones the node.
	* The cloned node has no parent and points to the same geometries as the original.
	*/
	virtual NodePtr Clone() const;
	
	/**
	* @return The world space position of this node.
	*/
	vec3 GetWorldPosition();
		
protected:
	virtual BoundingBox GetLocalBoundingBox() const;
	virtual void PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera);		
	void UpdateWorldBoundingBox();
	void SetTransformationChanged();
	void SetBoundingBoxChanged();		
	void RenderBoundingBox(MaterialPtr material, ProgramPtr program, bool onlyWithGeometry);
	void PrepareRenderBoundingBox();

	std::vector<NodePtr> children;	

	vec3 translation;
	mat4 rotation;
	vec3 scale;	
	mat4 localTransformation;
	bool worldTransformationChanged;	    
	bool localTransformationChanged;
	mat4 worldTransformation;
	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;
	bool worldBoundingBoxRenderingChanged;


	Node *parent;
	std::string name;
	RenderParameters renderParameters;
	NodeType type;

	BufferPtr bboxVBuffer;
	BufferPtr bboxIBuffer;	
};

/**
* Loads a node from a file.
* Supported file formats are 3ds, Collada and obj.
* @param filename The file to load.
* @param options Model loading options to apply to the loaded object.
* @return The loaded node.
*/
NodePtr FIRECUBE_API LoadMesh(const std::string &filename, ModelLoadingOptions options = ModelLoadingOptions());
}

#pragma warning(push)

#endif