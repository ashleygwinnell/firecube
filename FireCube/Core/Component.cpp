#include "Core/Component.h"
using namespace FireCube;

Component::Component(Engine *engine) : Object(engine), node(nullptr)
{
}

Component::~Component()
{
}

void Component::SetNode(Node *node)
{	
	this->node = node;
	NodeChanged();
}

Node *Component::GetNode()
{
	return node;
}
