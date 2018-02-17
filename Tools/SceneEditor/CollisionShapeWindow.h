#pragma once

#include "FireCube.h"
#include "ImguiHelpers.h"
#include "imguifilesystem.h"

class NodeDescriptor;
class EditorState;
class CollisionShapeDescriptor;

class CollisionShapeWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(CollisionShapeWindow);
public:
	CollisionShapeWindow(FireCube::Engine *engine);
	void Render(EditorState *editorState, CollisionShapeDescriptor *descriptor);
private:	
	Vec3InputHelper boxMinInput;
	Vec3InputHelper boxMaxInput;
	Vec4InputHelper planeInput;
	FloatInputHelper sphereRadiusInput;
	CheckBoxHelper triggerCheckBox;
};