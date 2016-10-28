#include "BaseComponentPanelImpl.h"
#include "RigidBodyPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/RigidBodyDescriptor.h"

using namespace FireCube;

RigidBodyPanelImpl::RigidBodyPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : RigidBodyPanel(parent), parent(parent), Object(engine)
{
	RigidBodyDescriptor *rigidBody = static_cast<RigidBodyDescriptor *>(parent->GetComponent());

	UpdateUI();

	SubscribeToEvent(rigidBody->componentChanged, &RigidBodyPanelImpl::UpdateUI);
}

RigidBodyPanelImpl::~RigidBodyPanelImpl()
{

}

void RigidBodyPanelImpl::MassChanged(wxCommandEvent& event)
{
	RigidBodyDescriptor *rigidBody = static_cast<RigidBodyDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);	

	double mass;
	event.GetString().ToDouble(&mass);
	float oldMass = rigidBody->GetMass();
	float newMass = mass;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mass", [rigidBody, newMass]()
	{
		rigidBody->SetMass(newMass);
		rigidBody->componentChanged(false);
	}, [rigidBody, oldMass]()
	{
		rigidBody->SetMass(oldMass);
		rigidBody->componentChanged(false);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void RigidBodyPanelImpl::UpdateUI()
{
	RigidBodyDescriptor *rigidBody = static_cast<RigidBodyDescriptor *>(parent->GetComponent());

	massTextCtrl->ChangeValue(wxString::FromDouble(rigidBody->GetMass()));
}
