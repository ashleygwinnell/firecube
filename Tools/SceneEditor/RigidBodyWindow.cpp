#include "RigidBodyWindow.h"
#include "Descriptors/RigidBodyDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"

using namespace FireCube;

RigidBodyWindow::RigidBodyWindow(FireCube::Engine *engine) : Object(engine)
{

}

void RigidBodyWindow::Render(EditorState *editorState, RigidBodyDescriptor *descriptor)
{
	massInput.Render("Mass", editorState, "Change Mass", [descriptor]() {
		return descriptor->GetMass();
	}, [descriptor](float newValue) {
		descriptor->SetMass(newValue);
	});
}

