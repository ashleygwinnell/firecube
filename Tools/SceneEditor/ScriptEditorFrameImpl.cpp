#include <fstream>
#include "ScriptEditorFrameImpl.h"
#include "FireCube.h"

using namespace FireCube;

enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
};

ScriptEditorFrameImpl::ScriptEditorFrameImpl(wxWindow* parent) : ScriptEditorFrame(parent)
{
	sourceText->StyleSetFaceName(wxSTC_STYLE_DEFAULT, "Consolas");
	sourceText->StyleSetSize(wxSTC_STYLE_DEFAULT, 10);
	sourceText->StyleClearAll();
	sourceText->SetLexer(wxSTC_LEX_LUA);

	sourceText->SetWrapMode(wxSTC_WRAP_NONE);

	sourceText->StyleSetForeground(wxSTC_LUA_DEFAULT, wxColour(0, 0, 0));
	sourceText->StyleSetForeground(wxSTC_LUA_COMMENT, wxColour(128, 128, 128));
	sourceText->StyleSetItalic(wxSTC_LUA_COMMENT, true);
	sourceText->StyleSetForeground(wxSTC_LUA_COMMENTLINE, wxColour(128, 128, 128));
	sourceText->StyleSetItalic(wxSTC_LUA_COMMENTLINE, true);
	sourceText->StyleSetForeground(wxSTC_LUA_COMMENTDOC, wxColour(128, 128, 128));
	sourceText->StyleSetItalic(wxSTC_LUA_COMMENTDOC, true);
	sourceText->StyleSetForeground(wxSTC_LUA_NUMBER, wxColour(0, 196, 0));
	sourceText->StyleSetForeground(wxSTC_LUA_STRING, wxColour(225, 0, 0));
	sourceText->StyleSetForeground(wxSTC_LUA_CHARACTER, wxColour(225, 128, 0));
	sourceText->StyleSetForeground(wxSTC_LUA_LITERALSTRING, wxColour(196, 0, 0));
	sourceText->StyleSetForeground(wxSTC_LUA_PREPROCESSOR, wxColour(64, 64, 64));
	sourceText->StyleSetForeground(wxSTC_LUA_OPERATOR, wxColour(128, 0, 196));
	sourceText->StyleSetBold(wxSTC_LUA_OPERATOR, true);
	sourceText->StyleSetForeground(wxSTC_LUA_IDENTIFIER, wxColour(0, 0, 0));
	sourceText->StyleSetForeground(wxSTC_LUA_STRINGEOL, wxColour(225, 0, 0));
	sourceText->StyleSetForeground(wxSTC_LUA_WORD, wxColour(0, 0, 255));
	sourceText->StyleSetBold(wxSTC_LUA_WORD, true);
	sourceText->StyleSetForeground(wxSTC_LUA_WORD2, wxColour(0, 0, 255));
	sourceText->StyleSetForeground(wxSTC_LUA_WORD3, wxColour(0, 128, 255));
	sourceText->StyleSetForeground(wxSTC_LUA_WORD4, wxColour(196, 196, 0));

	// Key words
	sourceText->SetKeyWords(0, wxT("function for while repeat until if else elseif end break return in do then"));
	sourceText->SetKeyWords(1, wxT("local"));
	sourceText->SetKeyWords(2, wxT("and or not"));
	sourceText->SetKeyWords(3, wxT("nil true false"));
	// Brace matching
	sourceText->StyleSetForeground(wxSTC_STYLE_BRACELIGHT, wxColour(255, 255, 255));
	sourceText->StyleSetBackground(wxSTC_STYLE_BRACELIGHT, wxColour(128, 0, 196));
	sourceText->StyleSetForeground(wxSTC_STYLE_BRACEBAD, wxColour(255, 255, 255));
	sourceText->StyleSetBackground(wxSTC_STYLE_BRACEBAD, wxColour(255, 0, 0));
	// Margin (folding)
	sourceText->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
	sourceText->SetMarginWidth(MARGIN_FOLD, 15);
	sourceText->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	sourceText->SetMarginSensitive(MARGIN_FOLD, true);
	// Folding
	wxColor grey(128, 128, 128);
	sourceText->SetProperty(wxT("fold"), wxT("1"));
	sourceText->SetProperty(wxT("fold.comment"), wxT("1"));
	sourceText->SetProperty(wxT("fold.compact"), wxT("0"));
	sourceText->SetProperty(wxT("fold.preprocessor"), wxT("1"));
	sourceText->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS, *wxWHITE, grey);
	sourceText->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS, *wxWHITE, grey);
	sourceText->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, *wxWHITE, grey);
	sourceText->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, *wxWHITE, grey);
	sourceText->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED, *wxWHITE, grey);
	sourceText->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED, *wxWHITE, grey);
	sourceText->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, *wxWHITE, grey);
	// Line numbers
	sourceText->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
	sourceText->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	sourceText->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
	sourceText->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	sourceText->Bind(wxEVT_STC_MARGINCLICK, &ScriptEditorFrameImpl::OnMarginClick, this);
	sourceText->Bind(wxEVT_STC_CHARADDED, &ScriptEditorFrameImpl::OnCharAdded, this);
	sourceText->Bind(wxEVT_STC_UPDATEUI, &ScriptEditorFrameImpl::OnBrace, this);
}

