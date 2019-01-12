#include <array>
#include "UIImage.h"
#include "Rendering/Texture2D.h"
using namespace FireCube;

UIImage::UIImage(Engine *engine) : UIElement(engine), texture(nullptr), color(1.0f)
{

}

UIImage::~UIImage()
{

}

void UIImage::GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &data)
{
	if (texture)
	{
		std::array<UIVertex, 6> vertexData;
		vertexData[0].position = vec3(0.0f);
		vertexData[0].uv = vec2(0, 1);
		vertexData[0].color = color;

		vertexData[1].position = vec3( vec2(0, size.y), 0.0f);
		vertexData[1].uv = vec2(0, 0);
		vertexData[1].color = color;

		vertexData[2].position = vec3( size, 0.0f);
		vertexData[2].uv = vec2(1, 0);
		vertexData[2].color = color;

		vertexData[3].position = vec3(0.0f);
		vertexData[3].uv = vec2(0, 1);
		vertexData[3].color = color;

		vertexData[4].position = vec3(size, 0.0f);
		vertexData[4].uv = vec2(1, 0);
		vertexData[4].color = color;

		vertexData[5].position = vec3(vec2(size.x, 0), 0.0f);
		vertexData[5].uv = vec2(1, 1);
		vertexData[5].color = color;

		UIPart part;
		part.texture = texture;
		part.offset = data.size();
		part.count = vertexData.size();
		parts.push_back(part);
		data.insert(data.end(), vertexData.begin(), vertexData.end());
	}
}

void UIImage::SetImage(Texture2D *texture)
{
	this->texture = texture;
}

void UIImage::SetSize(vec2 size)
{
	this->size = size;
}

Texture2D *UIImage::GetImage() const
{
	return texture;
}

vec2 UIImage::GetSize() const
{
	return size;
}

void UIImage::SetColor(vec4 color)
{
	this->color = color;
}

vec4 UIImage::GetColor() const
{
	return color;
}

