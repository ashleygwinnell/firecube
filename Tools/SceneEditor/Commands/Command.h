#pragma once

class EditorState;

class Command
{
public:
	Command(EditorState *editorState, const std::string &description) : editorState(editorState), description(description) {}
	virtual ~Command() {};
	virtual void Do() = 0;
	virtual void Undo() = 0;

	const std::string &GetDescription() const { return description; }

protected:
	EditorState *editorState;
	std::string description;
	
};