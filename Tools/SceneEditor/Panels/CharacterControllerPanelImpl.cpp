#include "BaseComponentPanelImpl.h"
#include "CharacterControllerPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CharacterControllerDescriptor.h"

using namespace FireCube;

CharacterControllerPanelImpl::CharacterControllerPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : CharacterControllerPanel(parent), parent(parent), Object(engine)
{
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());
	
	UpdateUI();

	SubscribeToEvent(characterController->componentChanged, &CharacterControllerPanelImpl::UpdateUI);
}

CharacterControllerPanelImpl::~CharacterControllerPanelImpl()
{

}

void CharacterControllerPanelImpl::RadiusChanged(wxCommandEvent& event)
{
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double val;
	event.GetString().ToDouble(&val);
	float oldRadius = characterController->GetRadius();
	float newRadius = val;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Radius", [characterController, newRadius]()
	{				
		characterController->SetRadius(newRadius);
		characterController->componentChanged(nullptr);
	}, [characterController, oldRadius]()
	{		
		characterController->SetRadius(oldRadius);
		characterController->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CharacterControllerPanelImpl::HeightChanged(wxCommandEvent& event)
{
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double val;
	event.GetString().ToDouble(&val);
	float oldHeight = characterController->GetHeight();
	float newHeight = val;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Height", [characterController, newHeight]()
	{		
		characterController->SetHeight(newHeight);
		characterController->componentChanged(nullptr);
	}, [characterController, oldHeight]()
	{	
		characterController->SetHeight(oldHeight);
		characterController->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CharacterControllerPanelImpl::ContactOffsetChanged(wxCommandEvent& event)
{
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double val;
	event.GetString().ToDouble(&val);
	float oldContactOffset = characterController->GetContactOffset();
	float newContactOffset = val;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Contact Offset", [characterController, newContactOffset]()
	{	
		characterController->SetContactOffset(newContactOffset);
		characterController->componentChanged(nullptr);
	}, [characterController, oldContactOffset]()
	{		
		characterController->SetContactOffset(oldContactOffset);
		characterController->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CharacterControllerPanelImpl::UpdateUI()
{
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());
	radiusTextCtrl->ChangeValue(wxString::FromDouble(characterController->GetRadius()));
	heightTextCtrl->ChangeValue(wxString::FromDouble(characterController->GetHeight()));
	contactOffsetTextCtrl->ChangeValue(wxString::FromDouble(characterController->GetContactOffset()));
}

