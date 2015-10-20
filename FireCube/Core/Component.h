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
		
	Node *GetNode() const;	
	void SetEnabled(bool enabled);
	bool IsEnabled() const;
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer) {};
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

