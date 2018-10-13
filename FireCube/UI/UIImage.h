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
		UIImage(Engine *engine);
		~UIImage();
		
		/**
		* Sets the image
		* @param texture The texture to use
		*/
		void SetImage(Texture2D *texture);

		/**		
		* @returns The texture used
		*/
		Texture2D *GetImage() const;

		/**
		* Sets the size
		* @param size the new size
		*/
		void SetSize(vec2 size);

		/**
		* Sets the size
		* @returns the size
		*/
		vec2 GetSize() const;

		/**
		* Sets the color
		* @param color the new color
		*/
		void SetColor(vec4 color);

		/**		
		* @returns the color
		*/
		vec4 GetColor() const;

	private:		

		virtual void GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData) override;

		SharedPtr<Texture2D> texture;
		std::vector<UIVertex> vertexData;
		vec2 size;
		vec4 color;
	};

}