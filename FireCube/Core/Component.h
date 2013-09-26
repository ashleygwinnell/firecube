#pragma once

#include <memory>
#include "Core/Object.h"

namespace FireCube
{

class Node;

class Component : public Object
{	
	friend class Node;
public:
	Component(Engine *engine);
	virtual ~Component();
	
	void SetNode(Node *node);
	Node *GetNode();	
protected:	
	virtual void MarkedDirty() = 0;
	virtual void NodeChanged() = 0;
	
	Node *node;
};

typedef std::shared_ptr<Component> ComponentPtr;

}

