#include "BaseComponentPanelImpl.h"
#include "CharacterControllerPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CharacterControllerDescriptor.h"

using namespace FireCube;

CharacterControllerPanelImpl::CharacterControllerPanelImpl(BaseComponentPanelImpl* parent) : CharacterControllerPanel(parent), parent(parent)
{
	CharacterControllerDescriptor *characterController = static_cast<CharacterControllerDescriptor *>(parent->GetComponent());
	radiusTextCtrl->SetLabel(wxString::FromDouble(characterController->GetRadius()));
	heightTextCtrl->SetLabel(wxString::FromDouble(characterController->GetHeight()));
	contactOffsetTextCtrl->SetLabel(wxString::FromDouble(characterController->GetContactOffset()));
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
	}, [characterController, oldRadius]()
	{		
		characterController->SetRadius(oldRadius);
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
	}, [characterController, oldHeight]()
	{	
		characterController->SetHeight(oldHeight);
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
	}, [characterController, oldContactOffset]()
	{		
		characterController->SetContactOffset(oldContactOffset);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}


