#pragma once

#include <memory>
#include "Utils/utils.h"
#include "Core/Object.h"

namespace FireCube
{

class Node;

class FIRECUBE_API Component : public Object
{	
	friend class Node;
public:
	Component(Engine *engine);
	virtual ~Component();
		
	Node *GetNode();	
protected:	
	void SetNode(Node *node);
	virtual void MarkedDirty() = 0;
	virtual void NodeChanged() = 0;
	
	Node *node;
};

typedef std::shared_ptr<Component> ComponentPtr;

}

