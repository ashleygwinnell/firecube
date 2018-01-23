#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"

class NodeDescriptor;
class EditorState;
class CameraDescriptor;

class CameraWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(BoxWindow);
public:
	CameraWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, CameraDescriptor *descriptor);
private:
	FloatInputHelper zNearInput;
	FloatInputHelper zFarInput;
	FloatInputHelper leftInput;
	FloatInputHelper rightInput;
	FloatInputHelper bottomInput;
	FloatInputHelper topInput;
	FloatInputHelper fovInput;
};