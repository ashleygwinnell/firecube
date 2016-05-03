#pragma once

#include <memory>
#include "Utils/utils.h"
#include "Core/Object.h"

namespace FireCube
{

class Node;
class DebugRenderer;
class Scene;

class FIRECUBE_API Component : public Object
{	
	FIRECUBE_OBJECT(Component)
	friend class Node;
public:
	Component(Engine *engine);
	virtual ~Component();
		
	/**
	* Returns the parent node of this component
	*/
	Node *GetNode() const;	
	
	/**
	* Controls whether the component is enabled
	* @param enabled Whether to enable the component
	*/
	void SetEnabled(bool enabled);
	
	/**
	* Returns whether the component is enabled
	*/
	bool IsEnabled() const;
	
	/**
	* Renders debug geometry of this component
	* @param debugRender The bug renderer
	*/
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer) {};
	
	/**
	* Clones the component
	*/
	virtual Component *Clone() const = 0;
protected:	
	Component(const Component &other);

	void SetNode(Node *node);
	void SetScene(Scene *scene);
	virtual void MarkedDirty() = 0;
	virtual void NodeChanged() = 0;
	virtual void EnabledChanged() {};
	virtual void SceneChanged(Scene *oldScene) = 0;
	
	Node *node;
	Scene *scene;
	bool enabled;
};

}

