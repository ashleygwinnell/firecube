#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class CharacterControllerDescriptor;

class CharacterControllerWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(CharacterControllerWindow);
public:
	CharacterControllerWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, CharacterControllerDescriptor *descriptor);
private:
	FloatInputHelper radiusInput;
	FloatInputHelper heightInput;
	FloatInputHelper contactOffsetInput;
};