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
	OBJECT(Component)
	friend class Node;
public:
	Component(Engine *engine);
	virtual ~Component();
		
	Node *GetNode();	
	void SetEnabled(bool enabled);
	bool IsEnabled() const;
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer) {};
protected:	
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

