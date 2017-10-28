#include "BaseComponentPanelImpl.h"
#include "CharacterControllerPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CharacterControllerDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

CharacterControllerPanelImpl::CharacterControllerPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : CharacterControllerPanel(parent), parent(parent), 
	PanelCommon(engine, static_cast<CharacterControllerDescriptor *>(parent->GetComponent()))
{
	UpdateUI();

	BindTextCtrl(radiusTextCtrl, "Change Radius", &CharacterControllerDescriptor::GetRadius, &CharacterControllerDescriptor::SetRadius);
	BindTextCtrl(heightTextCtrl, "Change Height", &CharacterControllerDescriptor::GetHeight, &CharacterControllerDescriptor::SetHeight);
	BindTextCtrl(contactOffsetTextCtrl, "Change Contact Offset", &CharacterControllerDescriptor::GetContactOffset, &CharacterControllerDescriptor::SetContactOffset);
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
