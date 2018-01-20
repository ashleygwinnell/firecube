#include "CharacterControllerWindow.h"
#include "Descriptors/CharacterControllerDescriptor.h"
#include "Commands/CustomCommand.h"
#include "EditorState.h"

using namespace FireCube;

CharacterControllerWindow::CharacterControllerWindow(FireCube::Engine *engine) : Object(engine)
{

}

void CharacterControllerWindow::Render(EditorState *editorState, CharacterControllerDescriptor *descriptor)
{
	if (ImGui::CollapsingHeader("CharacterController", ImGuiTreeNodeFlags_DefaultOpen))
	{
		radiusInput.Render("Radius", editorState, "Change Radius", [descriptor]() {
			return descriptor->GetRadius();
		}, [descriptor](float newValue) {
			descriptor->SetRadius(newValue);
		});

		heightInput.Render("Height", editorState, "Change Height", [descriptor]() {
			return descriptor->GetHeight();
		}, [descriptor](float newValue) {
			descriptor->SetHeight(newValue);
		});

		contactOffsetInput.Render("Contact Offset", editorState, "Change Contact Offset", [descriptor]() {
			return descriptor->GetContactOffset();
		}, [descriptor](float newValue) {
			descriptor->SetContactOffset(newValue);
		});
	}
}

