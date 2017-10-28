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

RigidBodyPanelImpl::RigidBodyPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : RigidBodyPanel(parent), parent(parent), 
	PanelCommon(engine, static_cast<RigidBodyDescriptor *>(parent->GetComponent()))
{
	UpdateUI();

	BindTextCtrl(massTextCtrl, "Change Mass", &RigidBodyDescriptor::GetMass, &RigidBodyDescriptor::SetMass);
}

RigidBodyPanelImpl::~RigidBodyPanelImpl()
{

}

void RigidBodyPanelImpl::UpdateUI()
{
	RigidBodyDescriptor *rigidBody = static_cast<RigidBodyDescriptor *>(parent->GetComponent());

	ChangeValue(massTextCtrl, wxString::FromDouble(rigidBody->GetMass()));
}