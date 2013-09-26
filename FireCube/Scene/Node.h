#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <string>
#include <vector>

#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Core/Component.h"

namespace FireCube
{

// Forward declarations.
class Node;
class RenderQueue;
class Viewport;

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
* A class representing a node in a scene graph.
*/
class FIRECUBE_API Node : public std::enable_shared_from_this<Node>
{
	friend class RenderQueue;
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

	NodePtr CreateChild(const std::string &name = "");

	/**
	* Gets a child node by name.
	* @param name The name of the node.
	* @return The child node.
	*/
	NodePtr GetChild(const std::string &name, bool recursive = false);

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
	* Clones the node.
	* The cloned node has no parent and points to the same geometries as the original.
	*/
	virtual NodePtr Clone() const;
	
	/**
	* @return The world space position of this node.
	*/
	vec3 GetWorldPosition();

	void AddComponent(Component *component);
	
	void SetViewport(Viewport *viewport);

	Viewport *GetViewport();

protected:		
	void SetTransformationChanged();	
	
	std::vector<NodePtr> children;	

	vec3 translation;
	mat4 rotation;
	vec3 scale;	
	mat4 localTransformation;
	bool transformationChanged;	
	mat4 worldTransformation;	
	
	Node *parent;
	std::string name;
	std::vector<ComponentPtr> components;
	Viewport *viewport;
};

}

#pragma warning(push)
