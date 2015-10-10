#include "BaseComponentPanelImpl.h"
#include <wx/msgdlg.h>
#include "wx/filedlg.h"
#include "wx/dirdlg.h"
#include "../app.h"
#include "../Commands/RemoveComponentCommand.h"

BaseComponentPanelImpl::BaseComponentPanelImpl(wxWindow* parent, ComponentDescriptor *componentDesc) : BaseComponentPanel(parent), theApp((MyApp*)wxTheApp), editorState(theApp->GetEditorState()), componentDesc(componentDesc)
{
	componentTypeLabel->SetLabelText(componentDesc->GetTypeName());
}

void BaseComponentPanelImpl::SetComponent(ComponentDescriptor *componentDesc)
{
	this->componentDesc = componentDesc;
	componentTypeLabel->SetLabelText(componentDesc->GetComponent()->GetTypeName());
}

void BaseComponentPanelImpl::AddControl(wxWindow *control)
{
	customComponentsSizer->Add(control, 0, wxALL | wxEXPAND, 1);	
}

ComponentDescriptor *BaseComponentPanelImpl::GetComponent()
{
	return componentDesc;
}

void BaseComponentPanelImpl::RemoveComponentClicked(wxCommandEvent& event)
{	
	MyApp *theApp = ((MyApp *)wxTheApp);
	auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", componentDesc->GetParent(), componentDesc, theApp->fcApp.GetEngine());
	theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
}
