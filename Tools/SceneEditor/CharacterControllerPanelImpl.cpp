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
	radiusXTextCtrl->SetLabel(wxString::FromDouble(characterController->GetRadius().x));
	radiusYTextCtrl->SetLabel(wxString::FromDouble(characterController->GetRadius().y));
	radiusZTextCtrl->SetLabel(wxString::FromDouble(characterController->GetRadius().z));

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
	
	vec3 radius = characterController->GetRadius();
	auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), characterController, [radius](Engine *engine, Node *node) -> Component *
	{
		CharacterController *characterController = node->CreateComponent<CharacterController>();
		characterController->SetRadius(radius);		

		return characterController;
	});

	theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
}

void CharacterControllerPanelImpl::RadiusXChanged(wxCommandEvent& event)
{
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = characterController->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), characterController));
	double val;
	event.GetString().ToDouble(&val);
	vec3 oldRadius = characterController->GetRadius();
	vec3 newRadius(val, oldRadius.y, oldRadius.z);		

	auto command = new CustomCommand(theApp->GetEditorState(), [componentIndex, node, newRadius]()
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

void CharacterControllerPanelImpl::RadiusYChanged(wxCommandEvent& event)
{
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = characterController->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), characterController));
	double val;
	event.GetString().ToDouble(&val);
	vec3 oldRadius = characterController->GetRadius();
	vec3 newRadius(oldRadius.x, val, oldRadius.z);

	auto command = new CustomCommand(theApp->GetEditorState(), [componentIndex, node, newRadius]()
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

void CharacterControllerPanelImpl::RadiusZChanged(wxCommandEvent& event)
{
	CharacterController *characterController = static_cast<CharacterController *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = characterController->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), characterController));
	double val;
	event.GetString().ToDouble(&val);
	vec3 oldRadius = characterController->GetRadius();
	vec3 newRadius(oldRadius.x, oldRadius.y, val);

	auto command = new CustomCommand(theApp->GetEditorState(), [componentIndex, node, newRadius]()
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
