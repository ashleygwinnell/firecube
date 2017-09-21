#pragma once

#include "mainframe.h"
#include "wx/stc/stc.h"
#include "wx/fdrepdlg.h"

class PageInfo
{
public:
	std::string filename;
	wxStyledTextCtrl *sourceText;
};

class ScriptEditorFrameImpl : public ScriptEditorFrame
{
public:	
	ScriptEditorFrameImpl(wxWindow* parent);
	~ScriptEditorFrameImpl();

	void OpenFile(const std::string &filename);

	void NewTab(const std::string &filename, const std::string &resolvedFileName);

private:	
	virtual void SaveClicked(wxCommandEvent& event) override;
	void OnKeyDown(wxKeyEvent& event);
	void OnMarginClick(wxStyledTextEvent &event);
	void OnCharAdded(wxStyledTextEvent &event);
	virtual void OnClose(wxCloseEvent& event) override;
	virtual void OnNotebookPageClose(wxAuiNotebookEvent& event) override;
	void OnBrace(wxStyledTextEvent &event);
	virtual void OnNotebookPageChanged(wxAuiNotebookEvent& event) override;
	void SaveScript();
	void UpdateTitle();
	void OnFindDialog(wxFindDialogEvent& event);
	
	std::vector<PageInfo> currentPages;
	wxFindReplaceData findReplaceData;
	wxFindReplaceDialog *findReplaceDlg;
	wxStyledTextCtrl *sourceText;
};