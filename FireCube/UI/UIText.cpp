#include "UI/UIText.h"
#include "Rendering/Font.h"

using namespace FireCube;


UIText::UIText(Engine *engine, UIElement *parent) : UIElement(engine, parent), color(1.0f)
{	
	
}

UIText::~UIText()
{
	
}

void UIText::GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &data)
{
	if (vertexData.empty())
	{
		return;
	}

	UIPart part;
	part.texture = fontFace->page->tex;
	part.offset = data.size();
	part.count = vertexData.size();
	parts.push_back(part);
	data.insert(data.end(), vertexData.begin(), vertexData.end());	
}

void UIText::SetFontFace(FontFace *fontFace)
{
	this->fontFace = fontFace;
}

void UIText::SetText(const std::string &text)
{
	if (!fontFace || text.empty())
		return;
	
	vertexData.resize(text.size() * 6);
	
	int numTris = 0;
	
	vec2 intialPosition = GetScreenPosition();
	vec2 curPos = intialPosition;	
	char previous = 0;
	for (std::string::const_iterator i = text.begin(); i != text.end(); i++)
	{
		char c = *i;
		if (c == 32)
		{
			// If current glyph is space simply advance the current position
			curPos.x += fontFace->glyph[c].advance;
			continue;
		}
		else if (c == '\n')
		{
			// If current glyph is new line set the current position accordingly
			curPos.x = intialPosition.x;
			curPos.y += fontFace->pointSize;
			continue;
		}
		else if (fontFace->glyph[c].size != vec2(0, 0))
		{
			if (previous && c)
			{
				curPos.x += fontFace->GetKerning(previous, c);
			}

			// Populate the vertex buffer with the position and the texture coordinates of the current glyph
			vertexData[numTris * 3 + 0].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x, fontFace->glyph[c].bitmapOffset.y + curPos.y, 0.0f);
			vertexData[numTris * 3 + 1].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x, fontFace->glyph[c].bitmapOffset.y + curPos.y + fontFace->glyph[c].size.y, 0.0f);
			vertexData[numTris * 3 + 2].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x + fontFace->glyph[c].size.x, fontFace->glyph[c].bitmapOffset.y + curPos.y, 0.0f);
			vertexData[numTris * 3 + 0].uv = vec2(fontFace->glyph[c].uv.x, fontFace->glyph[c].uv.y);
			vertexData[numTris * 3 + 1].uv = vec2(fontFace->glyph[c].uv.x, fontFace->glyph[c].uv.y + fontFace->glyph[c].size.y / 512.0f);
			vertexData[numTris * 3 + 2].uv = vec2(fontFace->glyph[c].uv.x + fontFace->glyph[c].size.x / 512.0f, fontFace->glyph[c].uv.y);
			vertexData[numTris * 3 + 0].color = color;
			vertexData[numTris * 3 + 1].color = color;
			vertexData[numTris * 3 + 2].color = color;
			numTris++;

			vertexData[numTris * 3 + 0].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x + fontFace->glyph[c].size.x, fontFace->glyph[c].bitmapOffset.y + curPos.y, 0.0f);
			vertexData[numTris * 3 + 1].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x, fontFace->glyph[c].bitmapOffset.y + curPos.y + fontFace->glyph[c].size.y, 0.0f);
			vertexData[numTris * 3 + 2].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x + fontFace->glyph[c].size.x, fontFace->glyph[c].bitmapOffset.y + curPos.y + fontFace->glyph[c].size.y, 0.0f);
			vertexData[numTris * 3 + 0].uv = vec2(fontFace->glyph[c].uv.x + fontFace->glyph[c].size.x / 512.0f, fontFace->glyph[c].uv.y);
			vertexData[numTris * 3 + 1].uv = vec2(fontFace->glyph[c].uv.x, fontFace->glyph[c].uv.y + fontFace->glyph[c].size.y / 512.0f);
			vertexData[numTris * 3 + 2].uv = vec2(fontFace->glyph[c].uv.x + fontFace->glyph[c].size.x / 512.0f, fontFace->glyph[c].uv.y + fontFace->glyph[c].size.y / 512.0f);
			vertexData[numTris * 3 + 0].color = color;
			vertexData[numTris * 3 + 1].color = color;
			vertexData[numTris * 3 + 2].color = color;
			numTris++;

			curPos.x += fontFace->glyph[c].advance;			
			previous = c;
		}		
	}	

	vertexData.resize(numTris * 3);
}

void UIText::SetColor(vec4 color)
{
	this->color = color;
}

vec4 UIText::GetColor() const
{
	return color;
}