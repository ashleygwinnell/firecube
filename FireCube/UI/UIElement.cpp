#include "UIElement.h"
#include "Core/Engine.h"

using namespace FireCube;

UIElement::UIElement(Engine *engine) : Object(engine), parent(nullptr), position(0.0f), screenPosition(0.0f), positionChanged(false)
{

}

std::vector<UIElement *> &UIElement::GetChildren()
{
	return children;
}

UIElement::~UIElement()
{
	for (auto child : children)
	{
		delete child;
	}
}

void UIElement::SetPosition(vec2 position)
{
	SetPositionChanged();
	this->position = position;
}

vec2 UIElement::GetPosition() const
{
	return position;
}

void UIElement::SetPositionChanged()
{
	if (positionChanged)
		return;

	positionChanged = true;
	for (auto c : children)
	{
		c->SetPositionChanged();
	}
}

vec2 UIElement::GetScreenPosition()
{
	if (positionChanged)
	{
		screenPosition = parent ? parent->GetScreenPosition() + position : position;
		positionChanged = false;
	}

	return screenPosition;
}

void UIElement::RemoveImmediately()
{
	if (parent)
	{
		parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), this), parent->children.end());
		parent = nullptr;
	}
}

void UIElement::AddChild(UIElement *element)
{
	element->RemoveImmediately();

	element->parent = this;
	this->children.push_back(element);
}

UIElement *UIElement::GetParent()
{
	return parent;
}

void UIElement::SetParent(UIElement *parent)
{
	RemoveImmediately();
	
	this->parent = parent;

	if (this->parent)
	{
		this->parent->children.push_back(this);
	}
}

void UIElement::RemoveAllChildrenImmediately()
{
	for (auto child : children)
	{
		delete child;
	}

	children.clear();
}

void UIElement::Remove()
{
	if (parent)
	{
		parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), this), parent->children.end());
		parent = nullptr;
		engine->GetUI()->AddDelayedRemoveElement(this);
	}
}

void UIElement::RemoveAllChildren()
{
	for (auto child : children)
	{
		child->parent = nullptr;
		engine->GetUI()->AddDelayedRemoveElement(child);
	}

	children.clear();
}
