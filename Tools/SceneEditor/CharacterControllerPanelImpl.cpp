#include "BaseComponentPanelImpl.h"
#include "CharacterControllerPanelImpl.h"
#include "app.h"
#include "wx/msgdlg.h"
#include "Commands/RemoveComponentCommand.h"
#include "Commands/CustomCommand.h"
#include "Types.h"

using namespace FireCube;

CharacterControllerPanelImpl::CharacterControllerPanelImpl(BaseComponentPanelImpl* parent) : CharacterControllerPanel(parent), parent(parent)
{
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());
	radiusTextCtrl->SetLabel(wxString::FromDouble(characterController->GetRadius()));
	heightTextCtrl->SetLabel(wxString::FromDouble(characterController->GetHeight()));
	contactOffsetTextCtrl->SetLabel(wxString::FromDouble(characterController->GetContactOffset()));
	
	parent->removeComponent->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CharacterControllerPanelImpl::RemoveComponentClicked), NULL, this);
}

CharacterControllerPanelImpl::~CharacterControllerPanelImpl()
{

}


void CharacterControllerPanelImpl::RemoveComponentClicked(wxCommandEvent& event)
{	
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());
	MyApp *theApp = ((MyApp *)wxTheApp);
	Engine *engine = theApp->fcApp.GetEngine();
	
	float radius = characterController->GetRadius();
	float height = characterController->GetHeight();
	float contactOffset = characterController->GetContactOffset();
	auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", characterController, [radius, height, contactOffset](Engine *engine, Node *node) -> Component *
	{
		CharacterController *characterController = node->CreateComponent<CharacterController>();
		characterController->SetRadius(radius);		
		characterController->SetHeight(height);
		characterController->SetContactOffset(contactOffset);

		return characterController;
	});

	theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
}

void CharacterControllerPanelImpl::RadiusChanged(wxCommandEvent& event)
{
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = characterController->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), characterController));
	double val;
	event.GetString().ToDouble(&val);
	float oldRadius = characterController->GetRadius();
	float newRadius = val;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Radius", [componentIndex, node, newRadius]()
	{		
		CharacterController *characterController = static_cast<CharacterController *>(node->GetComponents()[componentIndex]);
		characterController->SetRadius(newRadius);
	}, [componentIndex, node, oldRadius]()
	{
		CharacterController *characterController = static_cast<CharacterController *>(node->GetComponents()[componentIndex]);
		characterController->SetRadius(oldRadius);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CharacterControllerPanelImpl::HeightChanged(wxCommandEvent& event)
{
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = characterController->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), characterController));
	double val;
	event.GetString().ToDouble(&val);
	float oldHeight = characterController->GetHeight();
	float newHeight = val;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Height", [componentIndex, node, newHeight]()
	{
		CharacterController *characterController = static_cast<CharacterController *>(node->GetComponents()[componentIndex]);
		characterController->SetHeight(newHeight);
	}, [componentIndex, node, oldHeight]()
	{
		CharacterController *characterController = static_cast<CharacterController *>(node->GetComponents()[componentIndex]);
		characterController->SetHeight(oldHeight);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CharacterControllerPanelImpl::ContactOffsetChanged(wxCommandEvent& event)
{
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = characterController->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), characterController));
	double val;
	event.GetString().ToDouble(&val);
	float oldContactOffset = characterController->GetContactOffset();
	float newContactOffset = val;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Contact Offset", [componentIndex, node, newContactOffset]()
	{
		CharacterController *characterController = static_cast<CharacterController *>(node->GetComponents()[componentIndex]);
		characterController->SetContactOffset(newContactOffset);
	}, [componentIndex, node, oldContactOffset]()
	{
		CharacterController *characterController = static_cast<CharacterController *>(node->GetComponents()[componentIndex]);
		characterController->SetContactOffset(oldContactOffset);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}


