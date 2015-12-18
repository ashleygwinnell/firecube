#include "BaseComponentPanelImpl.h"
#include "RigidBodyPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/RigidBodyDescriptor.h"

using namespace FireCube;

RigidBodyPanelImpl::RigidBodyPanelImpl(BaseComponentPanelImpl* parent) : RigidBodyPanel(parent), parent(parent)
{
	RigidBodyDescriptor *rigidBody = static_cast<RigidBodyDescriptor *>(parent->GetComponent());

	massTextCtrl->SetLabel(wxString::FromDouble(rigidBody->GetMass()));	
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
	}, [rigidBody, oldMass]()
	{
		rigidBody->SetMass(oldMass);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}
