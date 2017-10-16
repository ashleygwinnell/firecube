#include "BaseComponentPanelImpl.h"
#include "RigidBodyPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/RigidBodyDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

RigidBodyPanelImpl::RigidBodyPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : RigidBodyPanel(parent), parent(parent), Object(engine), skipUiUpdate(false)
{
	MyApp *theApp = ((MyApp *)wxTheApp);
	RigidBodyDescriptor *rigidBody = static_cast<RigidBodyDescriptor *>(parent->GetComponent());

	UpdateUI();

	SubscribeToEvent(rigidBody->componentChanged, &RigidBodyPanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &RigidBodyPanelImpl::UndoPerformed);

	EventBindingHelpers::BindTextCtrl<float, RigidBodyDescriptor>(massTextCtrl, rigidBody, engine, theApp->GetEditorState(), "Change Mass", [](RigidBodyDescriptor *rigidBody) {
		return rigidBody->GetMass();
	}, [](RigidBodyDescriptor *rigidBody, const float &mass) {
		rigidBody->SetMass(mass);
	}, [this](RigidBodyDescriptor *rigidBody, wxCommandEvent &evt) {
		skipUiUpdate = true;
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevMass);
}

RigidBodyPanelImpl::~RigidBodyPanelImpl()
{

}

void RigidBodyPanelImpl::UpdateUI()
{
	if (skipUiUpdate)
	{
		skipUiUpdate = false;
		return;
	}
	RigidBodyDescriptor *rigidBody = static_cast<RigidBodyDescriptor *>(parent->GetComponent());

	massTextCtrl->ChangeValue(wxString::FromDouble(rigidBody->GetMass()));
}

void RigidBodyPanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}