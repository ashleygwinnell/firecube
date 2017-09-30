#pragma once

#include "UI/UIElement.h"

namespace FireCube
{

	class Image;

	/**
	* A static image UI element
	*/
	class FIRECUBE_API UIImage : public UIElement
	{
		FIRECUBE_OBJECT(UIImage);
	public:
		UIImage(Engine *engine, UIElement *parent);
		~UIImage();
		
		/**
		* Sets the image
		* @param texture The texture to use
		*/
		void SetImage(Texture2D *texture);

		/**
		* Sets the size
		* @param size the new size
		*/
		void SetSize(vec2 size);

	private:		

		virtual void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData) override;

		SharedPtr<Texture2D> texture;
		std::vector<UIVertex> vertexData;
		vec2 size;
	};

}