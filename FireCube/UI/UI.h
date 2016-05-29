#pragma once
#include <vector>
#include "Utils/utils.h"
#include "Core/Object.h"
#include "Math/Math.h"

namespace FireCube
{

class Renderer;
class Texture;
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
	Texture *texture;
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
	* @returns the root UI element
	*/
	UIElement *GetRoot();
private:
	
	void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData, UIElement *element);

	UIElement *root;
	VertexBuffer *vertexBuffer;
};

}