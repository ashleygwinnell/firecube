#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <string>
#include <vector>

#include "Math/Math.h"
#include "Math/BoundingBox.h"
#include "Core/Component.h"

namespace FireCube
{

// Forward declarations.
class Node;
class RenderQueue;
class Scene;

/**
* A shared pointer to a Node.
*/
class Geometry;
class Light;
class Material;
class Program;


/**
* A class representing a node in a scene graph.
*/
class FIRECUBE_API Node : public Object
{
	FIRECUBE_OBJECT(Node)
	friend class RenderQueue;
	friend class Scene;
public:	
	Node(Engine *engine);

	/**
	* Constructs a node.
	* @param name The name of this node.
	*/
	Node(Engine *engine, const std::string &name);	

	~Node();

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
	* @returns The world rotation of this node
	*/
	quat GetWorldRotation();

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
	* @param r The rotation quaternion to assign to this node.
	*/
	void SetRotation(const quat &r);

	/**
	* Gets the rotation of this node.
	* @return The rotation quaternion of this node.
	*/
	quat GetRotation() const;

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
	* @param position The new position of the node
	* @param at The look at position
	* @param up The up vector
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
	void SetParent(Node *parent);

	/**
	* Gets the children of this node.
	* @return A list of children.
	*/
	std::vector<SharedPtr<Node>> GetChildren() const;
	
	/**
	* Adds a child node. The node is reparented to this node 
	* @param node The node to add as a child.
	*/
	Node *AddChild(Node *node);
	
	/**
	* Creates a new child node
	* @param name The name to of the new child node	
	* @returns The child node
	*/
	Node *CreateChild(const std::string &name = "");

	/**
	* Gets a child node by name.
	* @param name The name of the node.
	* @return The child node.
	*/
	Node *GetChild(const std::string &name, bool recursive = false);

	/**
	* Removes a child node.
	* @param name The name of the child node to remove.
	*/
	void RemoveChild(const std::string &name);

	/**
	* Removes a child node.
	* @param node The child node to remove.
	*/
	void RemoveChild(Node *node);	
	
	/**
	* Removes all children of this node
	*/
	void RemoveAllChildren();
		
	/**
	* Clones the node.	
	*/
	virtual Node *Clone() const;
	
	/**
	* @return The world space position of this node.
	*/
	vec3 GetWorldPosition();
	
	/**
	* Adds a component to this node
	* @param component The component to add
	*/
	void AddComponent(Component *component);
	
	/**
	* Removes a component
	* @param component The component to remove
	*/
	void RemoveComponent(Component *component);	
	
	/**
	* @returns the scene this node belongs to
	*/	
	Scene *GetScene() const;
	
	/**
	* Removes all components from this node
	*/
	void RemoveAllComponents();
	
	/**
	* Creates a component and add it to this node 
	*/
	template <class T, typename... Args> T* CreateComponent(Args&&... args)
	{
		T *component = new T(engine, std::forward<Args>(args)...);
		AddComponent(component);
		return component;
	}	

	/**
	* Returns a component in this node of a given type if one exists. Otherwise, returns nullptr
	* @param type The hash of the component type 
	*/
	Component *GetComponent(const StringHash &type);

	/**
	* Returns a component in this node of a given type if one exists. Otherwise, returns nullptr	 
	*/
	template <class T> T* GetComponent()
	{
		T *ret = static_cast<T *>(GetComponent(T::GetTypeStatic()));
		return ret;
	}

	/**
	* Returns all component of a given type.
	* @param components Output vector where the returned components are added
	* @param recursive Whether to ecurse into child node		 
	*/
	template <class T>
	void GetComponents(std::vector<T *> &components, bool recursive = false)
	{
		if (!recursive)
		{
			for (auto c : this->components)
			{
				if (c->GetType() == T::GetTypeStatic())
				{
					components.push_back(static_cast<T *>(c));
				}
			}
		}
		else
		{
			GetComponentsRecursive(components);
		}
	}	

	/**
	* Returns all component of a given type.
	* @param type The hash of the component type
	* @param components Output vector where the returned components are added
	* @param recursive Whether to ecurse into child node		 
	*/
	void GetComponents(const StringHash &type, std::vector<Component *> &components, bool recursive = false);
	
	Node *GetRootNode() const;
	
	/**
	* @returns all the components belonging to this node 
	*/
	std::vector<Component *> &GetComponents();
	
	/**
	* Removes this node from its parent
	*/
	void Remove();

protected:		

	template <class T>
	void GetComponentsRecursive(std::vector<T *> &components)
	{
		for (auto c : this->components)
		{
			if (c->GetType() == T::GetTypeStatic())
			{
				components.push_back(static_cast<T *>(c));
			}
		}

		for (auto c : children)
		{
			c->GetComponentsRecursive(components);
		}
	}

	void GetComponentsRecursive(const StringHash &type, std::vector<Component *> &components);

	void SetTransformationChanged();	
	void SceneChanged(Scene *oldScene);
	void UpdateWorldTransformation();
	
	std::vector<SharedPtr<Node>> children;	

	vec3 translation;
	quat rotation;
	vec3 scale;	
	mat4 localTransformation;
	bool transformationChanged;	
	mat4 worldTransformation;	
	quat worldRotation;
	
	Node *parent;
	std::string name;
	std::vector<Component *> components;
	Scene *scene;
};

}

#pragma warning(push)
