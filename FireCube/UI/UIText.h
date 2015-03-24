#pragma once

#include "UI/UIElement.h"

namespace FireCube
{

class FontFace;

class FIRECUBE_API UIText : public UIElement
{
	OBJECT(UIText);
public:
	UIText(Engine *engine, UIElement *parent);
	~UIText();	
	virtual void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData);
	void SetFontFace(FontFace *fontFace);
	void SetText(const std::string &text);
	void SetColor(vec4 color);
	vec4 GetColor() const;
private:
	FontFace *fontFace;	
	std::string text;
	std::vector<UIVertex> vertexData;
	vec4 color;
};

}