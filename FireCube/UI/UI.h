#pragma once
#include <vector>
#include "Utils/utils.h"
#include "Core/Object.h"
#include "Math/Math.h"

namespace FireCube
{

class Renderer;
class Texture2D;
class UIElement;
class VertexBuffer;

struct UIVertex
{
	vec3 position;
	vec2 uv;
	vec4 color;
};

class UIPart
{
public:
	Texture2D *texture;
	unsigned int offset;
	unsigned int count;
};

/**
* This class is responsible for rendering UI
*/
class FIRECUBE_API UI : public Object
{
	FIRECUBE_OBJECT(UI);
public:
	UI(Engine *engine);
	~UI();
	
	/**
	* Renders the ui
	* @param renderer The renderer to use
	*/
	void Render(Renderer *renderer);

	/**
	* Renders the ui
	* @param renderer The renderer to use
	* @param root The root ui element
	*/
	void Render(Renderer *renderer, UIElement *root);
	
	/**
	* @returns the root UI element
	*/
	UIElement *GetRoot();

	void AddDelayedRemoveElement(UIElement *element);
	void DeleteDelayedRemoveElements();
private:
	
	void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData, UIElement *element);

	UIElement *root;
	VertexBuffer *vertexBuffer;
	std::vector<UIElement *> delayedRemoveElements;
};

}