#include "TexturePreviewPanelImpl.h"

TexturePreviewPanelImpl::TexturePreviewPanelImpl(wxWindow* parent, wxImage image) : TexturePreviewPanel(parent), image(image)
{
	
}

TexturePreviewPanelImpl::~TexturePreviewPanelImpl()
{

}

wxImage ScaleImage(wxImage &image, int maxDimension)
{
	if (image.GetWidth() > maxDimension || image.GetHeight() > maxDimension)
	{
		int newWidth;
		int newHeight;
		if (image.GetWidth() > image.GetHeight())
		{
			newWidth = maxDimension;
			newHeight = (float)image.GetHeight() / (float)image.GetWidth() * maxDimension;
		}
		else
		{
			newHeight = maxDimension;
			newWidth = (float)image.GetWidth() / (float)image.GetHeight() * maxDimension;
		}

		return image.Scale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);
	}

	return image;
}

void TexturePreviewPanelImpl::TexturePreviewBitmapResize(wxSizeEvent& event)
{
	static bool inResize = false; // Prevent infinite recursive call due to Layout call below
	if (image.IsOk() && inResize == false)
	{
		inResize = true;
		texturePreviewBitmap->Freeze();
		auto scaledImage = ScaleImage(image, min(event.GetSize().GetWidth(), event.GetSize().GetHeight()));
		texturePreviewBitmap->SetBitmap(wxBitmap(scaledImage));
		texturePreviewBitmap->GetParent()->Layout();
		texturePreviewBitmap->Thaw();
		inResize = false;
	}
}

