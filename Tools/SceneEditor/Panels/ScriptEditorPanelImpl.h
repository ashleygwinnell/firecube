#pragma once

#include "../mainframe.h"
#include "FireCube.h"

class EditorState;

class ScriptEditorPanelImpl : public ScriptEditorPanel, public FireCube::Object
{
	FIRECUBE_OBJECT(ScriptEditorPanelImpl)
		
public:
	ScriptEditorPanelImpl(wxWindow* parent);
	~ScriptEditorPanelImpl();

	void OpenFile(const std::string &filename);

private:

	void OnMarginClick(wxStyledTextEvent &event);
	void OnCharAdded(wxStyledTextEvent &event);
	void OnBrace(wxStyledTextEvent &event);
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void SaveClicked(wxCommandEvent& event);
	void SaveScript();
	
	EditorState *editorState;
	std::string currentFileName;
};
