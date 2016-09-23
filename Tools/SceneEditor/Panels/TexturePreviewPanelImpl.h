#pragma once

#include "../mainframe.h"

class TexturePreviewPanelImpl : public TexturePreviewPanel
{
public:
	TexturePreviewPanelImpl(wxWindow* parent, wxImage image);
	~TexturePreviewPanelImpl();
private:

	virtual void TexturePreviewBitmapResize(wxSizeEvent& event) override;
	
	wxImage image;
};