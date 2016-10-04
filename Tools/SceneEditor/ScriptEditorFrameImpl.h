#pragma once

#include "mainframe.h"

class ScriptEditorFrameImpl : public ScriptEditorFrame
{
public:	
	ScriptEditorFrameImpl(wxWindow* parent);

	void OpenFile(const std::string &filename);

private:	
	virtual void SaveClicked(wxCommandEvent& event) override;
	virtual void OnKeyDown(wxKeyEvent& event) override;
	void OnMarginClick(wxStyledTextEvent &event);
	void OnCharAdded(wxStyledTextEvent &event);
	virtual void OnClose(wxCloseEvent& event) override;
	void OnBrace(wxStyledTextEvent &event);
	void SaveScript();
	void UpdateTitle();

	std::string currentFileName;
};