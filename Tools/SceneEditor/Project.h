#pragma once

#include <string>

class EditorState;

class CProject
{
public:
	void Load(const std::string &filename);
	void Save(EditorState *editorState, const std::string &filename);

	std::string lastSceneFile;
	std::string gameExecutablePath;
};