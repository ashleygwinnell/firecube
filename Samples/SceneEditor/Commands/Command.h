#pragma once

class NodeDescriptor;
class EditorState;

class Command
{
public:
	Command(EditorState *editorState) : editorState(editorState) {}
	virtual ~Command() {};
	virtual void Do() = 0;
	virtual void Undo() = 0;
protected:
	EditorState *editorState;
	
};