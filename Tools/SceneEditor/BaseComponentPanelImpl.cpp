#include "BaseComponentPanelImpl.h"
#include <wx/msgdlg.h>
#include "wx/filedlg.h"
#include "wx/dirdlg.h"
#include "app.h"
#include "Types.h"
#include "Commands/AddNodeCommand.h"
#include "Commands/RemoveNodeCommand.h"
#include "Commands/RenameNodeCommand.h"
#include "Commands/AddComponentCommand.h"
#include "Commands/GroupCommand.h"
#include "SceneWriter.h"

BaseComponentPanelImpl::BaseComponentPanelImpl(wxWindow* parent, FireCube::Component *component) : BaseComponentPanel(parent), theApp((MyApp*)wxTheApp), editorState(theApp->GetEditorState()), component(component)
{
	componentTypeLabel->SetLabelText(component->GetTypeName());
}

void BaseComponentPanelImpl::SetComponent(FireCube::Component *component)
{
	this->component = component;
	componentTypeLabel->SetLabelText(component->GetTypeName());
}

void BaseComponentPanelImpl::AddControl(wxWindow *control)
{
	customComponentsSizer->Add(control, 0, wxALL | wxEXPAND, 1);	
}

FireCube::Component *BaseComponentPanelImpl::GetComponent()
{
	return component;
}
