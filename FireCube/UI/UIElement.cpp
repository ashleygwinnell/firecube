#include "UIElement.h"

using namespace FireCube;

UIElement::UIElement(Engine *engine, UIElement *parent) : Object(engine), parent(parent), position(0.0f), screenPosition(0.0f), positionChanged(false)
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