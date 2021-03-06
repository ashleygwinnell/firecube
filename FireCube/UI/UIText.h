#pragma once

#include "UI/UIElement.h"

namespace FireCube
{

class FontFace;

/**
* A static text UI element
*/
class FIRECUBE_API UIText : public UIElement
{
	FIRECUBE_OBJECT(UIText);
public:
	UIText(Engine *engine);
	~UIText();	
	
	/**
	* Sets the font to render the text
	* @param fontFace The font face to use
	*/
	void SetFontFace(FontFace *fontFace);

	/**	
	* @returns The font face used to render the text
	*/
	FontFace *GetFontFace() const;
	
	/**
	* Sets the text
	* @param text The text
	*/
	void SetText(const std::string &text);

	/**	
	* @returns The text
	*/
	std::string GetText() const;
	
	/**
	* Sets the color and transparency of the text
	* @param color The color and transparency of the text
	*/
	void SetColor(vec4 color);
	
	/**
	* @returns The color of the text
	*/
	vec4 GetColor() const;
private:

	virtual void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData) override;
	void UpdateData();

	FontFace *fontFace;	
	std::string text;
	std::vector<UIVertex> vertexData;
	vec4 color;
};

}