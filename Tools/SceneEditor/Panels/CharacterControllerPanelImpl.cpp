#include "BaseComponentPanelImpl.h"
#include "CharacterControllerPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CharacterControllerDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

CharacterControllerPanelImpl::CharacterControllerPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : CharacterControllerPanel(parent), parent(parent), Object(engine)
{
	MyApp *theApp = ((MyApp *)wxTheApp);
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());
	
	UpdateUI();

	SubscribeToEvent(characterController->componentChanged, &CharacterControllerPanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &CharacterControllerPanelImpl::UndoPerformed);

	EventBindingHelpers::BindTextCtrl<float, CharacterControllerDescriptor>(radiusTextCtrl, characterController, engine, theApp->GetEditorState(), "Change Radius", [](CharacterControllerDescriptor *characterController) {
		return characterController->GetRadius();
	}, [](CharacterControllerDescriptor *characterController, const float &newVal) {
		characterController->SetRadius(newVal);
	}, [](CharacterControllerDescriptor *characterController, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);

	EventBindingHelpers::BindTextCtrl<float, CharacterControllerDescriptor>(heightTextCtrl, characterController, engine, theApp->GetEditorState(), "Change Height", [](CharacterControllerDescriptor *characterController) {
		return characterController->GetHeight();
	}, [](CharacterControllerDescriptor *characterController, const float &newVal) {
		characterController->SetHeight(newVal);
	}, [](CharacterControllerDescriptor *characterController, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);

	EventBindingHelpers::BindTextCtrl<float, CharacterControllerDescriptor>(contactOffsetTextCtrl, characterController, engine, theApp->GetEditorState(), "Change Contact Offset", [](CharacterControllerDescriptor *characterController) {
		return characterController->GetContactOffset();
	}, [](CharacterControllerDescriptor *characterController, const float &newVal) {
		characterController->SetContactOffset(newVal);
	}, [](CharacterControllerDescriptor *characterController, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevFloatVal);
}

CharacterControllerPanelImpl::~CharacterControllerPanelImpl()
{

}

void CharacterControllerPanelImpl::UpdateUI()
{
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());
	radiusTextCtrl->ChangeValue(wxString::FromDouble(characterController->GetRadius()));
	heightTextCtrl->ChangeValue(wxString::FromDouble(characterController->GetHeight()));
	contactOffsetTextCtrl->ChangeValue(wxString::FromDouble(characterController->GetContactOffset()));
}

void CharacterControllerPanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}

