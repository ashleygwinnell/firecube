#pragma once

#include "Utils/utils.h"
#include "Math/Math.h"
#include "Core/Object.h"
#include "UI/UI.h"

namespace FireCube
{

class Renderer;
class UI;

/**
* Base class for UI elements
*/
class FIRECUBE_API UIElement : public Object
{
	friend class UI;
	FIRECUBE_OBJECT(UIElement);
public:
	UIElement(Engine *engine);
	virtual ~UIElement();
	
	std::vector<UIElement *> &GetChildren();
	void SetPosition(vec2 position);
	vec2 GetPosition() const;
	vec2 GetScreenPosition();
	void RemoveImmediately();
	void AddChild(UIElement *element);
	UIElement *GetParent();
	void SetParent(UIElement *parent);
	void RemoveAllChildrenImmediately();
	void Remove();
	void RemoveAllChildren();

	template <class T, typename... Args> T* CreateChild(Args... args)
	{
		T *child = new T(engine, args...);
		AddChild(child);
		return child;
	}
	
protected:
	void SetPositionChanged();
	virtual void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData) {}

	vec2 position;
	vec2 screenPosition;
	bool positionChanged;
	UIElement *parent;
	std::vector<UIElement *> children;
};

}