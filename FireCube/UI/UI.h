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

class FIRECUBE_API UI : public Object
{
	OBJECT(UI);
public:
	UI(Engine *engine);
	~UI();
	void Render(Renderer *renderer);
	UIElement *GetRoot();
private:
	
	void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData, UIElement *element);

	UIElement *root;
	VertexBuffer *vertexBuffer;
};

}