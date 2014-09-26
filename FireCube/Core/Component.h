#pragma once

#include <memory>
#include "Utils/utils.h"
#include "Core/Object.h"

namespace FireCube
{

class Node;
class DebugRenderer;

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
	virtual void MarkedDirty() = 0;
	virtual void NodeChanged() = 0;
	virtual void EnabledChanged() {};
	
	Node *node;
	bool enabled;
};

}

