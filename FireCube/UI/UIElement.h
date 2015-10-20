#pragma once

#include "Utils/utils.h"
#include "Math/Math.h"
#include "Core/Object.h"
#include "UI/UI.h"

namespace FireCube
{

class Renderer;

class FIRECUBE_API UIElement : public Object
{
	FIRECUBE_OBJECT(UIElement);
public:
	UIElement(Engine *engine, UIElement *parent);
	virtual ~UIElement();
	virtual void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData) {}
	std::vector<UIElement *> &GetChildren();
	void SetPosition(vec2 position);
	vec2 GetPosition() const;
	vec2 GetScreenPosition();

	template <class T, typename... Args> T* CreateChild(Args... args)
	{
		T *child = new T(engine, this, args...);
		children.push_back(child);
		return child;
	}
	
protected:
	void SetPositionChanged();

	vec2 position;
	vec2 screenPosition;
	bool positionChanged;
	UIElement *parent;
	std::vector<UIElement *> children;
};

}