void ScriptEditorFrameImpl::OpenFile(const std::string &filename)
{
	auto resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty() == false)
	{
		currentFileName = filename;

		std::ifstream f(resolvedFileName, std::ios_base::binary);
		std::string source((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		sourceText->SetTextRaw(source.c_str());
		sourceText->EmptyUndoBuffer();

		UpdateTitle();
	}
}

void ScriptEditorFrameImpl::OnMarginClick(wxStyledTextEvent &event)
{
	if (event.GetMargin() == MARGIN_FOLD)
	{
		int lineClick = sourceText->LineFromPosition(event.GetPosition());
		int levelClick = sourceText->GetFoldLevel(lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
		{
			sourceText->ToggleFold(lineClick);
		}
	}
}

void ScriptEditorFrameImpl::OnCharAdded(wxStyledTextEvent &event)
{
	if (static_cast<char>(event.GetKey()) == '\n' || static_cast<char>(event.GetKey()) == '\r')
	{
		int line = sourceText->GetCurrentLine();
		if (line > 0)
		{
			int startPos = sourceText->WordStartPosition(sourceText->GetLineIndentPosition(line - 1), true);
			int endPos = sourceText->WordEndPosition(sourceText->GetLineIndentPosition(line - 1), true);

			int extraIndent = 0;
			auto word = sourceText->GetRange(startPos, endPos);
			if (word == "if" || word == "elseif" || word == "else" || word == "function" || word == "while" || word == "for" || word == "repeat")
			{
				extraIndent = 1;
			}

			int curIndentation = sourceText->GetLineIndentation(line - 1);
			int indent = curIndentation + extraIndent * sourceText->GetIndent();
			if (indent > 0)
			{
				sourceText->SetLineIndentation(line, indent);
				sourceText->GotoPos(sourceText->GetLineIndentPosition(line));
			}
		}
	}
}

void ScriptEditorFrameImpl::OnBrace(wxStyledTextEvent &event)
{
	// Caret position
	int pos = sourceText->GetCurrentPos();

	// Keys at position
	char pre_key = static_cast<char>(sourceText->GetCharAt(pos - 1));
	char key = static_cast<char>(sourceText->GetCharAt(pos));

	// Brace search
	int hit = wxSTC_INVALID_POSITION;
	if (key == '(' || key == '{' || key == '[')
	{
		hit = pos;
	}
	else if (pre_key == ')' || pre_key == '}' || pre_key == ']')
	{
		hit = pos - 1;
	}

	// Brace action
	if (hit != wxSTC_INVALID_POSITION)
	{
		int match = sourceText->BraceMatch(hit);
		if (match != wxSTC_INVALID_POSITION)
		{
			sourceText->BraceHighlight(match, hit);
		}
		else
		{
			sourceText->BraceBadLight(hit);
		}
	}
	else
	{
		sourceText->BraceBadLight(wxSTC_INVALID_POSITION); // Remove any brace highlight
	}
}

void ScriptEditorFrameImpl::OnKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == 'S' && event.ControlDown())
	{
		SaveScript();
	}
	else
	{
		event.Skip();
	}
}

void ScriptEditorFrameImpl::SaveClicked(wxCommandEvent& event)
{
	SaveScript();
}

void ScriptEditorFrameImpl::SaveScript()
{
	if (currentFileName.empty() == false)
	{
		std::ofstream out(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + currentFileName, std::ios_base::binary);
		out << sourceText->GetTextRaw();
	}
}

void ScriptEditorFrameImpl::UpdateTitle()
{
	if (currentFileName.empty())
	{
		SetTitle("Script Editor");
	}
	else
	{
		SetTitle("Script Editor - " + Filesystem::GetLastPathComponent(currentFileName));
	}
}

void ScriptEditorFrameImpl::OnClose(wxCloseEvent& event)
{
	this->Hide();
	event.Veto();
}
