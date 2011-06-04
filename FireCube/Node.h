#ifndef NODE_H
#define NODE_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
typedef boost::shared_ptr<Node> NodePtr;
class Geometry;
typedef boost::shared_ptr<Geometry> GeometryPtr;
class Light;
class Material;
typedef boost::shared_ptr<Material> MaterialPtr;
class Program;
typedef boost::shared_ptr<Program> ProgramPtr;

/**
* A shared pointer to a NodeResource.
*/

/**
* A class representing a node in a scene graph.
*/
class FIRECUBE_API Node : public boost::enable_shared_from_this<Node>
{
public:
    Node();

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
    * @param r The rotation(Euler angles) to assign to this node.
    */
    void SetRotation(const vec3 &r);

    /**
    * Gets the rotation of this node.
    * @return The rotation(Euler angles) of this node.
    */
    vec3 GetRotation() const;

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
    * Sets the matrix transformation of this node.
    * @param t The matrix transformation to assign to this node.
    */
    void SetMatrixTransformation(const mat4 &t);

    /**
    * Gets the matrix transformation of this node.
    * @return The matrix transformation of this node.
    */
    mat4 GetMatrixTransformation() const;

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
    * Gets the parent of this node.
    * @return The parent of this node.
    */
    NodePtr GetParent();

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
    * Sets the geometry of this node.
    * @param geometry The geometry to set.
    */
    void SetGeometry(GeometryPtr geometry);

    /**
    * Gets the geometry of this node.
    * @return The geometry of this node.
    */
    GeometryPtr GetGeometry();

    /**
    * Adds a light to this node.
    * @param light The light to add.
    */
    void AddLight(const Light &light);

    /**
    * Removes a light from this node.
    * @param light The light to remove.
    */
    void RemoveLight(const Light &light);

    /**
    * Gets the lights of this node.
    * @return A list of lights.
    */
    std::vector<Light> &GetLights();

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
    * Creates hard normals for all geometries of the sub tree under this node.
    */
    void CreateHardNormals();

	/**
	* Returns the local bounding box of this node;
	*/
	BoundingBox GetWorldBoundingBox();

    /**
    * Renders the bounding boxes of the sub tree under this node.
	* @param color The color to render the boxes with.
	* @param onlyWithGeometry Specifies whether to render bounding boxes only for nodes that has a geometry associated with them, defaults to true. 
    */
    void RenderBoundingBox(vec4 color, bool onlyWithGeometry = true);

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
	NodePtr Clone() const;

	/**
	* @return The world space position of this node.
	*/
	vec3 GetWorldPosition();

private:
	void SetTransformationChanged();
	void SetBoundingBoxChanged();
	void UpdateWorldTransformation();	
    void UpdateWorldBoundingBox();
	void RenderBoundingBox(MaterialPtr material, ProgramPtr program, bool onlyWithGeometry);
	void PrepareRenderBoundingBox();
    std::vector<NodePtr> children;
    GeometryPtr geometry;
    std::vector<Light> lights;
    
	vec3 translation;
    vec3 rotation;
    vec3 scale;
    mat4 matTransform;
    mat4 localTransformation;
    bool worldTransformationChanged;	    
	bool localTransformationChanged;
	mat4 worldTransformation;
	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;
	bool worldBoundingBoxRenderingChanged;


    NodePtr parent;
    std::string name;
    RenderParameters renderParameters;

	BufferPtr bboxVBuffer;
	BufferPtr bboxIBuffer;
    
};

/**
* Loads a node from a file.
* Supported file formats are 3ds and Collada.
* @param filename The file to load.
* @return The loaded node.
*/
NodePtr FIRECUBE_API LoadMesh(const std::string &filename);
}

#pragma warning(push)

#endif