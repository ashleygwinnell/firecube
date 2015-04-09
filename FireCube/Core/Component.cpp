#include "Core/Component.h"
using namespace FireCube;

Component::Component(Engine *engine) : Object(engine), node(nullptr), enabled(true), scene(nullptr)
{
}

Component::Component(const Component &other) : Object(other.engine), enabled(other.enabled), node(nullptr), scene(nullptr)
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

void Component::SetScene(Scene *scene)
{
	Scene *oldScene = this->scene;
	this->scene = scene;
	if (oldScene != scene)
	{
		SceneChanged(oldScene);
	}
}

Node *Component::GetNode() const
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