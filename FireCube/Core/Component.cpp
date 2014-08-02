#include "Core/Component.h"
using namespace FireCube;

Component::Component(Engine *engine) : Object(engine), node(nullptr), enabled(true)
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

void Component::SetEnabled(bool enabled)
{
	if (this->enabled != enabled)
	{
		this->enabled = enabled;
		EnabledChanged();
	}
}

bool Component::IsEnabled() const
{
	return enabled;
}